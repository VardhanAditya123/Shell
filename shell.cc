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
int main(int argc, char **argv) {
  t = *argv;
  Shell::arg = t;
  Shell::prompt();
  yyparse();
}


Command Shell::_currentCommand;
