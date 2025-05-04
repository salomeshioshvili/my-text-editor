#include <ctype.h>      // iscntrl() function to check for control characters
#include <stdio.h>      // printf() function
#include <stdlib.h>     // atexit()
#include <termios.h>    // terminal control
#include <unistd.h>     // read() and STDIN_FILENO

// Stores the original terminal settings
struct termios orig_termios;

// Restore the original terminal mode when the program exits
void disableRawMode() {
  tcsetattr(STDIN_FILENO, TCSAFLUSH, &orig_termios);
}

// Set terminal to raw mode (turn off echo + canonical mode)
void enableRawMode() {
  // Get and save current terminal attributes
  tcgetattr(STDIN_FILENO, &orig_termios);

  // Register disableRawMode to run at exit
  atexit(disableRawMode);

  // Make a copy of the original settings to modify
  struct termios raw = orig_termios;

  // Turn off ECHO (no automatic input display)
  // Turn off ICANON (input is available immediately, not after Enter)
  raw.c_lflag &= ~(ECHO | ICANON);

  // Apply the modified settings
  tcsetattr(STDIN_FILENO, TCSAFLUSH, &raw);
}

int main() {
  enableRawMode();  

  char c;

  // Continuously read and print ASCII codes for each keypress (and characters if printable) until 'q' is pressed
  while (read(STDIN_FILENO, &c, 1) == 1 && c != 'q') {
    if (iscntrl(c)) {
      printf("%d\n", c);
    } else {
      printf("%d ('%c')\n", c, c);
    }
  }

  return 0;
}
