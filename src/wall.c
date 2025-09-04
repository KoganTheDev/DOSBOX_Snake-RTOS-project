#include "wall.h"
#include "food.h" // used to wrap the wall around the food
#include "snake.h"

WALL wall;

void spawn_wall()
{
	// TODO: When the wall is large, it has to be a bit far from the food
	//! NOTE:
	//! When creating food check if there`s enough space to create a food + wall object while making
	//! sure non of them will be on the snake
	

	//! Possible bug: what if the wall is spawned on the same column the snake is at
	// Check the location the food was spawned at related to the head of the snake
	if (food.x < snake.body[0].x)
	{
		// The food is to the left of the snake
		// Wall is '>'
		wall.position.x = food.x + 1;
		wall.position.y = food.y;
		wall.appearence = '>';
	}
	else
	{
		// The food is on the right of the snake
		// Wall is '<'
		wall.position.x = food.x - 1;
		wall.position.y = food.y;
		wall.appearence = '<';
	}
}

void draw_wall()
{
	// Insert the wall character to the display draft
	display_draft[wall.position.y][wall.position.x] = wall.appearence;
}

int is_snake_on_wall()
{
	if (snake.body[0].x == wall.position.x &&
		snake.body[0].y == wall.position.y)
	{
		return 1; // Wall hit
	}
	return 0; // No wall hit
}