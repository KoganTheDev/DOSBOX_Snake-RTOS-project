#include "wall.h"
#include "food.h"  // used to wrap the wall around the food
#include "snake.h"
#include "game_m~1.h"
#define MAX_WALL_LENGTH 16  // Increased to handle two walls of max 8 each

// Define display dimensions (adjust to your game)
#define DISPLAY_WIDTH 80
#define DISPLAY_HEIGHT 25

WALL wall[MAX_WALL_LENGTH];
int current_wall_count = 0;  // Track how many wall pieces are active

int get_wall_height_for_level()
{
    if (level == 1) return 4;
    else if (level == 2) return 6;
    else if (level == 3) return 8;
    else return 8;  // Level 4+ uses 8-height walls
}

int should_spawn_dual_walls()
{
    return (level >= 4);
}

void spawn_single_wall(int start_x, int start_y, int wall_height, int wall_start_index, int direction)
{
    int i, s;
    int safe;

    // Ensure wall stays within bounds
    if (start_y + wall_height >= DISPLAY_HEIGHT)
        start_y = DISPLAY_HEIGHT - wall_height - 1;
    if (start_y < 0)
        start_y = 0;

    if (direction > 0) {  // Right side wall
        if (start_x + wall_height >= DISPLAY_WIDTH)
            start_x = DISPLAY_WIDTH - wall_height - 1;
    } else {  // Left side wall
        if (start_x - wall_height < 0)
            start_x = wall_height;
    }

    // Ensure wall does not overlap snake or food
    safe = 0;
    while (!safe)
    {
        safe = 1;
        for (i = 0; i < wall_height; i++)
        {
            int wx, wy;
            
            if (direction > 0) {  // Right side curved pattern: \ \ \ / / /
                if (i < wall_height / 2) {
                    // First half: \ with increasing x offset
                    wx = start_x + i;
                    wy = start_y + i;
                } else {
                    // Second half: / with decreasing x offset
                    int rev_i = wall_height - 1 - i;
                    wx = start_x + rev_i;
                    wy = start_y + i;
                }
            } else {  // Left side curved pattern: / / / \ \ \
                if (i < wall_height / 2) {
                    // First half: / with decreasing x offset
                    wx = start_x - i;
                    wy = start_y + i;
                } else {
                    // Second half: \ with increasing x offset
                    int rev_i = wall_height - 1 - i;
                    wx = start_x - rev_i;
                    wy = start_y + i;
                }
            }

            // Check overlap with food
            if (wx >= food.x && wx <= food.x + 4 && wy >= food.y && wy <= food.y + 2)
            {
                safe = 0;
                if (direction > 0) {
                    start_x++;
                    if (start_x + wall_height >= DISPLAY_WIDTH) {
                        start_x = food.x + 6;
                        start_y++;
                    }
                } else {
                    start_x--;
                    if (start_x - wall_height < 0) {
                        start_x = food.x - 4;
                        start_y++;
                    }
                }
                break;
            }

            // Check overlap with snake
            for (s = 0; s < snake.length; s++)
            {
                if (wx == snake.body[s].x && wy == snake.body[s].y)
                {
                    safe = 0;
                    if (direction > 0) {
                        start_x++;
                        if (start_x + wall_height >= DISPLAY_WIDTH) {
                            start_x = food.x + 6;
                            start_y++;
                        }
                    } else {
                        start_x--;
                        if (start_x - wall_height < 0) {
                            start_x = food.x - 4;
                            start_y++;
                        }
                    }
                    break;
                }
            }
            if (!safe) break;
        }
    }

    // Place the wall with curved pattern
    for (i = 0; i < wall_height; i++)
    {
        int wall_index = wall_start_index + i;
        
        if (direction > 0) {  // Right side wall
            if (i < wall_height / 2) {
                // First half: \ with increasing x offset
                wall[wall_index].position.x = start_x + i;
                wall[wall_index].position.y = start_y + i;
                wall[wall_index].appearance = '\\';
            } else {
                // Second half: / with decreasing x offset
                int rev_i = wall_height - 1 - i;
                wall[wall_index].position.x = start_x + rev_i;
                wall[wall_index].position.y = start_y + i;
                wall[wall_index].appearance = '/';
            }
        } else {  // Left side wall
            if (i < wall_height / 2) {
                // First half: / with decreasing x offset
                wall[wall_index].position.x = start_x - i;
                wall[wall_index].position.y = start_y + i;
                wall[wall_index].appearance = '/';
            } else {
                // Second half: \ with increasing x offset
                int rev_i = wall_height - 1 - i;
                wall[wall_index].position.x = start_x - rev_i;
                wall[wall_index].position.y = start_y + i;
                wall[wall_index].appearance = '\\';
            }
        }
    }
}

void spawn_wall()
{
    int wall_height;
    int left_start_x, left_start_y;
    int right_start_x, right_start_y;
    int start_x, start_y;
    
    wall_height = get_wall_height_for_level();
    
    if (should_spawn_dual_walls())
    {
        // Level 4+: Spawn two walls (left and right of food)
        current_wall_count = wall_height * 2;
        
        // Left wall
        left_start_x = food.x - 4;
        left_start_y = food.y - 1;
        spawn_single_wall(left_start_x, left_start_y, wall_height, 0, -1);
        
        // Right wall
        right_start_x = food.x + 6;
        right_start_y = food.y - 1;
        spawn_single_wall(right_start_x, right_start_y, wall_height, wall_height, 1);
    }
    else
    {
        // Levels 1-3: Single wall based on snake position
        current_wall_count = wall_height;
        
        if (food.x < snake.body[0].x)
        {
            // Left of snake -> place wall to the right of food
            start_x = food.x + 6;
            start_y = food.y - 1;
            spawn_single_wall(start_x, start_y, wall_height, 0, 1);
        }
        else
        {
            // Right of snake -> place wall to the left of food
            start_x = food.x - 4;
            start_y = food.y - 1;
            spawn_single_wall(start_x, start_y, wall_height, 0, -1);
        }
    }
}

// Function to regenerate walls when level changes - call this after level increases
void regenerate_walls_for_level_change()
{
    spawn_wall();
}

void draw_wall()
{
    int i;
    for (i = 0; i < current_wall_count; i++)
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
    for (i = 0; i < current_wall_count; i++)
    {
        if (snake.body[0].x == wall[i].position.x &&
            snake.body[0].y == wall[i].position.y)
        {
            return 1; // Wall hit
        }
    }
    return 0; // No wall hit
}