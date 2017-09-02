#include <dirent.h>
#include <stdio.h>
#include <string.h>
#include <sys/stat.h>
#include <time.h>
#include <unistd.h>

void slash_ls(char **args) {
  int i = 0, flags[2] = {0}; // flags 00=ls, 01=ls -a, 10=ls -l, 11=ls -l -a
  while (args[i] != NULL)
    i++;

  if (i == 3) {
    flags[0] = flags[1] = 1;
  } else if (i == 2) {
    if (strlen(args[1]) == 3) {
      flags[0] = flags[1] = 1;
    } else {
      if (args[1][1] == 'l') {
        flags[0] = 1;
        flags[1] = 0;
      } else if (args[1][1] == 'a') {
        flags[0] = 0;
        flags[1] = 1;
      }
    }
  }

  char cwd[100];
  getcwd(cwd, 100);
  DIR *currDir;
  struct dirent *currFile;
  currDir = opendir(cwd);
  struct stat fileDetails = {0};

  if (flags[0] == 0 && flags[1] == 0) // only ls
  {
    if (currDir != NULL) {
      while (currFile = readdir(currDir)) {
        if (currFile->d_name[0] != 46) {
          printf("%s\n", currFile->d_name);
        }
      }
    }
  }

  else if (flags[0] == 0 && flags[1] == 1) // only ls -a
  {
    if (currDir != NULL) {
      while (currFile = readdir(currDir)) {
        printf("%s\n", currFile->d_name);
      }
    }
  }

  else if (flags[0] == 1) // only ls -l
  {
    printf("Permissions      Size Date        Name\n");
    printf("---------------------------------------\n");
    if (currDir != NULL) {
      while (currFile = readdir(currDir)) {
        getcwd(cwd, 100);
        strcat(cwd, "/");
        strcat(cwd, currFile->d_name);
        stat(cwd, &fileDetails);
        int permissions = fileDetails.st_mode;
        int i;
        char perm[10];
        char yes[10] = {"rwxrwxrwx"};
        for (i = 0; i < 9; i++) {
          if ((permissions >> (9 - i - 1)) & 1) {
            perm[i + 1] = yes[i];
          } else {
            perm[i + 1] = '-';
          }
        }
        if (permissions >> 15) {
          perm[0] = '-';
        } else {
          perm[0] = 'd';
        }
        i = 10;
        struct tm *time;
        char date[100];
        time = localtime(&fileDetails.st_mtime);
        strftime(date, 100, "%m %d %H:%M", time);
        if (!(flags[1] == 0 && currFile->d_name[0] == 46)) {
          printf("%s ", perm);
          printf("%*lld ", i, fileDetails.st_size);
          printf("%s ", date);
          printf("%s\n", currFile->d_name);
        }
      }
    }
  }
}
