#include <cstdio>
#include <unistd.h>
#include "shell.hh"
#include <cstdlib>
#include <iostream>
#include <string.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <sys/wait.h>
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

  Shell::arg = argv[0];
  Shell::prompt();
  yyparse();
}


Command Shell::_currentCommand;
