/*
 *
 * CS-252 
 * shell.l: lexical analyzer for shell
 * You have to extend it.
 *
 * NOTICE: This lab is property of Purdue University. You should not for any reason make this code public.
 */

%{

#include <cstring>
#include "y.tab.hh"
#include <string.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/wait.h>
#include <stdlib.h>
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
static  void yyunput (int c,char *buf_ptr  );
using namespace std;
void myunputc(int c) {
  unput(c);
}

%}

%option noyywrap

%%


\n {
  return NEWLINE;
}

[ \t] {
  /* Discard spaces and tabs */
}

">" {
  return GREAT;
}

"<" {
  return LESS;
}

"2>" {
  return TWOGREAT;
}

">>" {
  return GREATGREAT;
}

">>&" {
  return GREATGREATAMPERSAND;
}

">&" {
  return GREATAMPERSAND;
}

"|" {
  return PIPE;
}

"&" {
  return AMPERSAND;
}

\"[^\n]*\" {
   std::string s = yytext;
   s.replace(0,1,"");
   s.pop_back();
   yylval.cpp_string = new std::string(s);
   return Word;
}

\$[^\n]* {
  

    std::string s = yytext;
    s.replace(0,2,"");
    s.pop_back();
   

    int in[2];
    int out[2];
    int tmpin=dup(0);
    int tmpout=dup(1);
    pipe(in); 
    pipe(out);

    write(in[1],s,strlen(str));
    write(in[1],"\n",1);
    write(in[1],"exit\n",3);
    write(in[1],"\n",1);
    
    dup2(in[0],0);
    close(in[1]);
    int ret = fork();
    if(ret == 0){
      dup2(in[0],0);
      dup2(out[1], 1); 
      
      execvp("/proc/self/exe",NULL);
      exit(0);
      
    }
    
    else if(ret > 0){
      
  
      char str[1000];
      char str2[1000];
      strcpy(str,s.c_str());
      dup2(in[1],1);
      dup2(out[0], 0); 
      
     
      wait(NULL);
      close(in[0]);
      close(out[1]);
     
      read(out[0],str,1000);
      int i = 0;
      char c;
      while(read(out[0],&c,1)){
        if(c == '\n'){
          str2[i] = ' ';
        }
        else{
          str2[i] = c;
        }
        i++;
      }
      str2[i] = '\0';
      
       close(in[1]);
      close(out[0]);
      dup2(tmpin,0);
      dup2(tmpout,1);
      close(tmpin);
      close(tmpout);
      s = str2;
      for(int i = s.length() -1  ; i >= 0;i--){
      myunputc(s.at(i));
      }

    }
     

  
  //  std::string s = yytext;
  //  yylval.cpp_string = new std::string(s);
  //  return Word;
}

[^ \t\n][^ \t\n]*  {
  /* Assume that file names have only alpha chars */
  yylval.cpp_string = new std::string(yytext);
  return Word;
}
