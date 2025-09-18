#include <conf.h>
#include <kernel.h>
#include <io.h>
#include <bios.h>
#include <string.h>
#include <stdlib.h>

 // Game and System Constants
#define ARRSIZE 1000 // Size of keyboard input buffer
#define VIDEO_MEM_ADDR 0xB8000000 // Base address of VGA video memory (text mode)
#define SCREEN_WIDTH 80
#define SCREEN_HEIGHT 25
#define PADDLE_HEIGHT 4
#define PADDLE_LEFT_X 1 // Left paddle's X-coordinate
#define PADDLE_RIGHT_X (SCREEN_WIDTH - 2) // Right paddle's X-coordinate
#define TOP_WALL_Y 1
#define BOTTOM_WALL_Y 23
#define TOP_PLAY_AREA_Y 2 // The top row where the ball and paddles can move
#define BOTTOM_PLAY_AREA_Y 22 // The bottom row where the ball and paddles can move
#define PADDLE_MIN_Y 2 // Topmost Y position for the paddle
#define PADDLE_MAX_Y (BOTTOM_PLAY_AREA_Y - PADDLE_HEIGHT + 1) // Bottommost Y position for the paddle's top
#define STARTING_BALL_X 40 // Center X position
#define STARTING_BALL_Y 12 // Center Y position
#define PLAYER_PADDLE_CHAR '|'
#define AI_PADDLE_CHAR '|'
#define BALL_CHAR 'O'
#define TRAIL_CHAR '*'
#define BORDER_CHAR_HORIZ '-'
#define BORDER_CHAR_VERT '|'
#define BORDER_CHAR_CORNER '+'
#define SCORE_Y 0 // Y position of the score display
#define SCORE_X_OFFSET 30 // X offset for score text
#define GAME_SPEED 2 // Number of character cells the ball and paddles move per update cycle
#define TARGET_SCORE 2 // The score needed to win the game
#define TRAIL_LENGTH 5 // The number of previous ball positions to track for the trail

// Key Scan Codes and Mapped Characters
#define W_SCAN_CODE 17
#define S_SCAN_CODE 31
#define UP_ARROW_SCAN_CODE 72
#define DOWN_ARROW_SCAN_CODE 80
#define ESC_SCAN_CODE 1
#define W_CHAR 'w'
#define S_CHAR 's'

// Game States
#define STATE_PLAYING 1
#define STATE_GAME_OVER 2

// Shared Memory and Global Variables
// `entered_ascii_codes`: A raw buffer for the interrupt handler
// `ch_arr`: A character buffer used as a queue for the receiver process
char entered_ascii_codes[ARRSIZE];
int tail = -1;
char display_draft[SCREEN_HEIGHT][SCREEN_WIDTH]; // A 2D array representing the screen content

char ch_arr[ARRSIZE];
int front = -1;
int rear = -1;

// Scheduling variables used by the custom `scheduler` process
int point_in_cycle;
int gcycle_length;
int gno_of_pids;

// Game State Variables
int player_paddle_pos = 10;
int prev_player_paddle_pos = 10;
int ai_paddle_pos = 10;
int prev_ai_paddle_pos = 10;
int ball_x = STARTING_BALL_X;
int ball_y = STARTING_BALL_Y;
int prev_ball_x = STARTING_BALL_X;
int prev_ball_y = STARTING_BALL_Y;
int ball_dir_x = 1;
int ball_dir_y = 1;

char ch;
int player_score = 0;
int ai_score = 0;

int game_state = STATE_PLAYING;

// Ball trail variables using a circular buffer
struct trail_point {
    int x;
    int y;
};
struct trail_point ball_trail[TRAIL_LENGTH];
int trail_head = 0; // Index of the most recent trail point
int trail_count = 0; // The number of points currently in the buffer

// External declarations for XINU and BIOS functions
extern SYSCALL sleept(int);
extern struct intmap far* sys_imp;
extern int receiver_pid; /* Forward declaration of a global PID */

int sched_arr_pid[5] = { -1 };
int sched_arr_int[5] = { -1 };
int receiver_pid;
int shdpid; // Scheduler PID
int uppid, dispid; // Updater and Displayer PIDs

/*------------------------------------------------------------------------
 * new_int9  --  Keyboard interrupt handler
 *
 * This function replaces the default INT 9 BIOS handler. It reads the
 * keyboard scan code and ASCII value. If a valid game key is pressed,
 * it buffers the character in `entered_ascii_codes` and sends a
 * message to the `receiver` process. This is an efficient, non-blocking
 * way to handle user input in a real-time game loop.
 *------------------------------------------------------------------------
 */
INTPROC new_int9(int mdevno) {
    char result = 0;
    int scan = 0;
    int ascii = 0;

    // Use inline assembly to read keyboard status and buffer
    asm{
        MOV AH,1
        INT 16h
        JZ Skip1 // Jump if no key is in the buffer
        MOV AH,0
        INT 16h
        MOV BYTE PTR scan,AH
        MOV BYTE PTR ascii,AL
    }

        // Map keyboard scan codes to game actions
        if (scan == W_SCAN_CODE || scan == UP_ARROW_SCAN_CODE)
        {
            result = W_CHAR;
        }
        else if (scan == S_SCAN_CODE || scan == DOWN_ARROW_SCAN_CODE)
        {
            result = S_CHAR;
        }
        else if (scan == ESC_SCAN_CODE)
        {
            asm INT 27; // Terminate XINU
        }

    // Store the result if a valid key was pressed
    if ((result != 0) && (tail < ARRSIZE))
    {
        entered_ascii_codes[++tail] = result;
    }

    // Notify the receiver process that a key has been pressed
    send(receiver_pid, result);

Skip1:
}

/*------------------------------------------------------------------------
 * set_new_int9_newisr  --  Installs the new keyboard interrupt handler
 *
 * This function iterates through the XINU interrupt map (`sys_imp`)
 * and replaces the entry for interrupt vector 9 (the keyboard interrupt)
 * with our custom handler `new_int9`.
 *------------------------------------------------------------------------
 */
void set_new_int9_newisr() {
    int i;
    for (i = 0; i < 32; i++)
    {
        if (sys_imp[i].ivec == 9)
        {
            sys_imp[i].newisr = new_int9;
            return;
        }
    }
}

/*------------------------------------------------------------------------
 * hide_cursor  --  Hides the text cursor using a BIOS call
 *
 * Uses BIOS interrupt 10h to set the cursor's start and end scan
 * lines, effectively making it invisible by moving it off-screen.
 *------------------------------------------------------------------------
 */
void hide_cursor()
{
    asm{
        MOV AH, 01h // BIOS function to set cursor type
        MOV CH, 20h // Start scan line (off-screen)
        MOV CL, 00h // End scan line
        INT 10h
    }
}

/*------------------------------------------------------------------------
 * show_cursor  --  Restores the text cursor using a BIOS call
 *
 * Uses BIOS interrupt 10h to restore the default cursor appearance.
 *------------------------------------------------------------------------
 */
void show_cursor()
{
    asm{
        MOV AH, 01h // BIOS function to set cursor type
        MOV CH, 06h // Start scan line (standard block cursor)
        MOV CL, 07h // End scan line
        INT 10h
    }
}

/*------------------------------------------------------------------------
 * displayer  --  Displays the game state by writing to video memory
 *
 * This process acts as the renderer. It receives messages from the
 * scheduler and then copies the entire `display_draft` array to
 * video memory. The use of a draft array and a separate displayer
 * process prevents "tearing" and flickering by ensuring the screen
 * is updated all at once, rather than incrementally.
 *------------------------------------------------------------------------
 */
void displayer(void) {
    int i, j;
    char far* video_mem = (char far*)VIDEO_MEM_ADDR;

    // Clear the screen once at the beginning
    for (i = 0; i < SCREEN_WIDTH * SCREEN_HEIGHT; i++)
    {
        video_mem[i * 2] = ' ';
        video_mem[i * 2 + 1] = 0x07; // White on black attribute
    }

    while (1) {
        receive(); // Wait for a signal from the scheduler

        // Copy the entire display draft to video memory
        // Each character cell in text mode is 2 bytes: char + color attribute.
        for (i = 0; i < SCREEN_HEIGHT; i++)
        {
            for (j = 0; j < SCREEN_WIDTH; j++)
            {
                video_mem[(i * SCREEN_WIDTH + j) * 2] = display_draft[i][j];
                video_mem[(i * SCREEN_WIDTH + j) * 2 + 1] = 0x07; // White on black
            }
        }
    }
}

/*------------------------------------------------------------------------
 * receiver  --  Buffers keyboard input
 *
 * This process is woken up by the interrupt handler. It reads all
 * available characters from the interrupt buffer (`entered_ascii_codes`)
 * and transfers them to its own queue (`ch_arr`). This isolates the
 * interrupt handler from the main game loop and prevents lost input.
 *------------------------------------------------------------------------
 */
void receiver()
{
    char temp;
    while (1)
    {
        receive(); // Wait for a signal from the interrupt handler

        // Transfer all available keys from the interrupt buffer to the game's queue
        while (tail > -1)
        {
            temp = entered_ascii_codes[tail];
            rear++;
            tail--;
            if (rear < ARRSIZE)
            {
                ch_arr[rear] = temp;
            }
            if (front == -1)
            {
                front = 0;
            }
        }
    }
}


/*------------------------------------------------------------------------
 * updater  --  Contains the main game logic
 *
 * This is the heart of the game. It is woken up by the scheduler,
 * processes a single buffered key press, updates the state of all
 * game objects, and then draws the new state to the `display_draft`
 * array.
 *------------------------------------------------------------------------
 */
void updater() {
    int i, j;
    char score_str[20];
    int len;
    char* msg1;
    char* msg2;
    char* exit_msg;

    while (1)
    {
        receive(); // Wait for a signal from the scheduler

        // Process only ONE key press per frame for smoother, more predictable movement.
        ch = 0;
        if (front != -1)
        {
            ch = ch_arr[front];
            if (front != rear)
            {
                front++;
            }
            else
            {
                front = rear = -1;
            }
        }

        // --- Game Logic based on State ---
        if (game_state == STATE_PLAYING)
        {
            // Store previous paddle and ball positions for clearing in a single pass
            prev_player_paddle_pos = player_paddle_pos;
            prev_ai_paddle_pos = ai_paddle_pos;
            prev_ball_x = ball_x;
            prev_ball_y = ball_y;

            // Player paddle control
            if (ch == W_CHAR)
            {
                player_paddle_pos -= GAME_SPEED;
            }
            else if (ch == S_CHAR)
            {
                player_paddle_pos += GAME_SPEED;
            }

            // Clamp paddle positions to stay within bounds
            if (player_paddle_pos < PADDLE_MIN_Y) player_paddle_pos = PADDLE_MIN_Y;
            if (player_paddle_pos > PADDLE_MAX_Y) player_paddle_pos = PADDLE_MAX_Y;

            if (rand() % 100 > 20)  // 80% chance for the AI paddle to move
            {
                if (ball_y > ai_paddle_pos + 1)
                {
                    ai_paddle_pos += GAME_SPEED;
                }
                else if (ball_y < ai_paddle_pos + 1)
                {
                    ai_paddle_pos -= GAME_SPEED;
                }
            }

            // Clamp AI paddle position
            if (ai_paddle_pos < PADDLE_MIN_Y) ai_paddle_pos = PADDLE_MIN_Y;
            if (ai_paddle_pos > PADDLE_MAX_Y) ai_paddle_pos = PADDLE_MAX_Y;

            // Move ball
            ball_x += ball_dir_x * GAME_SPEED;
            ball_y += ball_dir_y * GAME_SPEED;

            // Ball collision with top/bottom borders
            if (ball_y <= TOP_PLAY_AREA_Y || ball_y >= BOTTOM_PLAY_AREA_Y)
            {
                ball_dir_y *= -1;
            }

            // Ball collision with player paddle (left) - now a range check
            if (ball_x <= PADDLE_LEFT_X + 1 && (ball_y >= player_paddle_pos && ball_y < player_paddle_pos + PADDLE_HEIGHT))
            {
                ball_dir_x *= -1;
            }

            // Ball collision with AI paddle (right) - now a range check
            if (ball_x >= PADDLE_RIGHT_X - 1 && (ball_y >= ai_paddle_pos && ball_y < ai_paddle_pos + PADDLE_HEIGHT))
            {
                ball_dir_x *= -1;
            }

            // Scoring
            if (ball_x <= 0)
            {
                ai_score++;
                ball_x = STARTING_BALL_X;
                ball_y = STARTING_BALL_Y;
                ball_dir_x = 1;
                ball_dir_y = 1;
                trail_head = 0;
                trail_count = 0;
            }
            else if (ball_x >= SCREEN_WIDTH)
            {
                player_score++;
                ball_x = STARTING_BALL_X;
                ball_y = STARTING_BALL_Y;
                ball_dir_x = -1;
                ball_dir_y = 1;
                trail_head = 0;
                trail_count = 0;
            }

            // Check for game over
            if (player_score >= TARGET_SCORE || ai_score >= TARGET_SCORE) {
                game_state = STATE_GAME_OVER;
            }
        }

        // --- Drawing Logic ---

        if (game_state == STATE_PLAYING)
        {
            // Clear the entire drawing area to prepare for the new frame
            for (i = 0; i < SCREEN_HEIGHT; i++)
            {
                for (j = 0; j < SCREEN_WIDTH; j++)
                {
                    display_draft[i][j] = ' ';
                }
            }

            // Draw static borders once per frame
            for (i = 1; i < SCREEN_WIDTH - 1; i++)
            {
                display_draft[TOP_WALL_Y][i] = BORDER_CHAR_HORIZ;
                display_draft[BOTTOM_WALL_Y][i] = BORDER_CHAR_HORIZ;
            }
            for (i = 2; i < SCREEN_HEIGHT - 2; i++)
            {
                display_draft[i][0] = BORDER_CHAR_VERT;
                display_draft[i][SCREEN_WIDTH - 1] = BORDER_CHAR_VERT;
            }

            // Draw corners with '+'
            display_draft[TOP_WALL_Y][0] = BORDER_CHAR_CORNER;
            display_draft[TOP_WALL_Y][SCREEN_WIDTH - 1] = BORDER_CHAR_CORNER;
            display_draft[BOTTOM_WALL_Y][0] = BORDER_CHAR_CORNER;
            display_draft[BOTTOM_WALL_Y][SCREEN_WIDTH - 1] = BORDER_CHAR_CORNER;

            // Draw paddles at their new positions
            for (i = 0; i < PADDLE_HEIGHT; i++)
            {
                display_draft[player_paddle_pos + i][PADDLE_LEFT_X] = PLAYER_PADDLE_CHAR;
                display_draft[ai_paddle_pos + i][PADDLE_RIGHT_X] = AI_PADDLE_CHAR;
            }

            // --- Corrected Trail Drawing Logic ---

            // Add current ball position to the trail history (circular buffer)
            ball_trail[trail_head].x = ball_x;
            ball_trail[trail_head].y = ball_y;
            trail_head = (trail_head + 1) % TRAIL_LENGTH;
            if (trail_count < TRAIL_LENGTH)
            {
                trail_count++;
            }

            // Draw the trail by iterating over the history.
            // We use a simple line-drawing algorithm to fill in the gaps between
            // consecutive ball positions, creating a continuous trail.
            for (i = 0; i < trail_count - 1; i++)
            {
                int curr_index = (trail_head - i - 1 + TRAIL_LENGTH) % TRAIL_LENGTH;
                int prev_index = (trail_head - i - 2 + TRAIL_LENGTH) % TRAIL_LENGTH;

                int start_x = ball_trail[prev_index].x;
                int start_y = ball_trail[prev_index].y;
                int end_x = ball_trail[curr_index].x;
                int end_y = ball_trail[curr_index].y;

                // Simple line drawing to fill in the gaps between points
                int dx = end_x - start_x;
                int dy = end_y - start_y;
                int steps = (abs(dx) > abs(dy)) ? abs(dx) : abs(dy);

                float x_increment = (float)dx / (float)steps;
                float y_increment = (float)dy / (float)steps;

                float current_x = (float)start_x;
                float current_y = (float)start_y;

                for (j = 0; j <= steps; j++)
                {
                    int draw_x = (int)(current_x + 0.5f);
                    int draw_y = (int)(current_y + 0.5f);
                    if (draw_x >= 1 && draw_x <= 78 && draw_y >= 2 && draw_y <= 22)
                    {
                        display_draft[draw_y][draw_x] = TRAIL_CHAR;
                    }
                    current_x += x_increment;
                    current_y += y_increment;
                }
            }

            // Draw ball at new position, on top of any trail characters
            display_draft[ball_y][ball_x] = BALL_CHAR;

            // Draw scores
            sprintf(score_str, "YOU: %d - AI: %d", player_score, ai_score);
            len = strlen(score_str);
            for (i = 0; i < len; i++)
            {
                display_draft[SCORE_Y][SCORE_X_OFFSET + i] = score_str[i]; // Centered on the top row
            }

            // Draw exit instructions
            exit_msg = "Press Esc to exit";
            strcpy(&display_draft[0][SCREEN_WIDTH - strlen(exit_msg) - 2], exit_msg);

        }
        else if (game_state == STATE_GAME_OVER)
        {
            show_cursor();
            // Clear the entire screen draft for the win screen
            for (i = 0; i < SCREEN_HEIGHT; i++)
            {
                for (j = 0; j < SCREEN_WIDTH; j++)
                {
                    display_draft[i][j] = ' ';
                }
            }

            msg1 = (player_score >= TARGET_SCORE) ? "YOU WIN!" : "AI WINS!";
            sprintf(score_str, "Final Score: YOU %d - AI %d", player_score, ai_score);
            msg2 = "Press 'w'\\'s'\\Up arrow\\Down arrow key to play again or Esc to exit.";

            strcpy(&display_draft[10][(SCREEN_WIDTH - strlen(msg1)) / 2], msg1);
            strcpy(&display_draft[12][(SCREEN_WIDTH - strlen(score_str)) / 2], score_str);
            strcpy(&display_draft[14][(SCREEN_WIDTH - strlen(msg2)) / 2], msg2);

            // Wait for key press to quit or restart
            if (ch != 0)
            {
                game_state = STATE_PLAYING;
                player_score = 0;
                ai_score = 0;
                ball_x = STARTING_BALL_X;
                ball_y = STARTING_BALL_Y;
                trail_head = 0;
                trail_count = 0;
                hide_cursor();
            }
        }
    }
}

/*------------------------------------------------------------------------
 * schedule  --  A custom scheduling function for the game loop
 *
 * This function sets up the parameters for the game's custom
 * scheduler process. It defines which processes (`uppid`, `dispid`)
 * to wake up and at what interval, creating a fixed game loop.
 * `gcycle_length`: The total number of `sleept` ticks in one full cycle.
 * `no_of_pids`: The number of processes to schedule.
 * `pid1, ...`: A variable-length argument list of process IDs and their
 * wakeup intervals.
 *------------------------------------------------------------------------
 */
SYSCALL schedule(int no_of_pids, int cycle_length, int pid1, ...) {
    int i;
    int ps;
    int* iptr;
    disable(ps);
    gcycle_length = cycle_length;
    point_in_cycle = 0;
    gno_of_pids = no_of_pids;
    iptr = &pid1;
    for (i = 0; i < no_of_pids; i++)
    {
        sched_arr_pid[i] = *iptr;
        iptr++;
        sched_arr_int[i] = *iptr;
        iptr++;
    }
    restore(ps);
}

/*------------------------------------------------------------------------
 * scheduler  --  The game loop master clock
 *
 * This process is the "clock" for the game loop. It runs in an infinite
 * loop, waking up the `updater` and `displayer` at predetermined intervals
 * by sending them messages. This ensures that the game logic and screen
 * updates occur at a consistent rate, leading to smooth, predictable
 * gameplay.
 *------------------------------------------------------------------------
 */
void scheduler()
{
    int interval_ticks, pid_index;
    point_in_cycle++;
    if (point_in_cycle == gcycle_length)
    {
        point_in_cycle = 0;
    }
    interval_ticks = gcycle_length / gno_of_pids;
    pid_index = 0;
    while (1)
    {
        sleept(interval_ticks);
        send(sched_arr_pid[pid_index], 11);
        pid_index++;
        if (pid_index == gno_of_pids)
            pid_index = 0;
    }
}

/*------------------------------------------------------------------------
 * xmain  --  The main entry point for the game
 *
 * This function initializes the game environment. It hides the cursor,
 * installs the custom keyboard interrupt handler, and creates and
 * resumes the four main game processes. Finally, it starts the
 * custom scheduler to begin the game loop.
 *------------------------------------------------------------------------
 */
xmain() {
    int recvpid;
    hide_cursor();
    set_new_int9_newisr();
    resume(dispid = create(displayer, INITSTK, INITPRIO, "DISPLAYER", 0));
    resume(recvpid = create(receiver, INITSTK, INITPRIO + 3, "RECIVEVER", 0));
    resume(uppid = create(updater, INITSTK, INITPRIO, "UPDATER", 0));
    receiver_pid = recvpid;

    // Use the custom schedule function to set up the game loop.
    // We schedule the updater and displayer to be woken up sequentially.
    // The cycle length of 2 ensures a fast, smooth framerate.
    schedule(2, 2, uppid, 0, dispid, 3);

    resume(shdpid = create(scheduler, INITSTK, INITPRIO + 1, "SCHEDULER", 0));
}