#include <cstdio>
#include <unistd.h>
#include "shell.hh"
#include <cstdlib>
#include <iostream>
#include <string.h>
using namespace std;
int yyparse(void);

void Shell::prompt() {
  if ( isatty(0) ) {
  printf("myshell>");
}

  fflush(stdout);
}

int main(int argc, char **argv) {
  
  Shell::prompt();
  yyparse();
}
char**  Shell::arg;
Command Shell::_currentCommand;
