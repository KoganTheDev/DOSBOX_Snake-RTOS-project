#include <stdio.h>
#include <dos.h>

#define MOUSE_IRQ 0x74
#define MOUSE_SOFTWARE_INTERRUPT 0x33

enum mouse_button
{
	NO_MOUSE_BUTTON = 0,
	LEFT_MOUSE_BUTTON = 1,
	RIGHT_MOUSE_BUTTON = 2,
	BOTH_MOUSE_BUTTONS = 3
};


void print_mouse_status(int button_status, int mouse_x, int mouse_y)
{
	printf("b = %d\nx = %d y = %d\n", button_status, mouse_x / 8, mouse_y / 8);
}

// --- Global variables ---
union REGS inregs, outregs;

// The specific interrupt to hook to.
unsigned char interrupt_number = MOUSE_IRQ;

int button_status;
int mouse_x; // holds x coordinate of mouse
int mouse_y; // holds y coordinate of mouse

// Step 1: define a global variable to store the original interrupt handler
void interrupt (*mouse_original_handler)(void);

// Step 2:  define a gloabal flag that our inrerrupt handler can change
volatile int mouse_event_flag = 0;

// Step 3: write the new interrupt handler
void interrupt my_mouse_interrupt_handler(void)
{
	// Firstly, chain to the original handler
	mouse_original_handler();

	// custom task - change flag value to indicate that a mouse event has occurred
	mouse_event_flag = 1; 
}

int main()
{ 
	// Phase 1: install the new interrupt handler
	mouse_original_handler = getvect(interrupt_number);

	printf("Statring mouse interrupt handler demo program\n");

	// set the interrupt vector to point to the new mouse handler
	setvect(interrupt_number, my_mouse_interrupt_handler);

	// Phase 2: do the main work of the program

	// busy-wait loop , waiting for mouse event
	while (1)
	{
		if (mouse_event_flag)
		{
			// reset the flag immediately to allow for the next event
			mouse_event_flag = 0;

			// Call for software interrupt to get mouse status
			inregs.x.ax = 3; // Function 3 - get mouse position and button status
			int86(MOUSE_SOFTWARE_INTERRUPT, &inregs, &outregs);

			// Get the button satus, x & y coordinates from the output registers
			button_status = outregs.x.bx;
			mouse_x = outregs.x.cx;
			mouse_y = outregs.x.dx;

			switch (button_status)
			{
			case NO_MOUSE_BUTTON:
				print_mouse_status(button_status, mouse_x, mouse_y);
				break;
			case LEFT_MOUSE_BUTTON:
				printf("Left mouse button pressed\n");
				print_mouse_status(button_status, mouse_x, mouse_y);
				break;
			case RIGHT_MOUSE_BUTTON:
				printf("Right mouse button pressed\n");
				print_mouse_status(button_status, mouse_x, mouse_y);
				break;
			case BOTH_MOUSE_BUTTONS:
				printf("Both mouse buttons pressed\n");
				print_mouse_status(button_status, mouse_x, mouse_y);
				break;
			}

			if (button_status == BOTH_MOUSE_BUTTONS)
			{
				break;
			}
		}	
	}

	printf("Terminating...\n");

	// Phase 3: restore the original interrupt handler
	setvect(interrupt_number, mouse_original_handler);

	return 0;
}
