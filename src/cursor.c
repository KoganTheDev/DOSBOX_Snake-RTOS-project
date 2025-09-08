#include "cursor.h"


void set_full_block_cursor()
{
	// Select the Cursor Start register (0x0A)
	outportb(CRT_INDEX_PORT, CURSOR_START_REG);

	// Set the cursor start line at 0
	outportb(CRT_DATA_PORT, 0);

	// Select the Cursor End register (CLOCK_HARDWARE_INTERRUPT)
	outportb(CRT_INDEX_PORT, CURSOR_END_REG);

	// Set the cursor end line to 15 to get a full block
	outportb(CRT_DATA_PORT, 15);
}


void move_cursor_to_snake_head()
{
    // Calculate the cursor position as a single 16-bit offset
    int offset = snake.body[0].y * DISPLAY_COLS + snake.body[0].x;

    // Select Cursor Location High Register (0x0E)
    outportb(CRT_INDEX_PORT, CURSOR_HIGH_POS_REG);
    // Write the high byte of the offset
    outportb(CRT_DATA_PORT, (offset >> 8));

    // Select Cursor Location Low Register (0x0F)
    outportb(CRT_INDEX_PORT, CURSOR_LOW_POS_REG);
    // Write the low byte of the offset
    outportb(CRT_DATA_PORT, (offset & 0xFF));
}