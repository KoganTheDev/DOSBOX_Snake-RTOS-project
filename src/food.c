#include "food.h"
#include "snake.h"


POSITION food;

void spawn_food()
{
    int i;
    int on_snake;

    do
    {
        on_snake = 0;
        food.x = rand() % DISPLAY_COLS; // Random x position
        food.y = rand() % DISPLAY_ROWS; // Random y position

        // Ensure the food does not spawn on the snake
        for (i = 0; i < snake.length; i++)
        {
            if (snake.body[i].x == food.x && snake.body[i].y == food.y)
            {
                on_snake = 1;
                break;
            }
        }
    } while (on_snake);
}