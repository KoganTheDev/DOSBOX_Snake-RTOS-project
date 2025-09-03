#include <stdio.h>
#include <dos.h>
#include <ctype.h>

#define MAX_PASS_SIZE 16
#define KEYBOARD_HARDWARE_INTERRUPT 0x09
#define KEYBOARD_DATA_PORT 0x60

// This enum defines the "make code" for specific keys.
// A make code is a unique value sent by the keyboard when a key is pressed.
typedef enum make_scan_codes
{
    SCAN_LSHIFT = 0x2A,
    SCAN_RSHIFT = 0x36,
    SCAN_CTRL = 0x1D,
    SCAN_ALT = 0x38,
    SCAN_ENTER = 0x1C,
    BREAK_CODE = 0x80, // Indicates a key release
};

typedef enum flags_for_make_keys
{
    FLAG_LSHIFT = 0x01,
    FLAG_RSHIFT = 0x02,
    FLAG_CTRL = 0x04,
    FLAG_ALT = 0x08,

// --- Global variables for the password system ---
volatile int password_index = 0; // Tracks the current number of keys captured.
volatile unsigned char password[MAX_PASS_SIZE]; // Stores the ASCII characters of the keys pressed.
volatile int password_combos[MAX_PASS_SIZE]; // Stores the bit flags of the modifier keys for each key press.
volatile unsigned char modifier_keys = 0; // A single byte used to hold the current state of modifier keys.
volatile void interrupt(*old_keyboard_handler)(void); // Pointer to the original handler

// Look up taable for common keys
unsigned char scan_to_ascii_lookup[256] = {
    0, 0, '1', '2', '3', '4', '5', '6', '7', '8', '9', '0', '-', '=', 0, 0,
    'q', 'w', 'e', 'r', 't', 'y', 'u', 'i', 'o', 'p', '[', ']', 0, 0, 'a', 's',
    'd', 'f', 'g', 'h', 'j', 'k', 'l', ';', '\'', '`', 0, '\\', 'z', 'x', 'c',
    'v', 'b', 'n', 'm', ',', '.', '/', 0, 0, 0, ' ', 0, 0, 0, 0, 0,
};


// The custom interrupt handler for keyboard events.
// It intercepts keyboard events.
void interrupt new_keyboard_handler(void)
{
    unsigned char scan_code;
    char key_char;

    // Read the scan code from the keyboard data port (60h)
    scan_code = inportb(KEYBOARD_DATA_PORT);

    // Check if this is a key release (break code)
    if (scan_code & BREAK_CODE)
    {
        scan_code &= 0x7F; // Remove the break code bit to get the original scan code
        switch (scan_code)
        {
        case SCAN_LSHIFT:
            modifier_keys &= ~FLAG_LSHIFT; // Clear Left Shift bit
            break;
        case SCAN_RSHIFT:
            modifier_keys &= ~FLAG_RSHIFT; // Clear Right Shift bit
            break;
        case SCAN_CTRL:
            modifier_keys &= ~FLAG_CTRL; // Clear the CTRL bit
            break;
        case SCAN_ALT:
            modifier_keys &= ~FLAG_ALT; // Clear the ALT bit
            break;
        }
    }
    // It's a key press (make code)
    else
    {
        // Ignore enter so it won`t be added into the password
        if (scan_code == SCAN_ENTER)
        {
            // Do nothing, let scanf handle this
        }
        else 
        {
            switch (scan_code)
            {
            case SCAN_LSHIFT:
                modifier_keys |= FLAG_LSHIFT; // Set the Left Shift bit
                break;
            case SCAN_RSHIFT:
                modifier_keys |= FLAG_RSHIFT; // Set the Right Shift bit
                break;
            case SCAN_CTRL:
                modifier_keys |= FLAG_CTRL; // Set the CTRL bit
                break;
            case SCAN_ALT:
                modifier_keys |= FLAG_ALT; // Set the ALT bit
                break;
            default:
                // If it's a regular key, store the combo and key
                if (password_index < MAX_PASS_SIZE)
                {
                    key_char = scan_to_ascii_lookup[scan_code];

                    // Check for Shift key and convert to uppercase if necessary
                    if ((modifier_keys & (FLAG_LSHIFT | FLAG_RSHIFT)) && isalpha(key_char))
                    {
                        key_char = toupper(key_char);
                    }

                    password_combos[password_index] = modifier_keys;
                    password[password_index] = key_char;
                    password_index++;
                }
                break;
            }
        }
    }

    // Acknowledge the interrupt to the PIC
    outportb(0x20, 0x20);

    // Call the original handler to allow the system to process the key normally
    old_keyboard_handler();
}


// This function prints the names of the modifier keys that were held down
void print_key_combo(int index)
{
    // Check the bit flags to print the modifier keys
    if (password_combos[index] & FLAG_ALT)
    {
        printf("ALT + ");
    }
    if (password_combos[index] & FLAG_CTRL)
    {
        printf("CNRT + ");
    }
    if (password_combos[index] & FLAG_LSHIFT)
    {
        printf("LEFTSHIFT + ");
    }
    if (password_combos[index] & FLAG_RSHIFT)
    {
        printf("RIGHTSHIFT + ");
    }
}

// This function is responsible for setting up the new keyboard handler
void setup_password_system()
{
    // Save the original interrupt vector for the keyboard handler
    old_keyboard_handler = getvect(KEYBOARD_HARDWARE_INTERRUPT);

    // Set our custom handler for Interrupt 9 which is the keyboard hardware interrupt = 0x09
    setvect(KEYBOARD_HARDWARE_INTERRUPT, new_keyboard_handler);
}

// This function is responsible to restore the old keyboard handler
void unset_password_system()
{
    // Restore the original handler
    setvect(KEYBOARD_HARDWARE_INTERRUPT, old_keyboard_handler);
}


int main()
{
    char text_password[16];
    unsigned char prev_password[MAX_PASS_SIZE];
    int prev_password_combos[MAX_PASS_SIZE];
    int i, prev_password_index, accept_flag;

    printf("Enter password signature: ");

    password_index = 0;

    setup_password_system(); // Install the new keyboard handler
    scanf("%s", text_password);
    unset_password_system(); //  restore the old keyboard handler

    for (i = 0; i < password_index; i++)
    {
        print_key_combo(i);
        printf("text letter = %c, key letter = %c\n",
            text_password[i], password[i]);
    } // for

    prev_password_index = password_index;
    password_index = 0;

    for (i = 0; i < prev_password_index; i++)
    {
        prev_password[i] = password[i];
        prev_password_combos[i] = password_combos[i];
    } // for



    for (i = 0; i < password_index; i++)
    {
        print_key_combo(i);
        printf("text letter = %c, key letter = %c\n",
            text_password[i], password[i]);
    } // for

    fflush(stdin);

    password_index = 0;
    printf("Enter password signature: ");
    setup_password_system();
    scanf("%s", text_password);
    unset_password_system();


    accept_flag = 1;
    i = 0;
    if (password_index != prev_password_index)
        accept_flag = 0;
    else
    {
        while ((i < password_index) && (accept_flag == 1))
        {
            if ((password[i] != prev_password[i]) ||
                (password_combos[i] != prev_password_combos[i]))
                accept_flag = 0;
            i++;
        } // while
    } // else

    if (accept_flag == 1)
        printf("password accepted\n");
    else
    {
        printf("password rejected\n");
        printf("rejection index = %d\n", i);
    } // else

    return 0;
} // main
