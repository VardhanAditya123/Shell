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
 void zombie(int sig) {
	int pid = wait(0, 0, NULL);
	while (waitpid(-1, NULL, WNOHANG) > 0);
}

int main(int argc, char **argv) {
 
  struct sigaction s1;
  s1.sa_handler = zombie;
  Shell::arg = argv[0];
  Shell::prompt();
  yyparse();
}


Command Shell::_currentCommand;
