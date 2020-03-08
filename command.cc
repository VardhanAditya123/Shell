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
void myunputc(int );
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

char* esc(char* str) {
  char* dst = (char*)malloc(strlen(str)+1);
  int c = 0;
  for(unsigned int i = 0 ; i < strlen(str);i++ ){
    if(*(str+i)=='\\' ){
      *(dst + c) = *(str+i+1);
      c+=1;
      i+=1;
    }
    else{
      *(dst + c) = *(str+i);
      c+=1;
    }
  }
  return dst;
}

int Command::commandCheck(){

  string s = *(_simpleCommandsArray[0]->_argumentsArray[0]);
  string s2 = "exit";




  if(s.compare(s2) == 0){
    cout << "Good bye!!" << endl;
    exit(0);
  }

  if(s.compare("setenv") == 0){

    setenv(const_cast<char*>((_simpleCommandsArray[0]->_argumentsArray[1])->c_str()),
        const_cast<char*>((_simpleCommandsArray[0]->_argumentsArray[2])->c_str()),1);
    return 1 ;
  }

  if(s.compare("unsetenv") == 0){

    unsetenv(const_cast<char*>((_simpleCommandsArray[0]->_argumentsArray[1])->c_str()));
    return 1 ;
  }

  if(s.compare("cd") == 0){
    // print();
    string str = (_simpleCommandsArray[0]->_argumentsArray[0])->c_str();

    if((_simpleCommandsArray[0]->number_args == 1))
      chdir(getenv("HOME"));

    else if((_simpleCommandsArray[0]->number_args == 2) && !_errFileName )
      chdir(const_cast<char*>((_simpleCommandsArray[0]->_argumentsArray[1])->c_str()));


    else{
      // cout << "FUCK" << endl;
      int tmperr=dup(2);
      int fderr;
      fderr=open(_errFileName->c_str() , O_CREAT |O_WRONLY|O_TRUNC ,0666);
      int ret = chdir(const_cast<char*>((_simpleCommandsArray[0]->_argumentsArray[1])->c_str()));
      if ( ret == -1){
        dup2(fderr,2);
        close(fderr);
        fprintf(stderr,"cd: can't cd to %s\n",(_simpleCommandsArray[0]->_argumentsArray[1])->c_str());
        dup2(tmperr,2);
        close(tmperr);
      }

      return 1;
    }

    return 1;
  }

  return 0;
}


int Command::subShell(){

  string s = *(_simpleCommandsArray[0]->_argumentsArray[0]);
  int flag = 0;
  for ( auto & simpleCommand : _simpleCommandsArray ) {
    for(auto & word : simpleCommand->_argumentsArray){
      if(word->at(0) =='$')
      flag = 1;
    }
  }
  cout << s << endl;

  if(flag==1){
    s.replace(0,2,"");
    s.pop_back();

    int in[2];
    int out[2];
    int tmpin=dup(0);
    int tmpout=dup(1);
    pipe(in); 
    pipe(out);
    int fd0 = 0;
    int fd1 = 1;
    dup2(fd0,0);
    dup2(fd1,1);
    int ret = fork();
    if(ret == 0){
      dup2(fd0,in[0]);
      dup2(fd1,out[1]);
      execvp("/proc/self/exe",NULL);
    }
    else if(ret > 0){
      dup2(fd1,in[1]);
      dup2(fd0,out[0]);
      
      char str[1000];
      strcpy(str,s.c_str()); 
      cout << s << endl;
      write(out[1],str,1000);
      wait(NULL);
      read(out[0],str,1000);
      s.append("\nexit\n");
      for(int i = s.length() -1  ; i >= 0;i--){
      myunputc(s.at(i));
      }

    }
    dup2(tmpin,0);
    dup2(tmpout,1);
    close(tmpin);
    close(tmpout);

    return 1;
  }

  return 0;
}



void Command::execute() {
  // Don't do anything if there are no simple commands
  if ( _simpleCommandsArray.size() == 0 ) { 
    Shell::prompt();
    return;
  }
 

  int check_fun = Command::commandCheck();
  if(check_fun == 1){
    clear();
    Shell::prompt();
    return;
  }
  int ret;
  int check_sub = subShell();
  // print();
  if(check_sub == 1){
    clear();
    Shell::prompt();
    return;
  }


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

    char **final  = new char*[100];
    int c =0;
    for(auto & word : simpleCommand->_argumentsArray){
      char*str = esc((char*)word->c_str());
      //  cout << str << endl;
      final[c]=const_cast<char*>(str);
      c=c+1;
    }



    ret = fork();

    if (ret == 0) {
      if(s.compare("printenv") == 0){
        for(int i = 0 ;environ[i]!=NULL; i++){

          cout << *(environ+i)<<endl;  
        }
        exit(1);
      }
      else{
        execvp(a, final);
        perror("execvp");
        _exit(1); 
      }

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
