#include <ctype.h>      // Check if character is a control character
#include <stdio.h>      // Print functions
#include <stdlib.h>     // Exit functions
#include <termios.h>    // Terminal settings
#include <unistd.h>     // Read from keyboard

// Save original terminal settings
struct termios orig_termios;

// Put terminal back to normal when program ends
void disableRawMode() {
  tcsetattr(STDIN_FILENO, TCSAFLUSH, &orig_termios);
}

// Make terminal read keys immediately (no Enter needed)
void enableRawMode() {
  // Save current terminal settings
  tcgetattr(STDIN_FILENO, &orig_termios);

  // Make sure terminal gets restored when program exits
  atexit(disableRawMode);

  // Copy settings to modify
  struct termios raw = orig_termios;

  // Turn off:
  // ECHO - don't show what you type
  // ICANON - don't wait for Enter key
  // IEXTEN - disable Ctrl-V
  // ISIG - disable Ctrl-C and Ctrl-Z
  raw.c_lflag &= ~(ECHO | ICANON | IEXTEN | ISIG);
  
  // Turn off input processing:
  // BRKINT - don't send SIGINT on break
  // ICRNL - don't convert carriage return to newline
  // INPCK - disable input parity checking
  // ISTRIP - don't strip 8th bit of characters
  // IXON - disable Ctrl-S and Ctrl-Q flow control
  raw.c_iflag &= ~(BRKINT | ICRNL | INPCK | ISTRIP | IXON);

  // Turn off output processing (no automatic \n to \r\n conversion)
  raw.c_oflag &= ~(OPOST);
  
  // Set character size to 8 bits per byte
  raw.c_cflag |= (CS8);

  // Apply new settings
  tcsetattr(STDIN_FILENO, TCSAFLUSH, &raw);
}

int main() {
  enableRawMode();  

  char c;

  // Read each key press and show its number until 'q' is pressed
  while (read(STDIN_FILENO, &c, 1) == 1 && c != 'q') {
    if (iscntrl(c)) {
      printf("%d\r\n", c);                    
    } else {
      printf("%d ('%c')\r\n", c, c);          
    }
  }

  return 0;
}