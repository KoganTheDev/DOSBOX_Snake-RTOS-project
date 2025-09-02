#include "snake.h"
#include "game_m~1.h"
#include "keyboa~1.h"
#include "food.h"
#include "score.h"
#include "music.h"

SNAKE snake; // Global struct for the snake in the game

char ch_arr[ARR_SIZE];
int front = -1; // Index of the front of the queue
int rear = -1; // Index of the rear of the queue


void initialize_snake()
{
    int start_x;
    int start_y;
    int i;

    // TODO: for difficulty, create a variable for initial length, when leveling up,
   // TODO: increase the length by 1 or 2.
    snake.length = 5; // Initial length of the snake
    snake.direction = RIGHT_ARROW; // Initial direction of the snake

    // Position the snake in the center of the display
    start_x = DISPLAY_COLS / 2;
    start_y = DISPLAY_ROWS / 2;


    for (i = 0; i < snake.length; i++)
    {
        // TODO: Adjust here if the snake is out-of-bounds at start.
        snake.body[i].x = start_x - i; // Horizontal placement
        snake.body[i].y = start_y; // Vertical placement
    }
}

// TODO: Implement collision detection with walls
void move_snake()
{
    int i;
    POSITION* head = &snake.body[0];
    POSITION prev_head = *head; // Store previous head position

    // Move body segments
    for (i = snake.length - 1; i > 0; i--)
    {
        snake.body[i] = snake.body[i - 1];
    }

    // Update head position based on direction
    head = &snake.body[0];

    // Update the head position based on direction
    switch (snake.direction)
    {
    case UP_ARROW:
        head->y--; // Go up
        break;
    case DOWN_ARROW:
        head->y++; // Go down
        break;
    case LEFT_ARROW:
        head->x--; // Go left
        break;
    case RIGHT_ARROW:
        head->x++; // Go right
        break;
    }

    // Check if the snale has eaten the food
    if (head->x == food.x && head->y == food.y)
    {
        play_snake_eat_food_sound(); 
        // Increase the length of the snake
        if (snake.length < SNAKE_MAX_LENGTH)
        {
            snake.length++;
            for (i = snake.length; i > 0; i--)
            {
                snake.body[i] = snake.body[i - 1];
            }
            snake.body[0] = *head;
        }

        // Increase Score
        // TODO: ADD score multiplayer for fun!
        score += 10;

        // TODO: Implement level up mechanism by making the game harder

        // TODO: Maybe do somehting cool like displaying a message or sound effect
        spawn_food(); // Spawn new target for the snake
    }
}

void update_snake_direction()
{
    if (front != -1 && rear >= front)
    {
        int new_direction = ch_arr[front++];

        // Ensure the new direction is not directly opposite to the current direction
        if ((new_direction == UP_ARROW && snake.direction != DOWN_ARROW) ||
            (new_direction == DOWN_ARROW && snake.direction != UP_ARROW) ||
            (new_direction == LEFT_ARROW && snake.direction != RIGHT_ARROW) ||
            (new_direction == RIGHT_ARROW && snake.direction != LEFT_ARROW))
        {
            snake.direction = new_direction;
        }
        if (front > rear) // Reset queue if empty
        {
            front = -1;
            rear = -1;
        }
    }
}

void draw_snake()
{
    int i;

    for (i = 0; i < snake.length; i++)
    {
        int x = snake.body[i].x;
        int y = snake.body[i].y;

        if (0 <= x && x < DISPLAY_COLS && 0 <= y && y < DISPLAY_ROWS)
        {
            if (i == 0) // Head segment
            {
                display_draft[y][x] = '@';
            }
            else
            {
                // Change body segments according to the direction of movement
                if (UP_ARROW == snake.direction || snake.direction == DOWN_ARROW) // Vertical movement
                {
                    display_draft[y][x] = '|'; // Head is '@', body is '|'
                }
                else // horizontal movement
                {
                    display_draft[y][x] =  '='; // Head is '@', body is '='
                }
            }
        }
    }
}