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
#include <limits.h>
#include <pwd.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <algorithm>
#include <bits/stdc++.h> 


using namespace std;
int last;
pid_t last_id;
string last_arg;

void myunputc(int );
// int last;
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

    if(*(str+i)=='\\' && *(str+i+1)=='\0' && *(str+i-1)=='\0'  ){
      *(dst + c) = *(str+i);
      c+=1;
      continue;
    }

    if(*(str+i)=='\\'  ){
      *(dst + c) = *(str+i+1);
      c+=1;
      i+=1;
    }
    else{
      *(dst + c) = *(str+i);
      c+=1;
    }
  }
  dst[c] = '\0';
  return dst;
}

char* pecho (char* str){

  string fin ;
  string tmp;
  for(unsigned int i = 0 ; i < strlen(str);i++ ){
    if(str[i]!='$'  ){
      fin += str[i];
    }
    else{
      i=i+2; 
      while(str[i]!='}'){
        tmp += str[i];
        i++;
      }
      fin+=getenv(tmp.c_str());
      tmp="";
    }
  }

  char* tmp2 = strcpy(new char[fin.length()+ 1],fin.c_str());
  return tmp2;
}

int commandCheck(char**final , int c){

  string s = final[0];
  string s2 = "exit";



  if(s.compare("source")==0){

    string s1 = final[1];
    // cout << s1 << endl;
    std::string word;
    fstream file;
    file.open(s1.c_str());

    string str[100];
    int count  = 0;
    while (1) 
    { 
      getline(file,word);

      if(word.length()==0)
        break;

      str[count] = word;
      count=count+1;
    }

    for(int j = count-1; j >= 0;j--){
      string s = str[j];
      myunputc('\n');
      for(int i = s.length() -1  ; i >= 0;i--){
        myunputc(s.at(i));
      }
    }


    return 1;
  }

  if(s.compare(s2) == 0){
   return 2;
  }

  if(s.compare("setenv") == 0){

    setenv(final[1],final[2],1);
    return 1 ;
  }

  if(s.compare("unsetenv") == 0){

    // unsetenv((_simpleCommandsArray[0]->_argumentsArray[1])->c_str());
    unsetenv(final[1]);
    return 1 ;
  }

  if(s.compare("cd") == 0){

    // string str = (_simpleCommandsArray[0]->_argumentsArray[0])->c_str();
    string str = final[0];

    if(c == 1)
      chdir(getenv("HOME"));

    else{

      int ret = chdir(final[1]);
      if ( ret == -1){
        fprintf(stderr,"cd: can't cd to %s\n",(final[1]));
      }

    }
    return 1;
  }

  return 0;

}


char* checkEnvironment(string s){

  // cout << s << endl;
  string temp;
  char* temp2;
  string s1 = string(s);
  string s2 = string(s);
  temp = string(s);
  if(s.at(0)== '$'){

    s1.replace(0,2,"");
    s1.pop_back();

    temp2 = getenv(s1.c_str());

    if(temp2 == NULL){
      temp = s;
    }


    if(s.compare("${?}")==0){
      int pid = WEXITSTATUS(last);
      char mypid[6];   // ex. 34567
      sprintf(mypid, "%d", pid);
      temp = mypid;
    }

    if(s.compare("${$}")== 0){

      int pid = getpid();
      char mypid[6];   // ex. 34567
      sprintf(mypid, "%d", pid);
      temp = mypid; 

    }

    if(s.compare("${!}")== 0){
      int pid = last_id;
      char mypid[6];   // ex. 34567
      sprintf(mypid, "%d", pid);
      temp = mypid;
    }

    if(s.compare("${_}")==0){
  
      temp = last_arg;  

    }

    if(s.compare("${SHELL}")==0){
      temp = realpath(Shell::arg,NULL);

    }


  }

  s2 = temp;
  return strcpy(new char[s2.length()+ 1],s2.c_str());


}

char* tilde(char* s){
  string str = s;
  string s2;


  if(strchr(s,'~')!=NULL){

    for(unsigned int i = 1 ; i < strlen (s); i++){
      if(str[i]=='/')
        break;
      else{
        s2+=str[i];
      }
    }

    if(s[1] == '/' || s[1]=='\0')
      str.replace(0, 1,getenv("HOME") ); 
    else{
      char* str2 = strchr(s,'/');
      str = getpwnam(s2.c_str())->pw_dir ;
      if(str2!=NULL)
        str+=str2;
    }

  } 
  char* nstr = strcpy(new char[str.length()+ 1],str.c_str());
  return nstr;
}

int s_count;
void Command::execute() {
  // Don't do anything if there are no simple commands
  if ( _simpleCommandsArray.size() == 0 ) { 
    Shell::prompt();
    return;
  } 
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
      close(tmpin);
      close(tmpout);
      close(tmperr);
      clear();
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
     
    if(fderr == -1){
      close(tmpin);
      close(tmpout);
      close(tmperr);
      clear();
      exit(1);
    }
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
       close(tmpin); 
       close(tmpout);
       close(tmperr);
      cout<< "Ambiguous output redirect." <<endl;
      clear();
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
    string s2 = *(simpleCommand->_argumentsArray[0]);
    char *a = (char*)(s.c_str());
  
    int c =0;
    char *final_arr[10000];
  
    
    for(auto & word : simpleCommand->_argumentsArray){

      s_count+=simpleCommand->number_args;
      char*tmp1 = const_cast<char*>(checkEnvironment((char*)word->c_str()));
      char*tmp2=pecho(tmp1);
      free(tmp1);
      char*tmp3 = esc(tmp2);
      free(tmp2);
      char*str = tilde(tmp3); 
      free(tmp3);
      final_arr[c]=strdup(str);
      free(str); 
      c=c+1;
      
      
    }  
    
     
    final_arr[c]=NULL;
    last_arg = final_arr[c-1];
    
  
    int check_fun = commandCheck(final_arr , c );
    if(check_fun == 2){
      close(tmpin); 
      close(tmpout);
      close(tmperr);
      clear();
      exit(0);
    }
    if(check_fun == 1){
      clear();    
      close(tmpin); 
      close(tmpout);
      close(tmperr);
      Shell::prompt();
      return;
    }


    ret = fork();


    if (ret == 0) { 
      close(tmpin); 
       close(tmpout);
       close(tmperr);
      if(_backgnd){
        last_id=getpid();
      }
      if(s.compare("printenv") == 0){
        for(int i = 0 ;environ[i]!=NULL; i++){

          cout << *(environ+i)<<endl;  
        }
        clear();
        exit(1);
      }
      else{
        execvp(a, final_arr);
        perror("execvp");
        _exit(1); 
      }

    }

    else if (ret < 0) { 
      perror("fork");
      // close(tmperr);
      return;
    }
    
   for(int i =0 ; i <c ;i++){
     free(final_arr[i]);
   }
  
    count += 1; 
   
  } // for 
 
  dup2(tmpin,0);
  dup2(tmpout,1);

  close(tmpin);
  close(tmpout);
  close(tmperr);
 

  if (!_backgnd) {
    // Wait for last command
    waitpid(ret ,&last, 0);
  }


  // print();
  
  clear();
  
  Shell::prompt();
} 


SimpleCommand * Command::_currSimpleCommand;
