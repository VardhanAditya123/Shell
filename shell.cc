#include <cstdio>
#include <unistd.h>
#include "shell.hh"

int yyparse(void);

void Shell::prompt() {
  if ( isatty(0) ) {
  printf("myshell>");
}

  fflush(stdout);
}

int main(int argc, char** argv) {
  Shell::prompt();
  Shell::pth = argv[0];
  yyparse();
}

Command Shell::_currentCommand;
