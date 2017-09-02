#include <stdio.h>
#include <unistd.h>

char mainFilePath[100];

int slash_cd(char **args) {
  if (args[1] == NULL) {
    chdir(mainFilePath);
  } else {
    if (args[1][0] == 126) {
      chdir(mainFilePath);
    } else if (chdir(args[1]) != 0) {
      printf("Error in going to specified location\n");
    }
  }
  return 1;
}