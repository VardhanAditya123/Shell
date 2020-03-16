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


char*  Shell::arg;
char*  Shell::arg2;
int main(int argc, char **argv) {
  Shell::arg = argv[0];
  Shell::arg2 = argv[argc-1];
  Shell::prompt();
  yyparse();
}


Command Shell::_currentCommand;
