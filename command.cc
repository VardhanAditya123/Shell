/*
 * CS252: Shell project
 *
 * Template file.
 * You will need to add more code here to execute the command table.
 *
 * NOTE: You are responsible for fixing any bugs this code may have!
 *
 * DO NOT PUT THIS PROJECT IN A PUBLIC REPOSITORY LIKE GIT. IF YOU WANT 
 * TO MAKE IT PUBLICALLY AVAILABLE YOU NEED TO REMOVE ANY SKELETON CODE 
 * AND REWRITE YOUR PROJECT SO IT IMPLEMENTS FUNCTIONALITY DIFFERENT THAN
 * WHAT IS SPECIFIED IN THE HANDOUT. WE OFTEN REUSE PART OF THE PROJECTS FROM  
 * SEMESTER TO SEMESTER AND PUTTING YOUR CODE IN A PUBLIC REPOSITORY
 * MAY FACILITATE ACADEMIC DISHONESTY.
 */

#include <cstdio>
#include <cstdlib>
#include <iostream>
#include "command.hh"
#include "shell.hh"
#include <unistd.h>
#include <sys/types.h>
#include <sys/wait.h>
#include <iostream>
#include <string.h>
#include <assert.h>
#include <stdlib.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
using namespace std;

Command::Command() {
  // Initialize a new vector of Simple Commands
  _simpleCommandsArray = std::vector<SimpleCommand *>();
  _outFileName = NULL;
  _inFileName = NULL;
  _errFileName = NULL;
  _backgnd = false;
}

void Command::insertSimpleCommand( SimpleCommand * simpleCommand ) {
  // add the simple command to the vector
  _simpleCommandsArray.push_back(simpleCommand);
}

void Command::clear() {
  // deallocate all the simple commands in the command vector
  for (auto simpleCommand : _simpleCommandsArray) {
    delete simpleCommand;
  }   
  // remove all references to the simple commands we've deallocated
  // (basically just sets the size to 0)
  _simpleCommandsArray.clear();

  if ( _outFileName==NULL ) {
    delete _outFileName;
  }
  _outFileName = NULL;

  if ( _inFileName==NULL ) {
    delete _inFileName;
  }
  _inFileName = NULL;

  if ( _errFileName==NULL ) {
    delete _errFileName;
  }
  _errFileName = NULL;

  _backgnd = false;
}

void Command::print() {
  printf("\n\n");
  printf("              COMMAND TABLE                \n");
  printf("\n");
  printf("  #   Simple Commands\n");
  printf("  --- ----------------------------------------------------------\n");

  int i = 0;
  // iterate over the simple commands and print them nicely
  for ( auto & simpleCommand : _simpleCommandsArray ) {
    printf("  %-3d ", i++ );
    simpleCommand->print();
  }

  printf( "\n\n" );
  printf( "  Output       Input        Error        Background\n" );
  printf( "  ------------ ------------ ------------ ------------\n" );
  printf( "  %-12s %-12s %-12s %-12s\n",
      _outFileName?_outFileName->c_str():"default",
      _inFileName?_inFileName->c_str():"default",
      _errFileName?_errFileName->c_str():"default",
      _backgnd?"YES":"NO");
  printf( "\n\n" );
}


void Command::execute() {
  // Don't do anything if there are no simple commands
  if ( _simpleCommandsArray.size() == 0 ) { 
    Shell::prompt();
    return;
  }


  // Print contents of Command data structure
  print();

string s = *(_simpleCommandsArray[0]->_argumentsArray[0]);
char *a = &(s[0]);
int ret;

//save in/out
int tmpin=dup(0);
int tmpout=dup(1);
//set the initial input
int fdin;
if (_inFileName) {
fdin = open(_inFileName, O_READ);
}
else {
// Use default input
fdin=dup(tmpin);
}
int fdout;


unsigned int count = 0;
for ( auto & simpleCommand : _simpleCommandsArray ) {


dup2(fdin, 0);
close(fdin);
//setup output
if (count == _simpleCommandsArray.size()-1){
// Last simple command
if(_outFileName){
fdout=open(_outFileName, ,O_WRITE);
}
else {
// Use default output
fdout=dup(tmpout);
}
}

else {
// Not last
//simple command
//create pipe
int fdpipe[2];
pipe(fdpipe);
fdout=fdpipe[1];
fdin=fdpipe[0];
}// if/else

// Redirect output
dup2(fdout,1);
close(fdout);




char **final  = new char*[100];
int c =0;
for(auto & word : simpleCommand->_argumentsArray){
  final[c]=const_cast<char*>((word->c_str()));
  c=c+1;
}



ret = fork();
if (ret == 0) {
//child
execvp(a, final);
perror("execvp");
_exit(1);
}

else if (ret < 0) {
perror("fork");
return;
}
// Parent shell continue

else{
// wait for last process
waitpid(-1,&ret, -0);
}
 
 //restore in/out defaults
dup2(tmpin,0);
dup2(tmpout,1);
close(tmpin);
close(tmpout);
count += 1;
}

// for
clear();
Shell::prompt();
} 

SimpleCommand * Command::_currSimpleCommand;
