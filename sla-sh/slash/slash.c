#include "execute.h"

void slashloop() {
  int isError = 0;
  while (!isError) {
    print_prompt();
    read_command();
  }
}

int main() {
  getcwd(mainFilePath, 100);
  slashloop();
  return 0;
}
