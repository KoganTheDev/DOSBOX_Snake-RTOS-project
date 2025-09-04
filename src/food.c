#include "food.h"
#include "snake.h"
#include "music.h"


POSITION food;

void spawn_food()
{
    int i;
    int on_snake = 0;

    // Define the valid range for X and Y coordinates based on the limits enum
    int min_x = LEFT_LIMIT;
    int max_x = RIGHT_LIMIT;
    int min_y = UPPER_LIMIT;
    int max_y = LOWER_LIMIT;

    do
    {
        on_snake = 0;

        food.x = min_x + rand() % (max_x - min_x + 1);
        food.y = min_y + rand() % (max_y - min_y + 1);

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


int is_snake_on_food()
{
    // Check if the snake has eaten the food
    if (snake.body[0].x == food.x && snake.body[0].y == food.y)
    {
        spawn_food();

        return 1;
    }

    return 0;
}