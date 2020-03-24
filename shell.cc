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
void zombie(int);

void Shell::prompt() {
  if ( isatty(0) ) {
  printf("myshell>");
}

  fflush(stdout);
}

void zombie(int sig) {
	int pid = wait3(0, 0, NULL);
	while (waitpid(-1, NULL, WNOHANG) > 0);
}

char*  Shell::arg;

int main(int argc, char **argv) {

   struct sigaction sigA2;
   sigA2.sa_handler = zombie;
  Shell::arg = argv[0];
  Shell::prompt();
  yyparse();
}


Command Shell::_currentCommand;
