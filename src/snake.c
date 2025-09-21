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

    snake.length = 5; // Initial length of the snake
    snake.direction = RIGHT_ARROW; // Initial direction of the snake

    // Position the snake in the center of the display
    start_x = DISPLAY_COLS / 2;
    start_y = DISPLAY_ROWS / 2;


    for (i = 0; i < snake.length; i++)
    {
        snake.body[i].x = start_x - i; // Horizontal placement
        snake.body[i].y = start_y; // Vertical placement
    }
}

void move_snake()
{
    int i;
    POSITION prev_head = snake.body[0]; // Store previous head position

    // Move body segments (from tail to head)
    for (i = snake.length - 1; i > 0; i--)
    {
        snake.body[i] = snake.body[i - 1];
    }

    // Update head position based on direction
    switch (snake.direction)
    {
    case UP_ARROW:
        snake.body[0].y--; // Go up
        break;
    case DOWN_ARROW:
        snake.body[0].y++; // Go down
        break;
    case LEFT_ARROW:
        snake.body[0].x--; // Go left
        break;
    case RIGHT_ARROW:
        snake.body[0].x++; // Go right
        break;
    }

    if (is_snake_on_food())
    {
        play_snake_eat_food_sound();

        // Increase the length of the snake
        if (snake.length < SNAKE_MAX_LENGTH)
        {
            snake.length++;
        }

        score += 10;
    }
}

void update_snake_direction()
{
    if (front != -1 && rear >= front)
    {
        int new_direction = ch_arr[front++];

        // TODO: Cause GAME OVER through here if the direction is oposite
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
    int prev_x, prev_y;

    // Draw the head of the snake
    if (snake.length > 0)
    {
        int head_x = snake.body[0].x;
        int head_y = snake.body[0].y;

        if (0 <= head_x && head_x < DISPLAY_COLS && 0 <= head_y && head_y < DISPLAY_ROWS)
        {
            display_draft[head_y][head_x] = '@';
        }
    }

    // Draw the body segments based on the movement of each segment
    for (i = 1; i < snake.length; i++)
    {
        int x = snake.body[i].x;
        int y = snake.body[i].y;

        // Check if the segment is within display boundaries
        if (0 <= x && x < DISPLAY_COLS && 0 <= y && y < DISPLAY_ROWS)
        {
            prev_x = snake.body[i - 1].x;
            prev_y = snake.body[i - 1].y;

            // Determine if the movement is horizontal or vertical
            if (x == prev_x) // Vertical movement
            {
                display_draft[y][x] = '|';
            }
            else if (y == prev_y) // Horizontal movement
            {
                display_draft[y][x] = '=';
            }
        }
    }
}

int snake_self_collision()
{
    int i;
    POSITION snake_head = snake.body[0];

    // Start from the first body segment (index 1) and check against the head (index 0)
    for (i = 1; i < snake.length; i++)
    {
        if (snake_head.x == snake.body[i].x && snake_head.y == snake.body[i].y)
        {
            play_snake_dies_sound();
            return 1; // Collision detected
        }
    }

    return 0; // No collision
}
