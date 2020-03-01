#include <cstdio>

#include "shell.hh"

int yyparse(void);

void Shell::prompt() {
  if ( isatty(0) ) {
  Print prompt
}

  printf("myshell>");
  fflush(stdout);
}

int main() {
  Shell::prompt();
  yyparse();
}

Command Shell::_currentCommand;
