#include "food.h"
#include "snake.h"
#include "music.h"
#include "wall.h"

int food_eaten = 0;

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

        // Adjust range so apple (width up to 5 chars) always fits on screen
        food.x = min_x + rand() % (max_x - min_x - 4);
        food.y = min_y + rand() % (max_y - min_y - 2);

        // Ensure the apple does not spawn on the snake
        for (i = 0; i < snake.length; i++)
        {
            if (snake.body[i].x >= food.x && snake.body[i].x <= food.x + 4 &&
                snake.body[i].y >= food.y && snake.body[i].y <= food.y + 2)
            {
                on_snake = 1;
                break;
            }
        }
    } while (on_snake);

    //! Will need to be refactored to a separate fucntion
    //! Can call it spawn_objects and use it to spawn wall & food at once
    spawn_wall();
}

void draw_food()
{
    // Apple representation (3 rows, up to 5 characters wide)
    const char *apple[3] = {
        " _\\_",
        "(   )",
        " \\_/"
    };

    int dy = 0;
    int dx = 0;

    for (dy = 0; dy < 3; dy++)
    {
        for (dx = 0; apple[dy][dx] != '\0'; dx++)
        {
            int fx = food.x + dx;
            int fy = food.y + dy;

            display_draft[fy][fx] = apple[dy][dx];
        }
    }
}

int is_snake_on_food()
{
    // Apple dimensions (same as above)
    const char *apple[3] = {
        " _\\_",
        "(   )",
        " \\_/"
    };

    int dy = 0;
    int dx = 0;

    for (dy = 0; dy < 3; dy++)
    {
        for (dx = 0; apple[dy][dx] != '\0'; dx++)
        {
            if (snake.body[0].x == food.x + dx &&
                snake.body[0].y == food.y + dy)
            {
                food_eaten++;  
                spawn_food();
                return 1;
            }
        }
    }

    return 0;
}
