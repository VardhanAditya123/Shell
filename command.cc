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

  // Add execution here
  // For every simple command fork a new process
  // Setup i/o redirection
  // and call exec

 // Create a new process


string s = *(_simpleCommandsArray[0]->_argumentsArray[0]);
char *a = &(s[0]);
int ret;
for ( unsigned int i = 0; i < _simpleCommandsArray.size() ;i++ ) {
ret = fork();

vector<char* const* >arr ;
for ( unsigned int j = 0; j <(_simpleCommandsArray[i]->_argumentsArray).size(); j++) 
 arr.push_back((char* const*) (_simpleCommandsArray[i]->_argumentsArray[j])); 
 
 arr.push_back('\0');
char*const* v = (char*const*)(&arr[0]) ;


if (ret == 0) {
//child
execvp(a , v);
perror("execvp");
_exit(1);
}


else if (ret < 0) {
perror("fork");
return;
}

// Parent shell continue
} // for

if (!_backgnd) {
// wait for last process

waitpid(getpid(),&ret, NULL);
}


  // Clear to prepare for next command
  clear();

  // Print new prompt
  Shell::prompt();
}

SimpleCommand * Command::_currSimpleCommand;
