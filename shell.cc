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

void sigintHandler(int sig_num) 
{ 
    /* Reset handler to catch SIGINT next time. 
       Refer http://en.cppreference.com/w/c/program/signal */
    signal(SIGCHLD, sigintHandler); 
    printf("\n Cannot be terminated using Ctrl+C \n"); 
    fflush(stdout); 
}

char*  Shell::arg;

int main(int argc, char **argv) {

  
  signal(SIGCHLD, sigintHandler); 
  
  Shell::arg = argv[0];
  Shell::prompt();
  yyparse();
}


Command Shell::_currentCommand;
