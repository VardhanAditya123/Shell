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
#include <algorithm>

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
  
  _append = false;
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
string s1 = *(_simpleCommandsArray[0]->_argumentsArray[0]);
string s2 = "exit";
if(s1.compare(s2) == 0){
  cout << "Good bye!!" << endl;
  exit(0);
}
  // Print contents of Command data structure
  // print();


int ret;

//save in/out
int tmpin=dup(0);
int tmpout=dup(1);
int tmperr=dup(2);


//set the initial input
int fdin;
int fdout;
int fderr;

if (_inFileName) {
fdin = open(_inFileName->c_str(), O_RDONLY,0666);
if(fdin == -1){
  exit(1);
}
}
else {
// Use default input
fdin=dup(tmpin);
}

if(_errFileName){
if(_append)
fderr=open(_errFileName->c_str() ,O_APPEND | O_CREAT |O_RDWR ,0666);
else
fderr=open(_errFileName->c_str() ,O_RDWR | O_CREAT | O_TRUNC,0666);
}
else {
// Use default output
fderr=dup(tmperr);
}
dup2(fderr,2);
close(fderr);



unsigned int count = 0;
for ( auto & simpleCommand : _simpleCommandsArray ) {


dup2(fdin, 0);
close(fdin);
if(simpleCommand->ambig_count > 1){
  cout<< "Ambiguous output redirect." <<endl;
  exit(1);
}
//setup output

if (count == _simpleCommandsArray.size()-1){
// Last simple command
if(_outFileName){
if(_append){
fdout=open(_outFileName->c_str() ,O_APPEND | O_CREAT |O_RDWR ,0666);
}
else
fdout=open(_outFileName->c_str() ,O_RDWR | O_CREAT | O_TRUNC,0666);
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


string s = *(simpleCommand->_argumentsArray[0]);
char *a = &(s[0]);
std::string chars = "\\";


char **final  = new char*[100];
int c =0;
for(auto & word : simpleCommand->_argumentsArray){
  string temp = word->c_str();
  for(char c1: chars){
  temp.erase(std::remove(temp.begin(),temp.end(),c1),temp.end());
  }
  temp = temp.c_str();
  // cout << temp <<" " ;
  // final[c]=const_cast<char*>((word->c_str()));
  final[c] = (char*)temp;
  
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
waitpid(ret ,NULL, 0);
}
 
 //restore in/out defaults
count += 1;
} // for

dup2(tmpin,0);
dup2(tmpout,1);
close(tmpin);
close(tmpout);

if (_backgnd) {
// Wait for last command
_backgnd = true;
waitpid(ret, NULL,0 );
}

clear();
Shell::prompt();
} 

SimpleCommand * Command::_currSimpleCommand;
