#include "cd.h"
#include "cwd.h"
#include "echo.h"
#include "ls.h"
#include "nightswatch.h"
#include "pinfo.h"
#include <pwd.h>
#include <sys/wait.h>

void slash_exec(char **args);
void print_prompt();
void read_command();
void split_command(char *str);
void slash_send(char *args);
void slash_execute(char **args);

void slash_exec(char **args) {
  // pid_t parent;
  pid_t pid1;
  pid_t pid2;
  int len = strlen(args[0]);
  pid1 = fork();
  if (args[0][len - 1] != '&') {
    if (pid1 == 0) // child
    {
      if (execvp(args[0], args) == -1) {
        printf("%s: Command not found\n", args[0]);
      }
    } else if (pid1 < 0) {
      printf("Error in forking\n");
    } else // parent
    {
      wait(NULL);
      // exit(0);
    }
  } else {
    if (pid1 == 0) {
      pid2 = fork();
      if (pid2 == 0) // grandchild
      {
        pid_t child = getpid();
        args[0][len - 1] = 0;
        // setpgid(0, 0);
        printf("%d STARTED\n", child);
        execvp(args[0], args);
      } else if (pid2 != 0) // child
      {
        wait(NULL);
        printf("%d DONE\n", pid2);
      }
    }
  }
}

void print_prompt() {
  char *username;
  username = getpwuid(geteuid())->pw_name;

  char hostname[1000];
  gethostname(hostname, 1000);

  char cwd[100];
  getcwd(cwd, 100);

  int len = strlen(mainFilePath);
  char e[100];

  int i, counter = 0;
  for (i = 0; i < len; i++) {
    if (cwd[i] == mainFilePath[i]) {
      counter++;
    }
  }
  if (counter == len) {
    e[0] = 126;
    for (i = 1; i <= strlen(cwd); i++) {
      e[i] = cwd[i + len - 1];
    }
    printf("\033[1;32m<%s@%s:\033[1;34m%s\033[1;32m>\033[0m$ ", username,
           hostname, e);
  } else {
    printf("\033[1;32m<%s@%s:\033[1;34m%s\033[1;32m>\033[0m$ ", username,
           hostname, cwd);
  }
}

void read_command() {
  char input[100];
  int i;
  for (i = 0; i < 100; i++) {
    input[i] = 0;
  }
  if (fgets(input, 100, stdin)) {
    int len = strlen(input);
    input[len - 1] = 0; // replaces the last "\n" with a "\0"
    split_command(input);
  } else {
    //
    printf("\n");
  }
}

void split_command(char *str) {
  char *p;
  char commands[100][100];
  int i = 0;
  int j = 0;
  for (i = 0; i < 100; i++)
    for (j = 0; j < 100; j++)
      commands[i][j] = 0;
  i = 0;

  for (p = strtok(str, ";"); p != NULL; p = strtok(NULL, ";"), i++) {
    int j = 0;
    int len = strlen(p);
    for (j = 0; j < len; j++) {
      commands[i][j] = p[j];
    }
  }
  int noOfCommands = i;
  for (j = 0; j < noOfCommands; j++) {
    slash_send(commands[j]);
  }
}

void slash_send(char *args) {
  char *arr[100];
  char *p = NULL;
  int i = 0;
  for (i = 0; i < 100; i++)
    arr[i] = NULL;

  i = 0;

  // printf("1\n");
  // printf("%s\n", args);
  for (p = strtok(args, " "); p != NULL; p = strtok(NULL, " "), i++) {
    arr[i] = p;
  }
  // printf("%s %s\n", arr[0], arr[1]);
  slash_execute(arr);
}

void slash_execute(char **args) {
  if (!strcmp(args[0], "cd")) {
    slash_cd(args);
  } else if (!strcmp(args[0], "pwd")) {
    slash_pwd(args);
  } else if (!strcmp(args[0], "echo")) {
    int i;
    for (i = 0; args[i] != NULL; i++)
      ;
    slash_echo(args, i);
  } else if (!strcmp(args[0], "ls")) {
    slash_ls(args);
  } else if (!strcmp(args[0], "pinfo")) {
    slash_pinfo(args);
  } else if (!strcmp(args[0], "nightswatch")) {
    slash_nightswatch(args);
  } else {
    slash_exec(args);
  }
}
