#include "food.h"
#include "snake.h"
#include "music.h"
#include "wall.h"
#include "game_m~1.h"

int food_eaten = 0;
POSITION food;

// Structure to hold apple data for different levels
typedef struct {
    const char *apple[3];
    int width;
    int height;
} AppleType;

// Define different apple types for each level
AppleType apple_types[3] = {
    // Level 1 - Original apple
    {
        {" _\\_", "(   )", " \\_/"},
        5, 3
    },
    // Level 2 - Small apple
    {
        {"  ,", " (_)", "  "},
        3, 2
    },
    // Level 3 - Dot apple
    {
        {"*", "", ""},
        1, 1
    }
};

AppleType* get_current_apple_type()
{
    // Clamp level to valid range (1-3)
    int apple_index = (level < 1) ? 0 : (level > 3) ? 2 : level - 1;
    return &apple_types[apple_index];
}

void spawn_food()
{
    int i;
    int on_snake = 0;
    AppleType *current_apple = get_current_apple_type();

    // Define the valid range for X and Y coordinates based on the limits enum
    int min_x = LEFT_LIMIT;
    int max_x = RIGHT_LIMIT;
    int min_y = UPPER_LIMIT;
    int max_y = LOWER_LIMIT;

    do
    {
        on_snake = 0;

        // Adjust range so apple always fits on screen
        food.x = min_x + rand() % (max_x - min_x - current_apple->width);
        food.y = min_y + rand() % (max_y - min_y - current_apple->height);

        // Ensure the apple does not spawn on the snake
        for (i = 0; i < snake.length; i++)
        {
            if (snake.body[i].x >= food.x && 
                snake.body[i].x < food.x + current_apple->width &&
                snake.body[i].y >= food.y && 
                snake.body[i].y < food.y + current_apple->height)
            {
                on_snake = 1;
                break;
            }
        }
    } while (on_snake);

    spawn_wall();
}

void draw_food()
{
    AppleType *current_apple = get_current_apple_type();
    int dy = 0;
    int dx = 0;

    for (dy = 0; dy < current_apple->height; dy++)
    {
        for (dx = 0; current_apple->apple[dy][dx] != '\0'; dx++)
        {
            int fx = food.x + dx;
            int fy = food.y + dy;

            display_draft[fy][fx] = current_apple->apple[dy][dx];
        }
    }
}

int is_snake_on_food()
{
    AppleType *current_apple = get_current_apple_type();
    int dy = 0;
    int dx = 0;

    for (dy = 0; dy < current_apple->height; dy++)
    {
        for (dx = 0; current_apple->apple[dy][dx] != '\0'; dx++)
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
