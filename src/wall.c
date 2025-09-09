#include "wall.h"
#include "food.h"  // used to wrap the wall around the food
#include "snake.h"

#define WALL_LENGTH 6

// Define display dimensions (adjust to your game)
#define DISPLAY_WIDTH 80
#define DISPLAY_HEIGHT 25

WALL wall[WALL_LENGTH];

void spawn_wall()
{
    int i, s;
    int start_x, start_y;
    int safe;

    if (food.x < snake.body[0].x)
    {
        // Left of snake -> place wall to the right of food (covering right side)
        start_x = food.x + 6; // start after food width
        start_y = food.y - 1; // start slightly above food

        // Keep the wall fully inside display
        if (start_x + 3 >= DISPLAY_WIDTH) // max x offset is 3 for the curve
            start_x = DISPLAY_WIDTH - 4;
        if (start_y + WALL_LENGTH >= DISPLAY_HEIGHT)
            start_y = DISPLAY_HEIGHT - WALL_LENGTH - 1;
        if (start_y < 0)
            start_y = 0;

        // Ensure wall does not overlap snake or food
        safe = 0;
        while (!safe)
        {
            safe = 1;
            for (i = 0; i < WALL_LENGTH; i++)
            {
                int wx, wy;
                
                // Create curved pattern: \ \ \ / / /
                if (i < 3) {
                    // First half: \ with increasing x offset
                    wx = start_x + i;
                    wy = start_y + i;
                } else {
                    // Second half: / with decreasing x offset
                    int rev_i = WALL_LENGTH - 1 - i; // reverse index for second half
                    wx = start_x + rev_i;
                    wy = start_y + i;
                }

                // Check overlap with food
                if (wx >= food.x && wx <= food.x + 4 && wy >= food.y && wy <= food.y + 2)
                {
                    safe = 0;
                    start_x++; // move wall right
                    if (start_x + 3 >= DISPLAY_WIDTH) {
                        start_x = food.x + 6;
                        start_y++;
                        if (start_y + WALL_LENGTH >= DISPLAY_HEIGHT)
                            start_y = food.y - 1;
                    }
                    break;
                }

                // Check overlap with snake
                for (s = 0; s < snake.length; s++)
                {
                    if (wx == snake.body[s].x && wy == snake.body[s].y)
                    {
                        safe = 0;
                        start_x++; // move wall right
                        if (start_x + 3 >= DISPLAY_WIDTH) {
                            start_x = food.x + 6;
                            start_y++;
                            if (start_y + WALL_LENGTH >= DISPLAY_HEIGHT)
                                start_y = food.y - 1;
                        }
                        break;
                    }
                }
                if (!safe) break;
            }
        }

        // Place the wall with curved pattern
        for (i = 0; i < WALL_LENGTH; i++)
        {
            if (i < 3) {
                // First half: \ with increasing x offset
                wall[i].position.x = start_x + i;
                wall[i].position.y = start_y + i;
                wall[i].appearance = '\\';
            } else {
                // Second half: / with decreasing x offset
                int rev_i = WALL_LENGTH - 1 - i; // reverse index for second half
                wall[i].position.x = start_x + rev_i;
                wall[i].position.y = start_y + i;
                wall[i].appearance = '/';
            }
        }
    }
    else
    {
        // Right of snake -> place wall to the left of food (covering left side)
        start_x = food.x - 4; // start before food
        start_y = food.y - 1; // start slightly above food

        if (start_x - 3 < 0) // min x offset is -3 for the curve
            start_x = 3;
        if (start_y + WALL_LENGTH >= DISPLAY_HEIGHT)
            start_y = DISPLAY_HEIGHT - WALL_LENGTH;
        if (start_y < 0)
            start_y = 0;

        // Ensure wall does not overlap snake
        safe = 0;
        while (!safe)
        {
            safe = 1;
            for (i = 0; i < WALL_LENGTH; i++)
            {
                int wx, wy;
                
                // Create curved pattern: / / / \ \ \
                if (i < 3) {
                    // First half: / with decreasing x offset
                    wx = start_x - i;
                    wy = start_y + i;
                } else {
                    // Second half: \ with increasing x offset
                    int rev_i = WALL_LENGTH - 1 - i; // reverse index for second half
                    wx = start_x - rev_i;
                    wy = start_y + i;
                }

                // Check overlap with food
                if (wx >= food.x && wx <= food.x + 4 && wy >= food.y && wy <= food.y + 2)
                {
                    safe = 0;
                    start_x--; // move wall left
                    if (start_x - 3 < 0) {
                        start_x = food.x - 4;
                        start_y++;
                        if (start_y + WALL_LENGTH >= DISPLAY_HEIGHT)
                            start_y = food.y - 1;
                    }
                    break;
                }

                // Check overlap with snake
                for (s = 0; s < snake.length; s++)
                {
                    if (wx == snake.body[s].x && wy == snake.body[s].y)
                    {
                        safe = 0;
                        start_x--; // move wall left
                        if (start_x - 3 < 0) {
                            start_x = food.x - 4;
                            start_y++;
                            if (start_y + WALL_LENGTH >= DISPLAY_HEIGHT)
                                start_y = food.y - 1;
                        }
                        break;
                    }
                }
                if (!safe) break;
            }
        }

        // Place the wall with curved pattern
        for (i = 0; i < WALL_LENGTH; i++)
        {
            if (i < 3) {
                // First half: / with decreasing x offset
                wall[i].position.x = start_x - i;
                wall[i].position.y = start_y + i;
                wall[i].appearance = '/';
            } else {
                // Second half: \ with increasing x offset
                int rev_i = WALL_LENGTH - 1 - i; // reverse index for second half
                wall[i].position.x = start_x - rev_i;
                wall[i].position.y = start_y + i;
                wall[i].appearance = '\\';
            }
        }
    }
}


void draw_wall()
{
    int i;
    for (i = 0; i < WALL_LENGTH; i++)
    {
        // Safety check: make sure we're inside display bounds
        if (wall[i].position.y >= 0 && wall[i].position.y < DISPLAY_HEIGHT &&
            wall[i].position.x >= 0 && wall[i].position.x < DISPLAY_WIDTH)
        {
            display_draft[wall[i].position.y][wall[i].position.x] = wall[i].appearance;
        }
    }
}

int is_snake_on_wall()
{
    int i;
    for (i = 0; i < WALL_LENGTH; i++)
    {
        if (snake.body[0].x == wall[i].position.x &&
            snake.body[0].y == wall[i].position.y)
        {
            return 1; // Wall hit
        }
    }
    return 0; // No wall hit
}