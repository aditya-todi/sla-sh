#include <fcntl.h>
#include <stdio.h>
#include <termios.h>
#include <unistd.h>

int kbhit(void) {
  struct termios old_settings, new_settings;
  int character;
  int oldf;

  tcgetattr(STDIN_FILENO, &old_settings);
  tcgetattr(STDIN_FILENO, &new_settings);

  new_settings.c_lflag &= ~(ICANON | ECHO);
  tcsetattr(STDIN_FILENO, TCSANOW, &new_settings);

  oldf = fcntl(STDIN_FILENO, F_GETFL, 0);
  fcntl(STDIN_FILENO, F_SETFL, oldf | O_NONBLOCK);

  character = getchar();

  tcsetattr(STDIN_FILENO, TCSANOW, &old_settings);
  fcntl(STDIN_FILENO, F_SETFL, oldf);

  if (character != EOF) {
    return character;
  }

  return 0;
}