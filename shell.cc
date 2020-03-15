#include <cstdio>
#include <unistd.h>
#include "shell.hh"
#include <cstdlib>
#include <iostream>
using namespace std;
int yyparse(void);

void Shell::prompt() {
  if ( isatty(0) ) {
  printf("myshell>");
}

  fflush(stdout);
}

int main(int argc, char **argv) {
  cout << argv[0] << endl;
  Shell::prompt();
  yyparse();
}

Command Shell::_currentCommand;
