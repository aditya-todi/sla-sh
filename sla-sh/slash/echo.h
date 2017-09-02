#include <stdio.h>

int slash_echo(char **args, int noOfArgs) {
  int i;
  for (i = 1; i < noOfArgs; i++) {
    printf("%s ", args[i]);
  }
  printf("\n");
}
