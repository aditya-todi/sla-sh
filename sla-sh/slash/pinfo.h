#include <fcntl.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>

void slash_pinfo(char **args) {
  int i = 0;
  while (args[i] != NULL)
    i++;

  if (i == 2) {
    char statPath[100] = {"/proc/"};
    strcat(statPath, args[1]);
    strcat(statPath, "/stat");
    int fd_file1;
    fd_file1 = open(statPath, O_RDONLY);
    if (fd_file1 > 0) {
      char buf[1000];
      read(fd_file1, buf, 1000);
      char *arr[100];
      int i = 0;
      for (i = 0; i < 100; i++)
        arr[i] = NULL;

      i = 0;
      char *p;
      for (p = strtok(buf, " "); p != NULL; p = strtok(NULL, " "), i++) {
        arr[i] = p;
      }

      char procPath[100];
      char statPath2[100] = {"/proc/"};
      strcat(statPath2, args[1]);
      strcat(statPath2, "/exe");
      int len = readlink(statPath2, procPath, 100);
      procPath[len] = 0;

      printf("PID: %s\nProcess Status: %s\nVirtual Memory: %s\nExecutable File "
             "Path: %s\n",
             arr[0], arr[2], arr[25], procPath);
    } else {
      printf("Error in finding process, make sure this pid exists\n");
    }
  } else {
    int pid = getpid();
    char *argu[2];
    argu[0] = args[0];
    argu[1] = (char *)malloc(1000);
    sprintf(argu[1], "%d", pid);
    slash_pinfo(argu);
  }
}
