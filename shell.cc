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

char* t;
int main(int argc, char **argv) {
  t = *argv;
  // cout << *t << endl;
  Shell::prompt();
  yyparse();
}
char*  Shell::arg=t;
Command Shell::_currentCommand;
