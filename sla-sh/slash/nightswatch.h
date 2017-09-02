#include "kbhit.h"
#include <stdio.h>
#include <string.h>
#include <time.h>
#include <unistd.h>

int cal_time(char *input_time) {
  int length = strlen(input_time);
  int i;
  int val = 0;

  for (i = 0; i < length; i++) {
    val = val * 10 + (input_time[i] - '0');
  }

  return val;
}

void slash_nightswatch(char **args) {
  time_t time_lapsed;
  char *options = args[1];
  char *command = args[3];

  if (options[1] != 'n') {
    return;
  }

  double wait_time = (double)cal_time(args[2]);

  time(&time_lapsed);

  do {
    char buffer[1001];
    char temp;
    int i;

    if (difftime(time(0), time_lapsed) >= wait_time) {
      if (!strcmp(command, "interrupt")) {
        FILE *interrupt_file = fopen("/proc/interrupts", "r");
        fscanf(interrupt_file, "%[^'\n']%c", buffer, &temp);
        printf("%s\n", buffer);
        fscanf(interrupt_file, "%[^'\n']%c", buffer, &temp);
        fscanf(interrupt_file, "%[^'\n']%c", buffer, &temp);
        printf("%s\n", buffer);
        fclose(interrupt_file);
      } else if (!strcmp(command, "dirty")) {
        FILE *dirty_file = fopen("/proc/meminfo", "r");
        while (1) {
          fscanf(dirty_file, "%[^'\n']%c", buffer, &temp);
          char string[6];
          string[5] = '\n';
          for (i = 0; i < 5; i++) {
            string[i] = buffer[i];
          }
          if (!strcmp(string, "Dirty\n")) {
            printf("%s\n", buffer);
            break;
          }
        }
        fclose(dirty_file);
      }
      time(&time_lapsed);
    }
  } while (kbhit() != 'q');
}
