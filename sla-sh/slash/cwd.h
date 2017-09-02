#include <stdio.h>
#include <unistd.h>

int slash_pwd(char **args) {
  if (args[1]) {
    printf("Too many arguments\n");
  } else {
    char cwd[100];
    if (getcwd(cwd, 100)) {
      printf("%s\n", cwd);
    } else {
      printf("Error in printing present working directory");
    }
  }
}
