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
#include "shell.hh"
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
#include <iostream>
#include <assert.h>
#include <fcntl.h>
#include <algorithm>
#include <bits/stdc++.h> 
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




\$\([^\n]*\) {
  

    std::string s = yytext;
    s.replace(0,2,"");
    s.pop_back();
    s.append("\nexit\n");
    char str[1000];
    strcpy(str,s.c_str());
    int in[2];
    int out[2];
    pipe(in); 
    pipe(out);

    int tmpin=dup(0);
    int tmpout=dup(1);

    write(in[1],str,strlen(str));
    close(in[1]);
    
    int ret = fork();
    if(ret == 0){
       dup2(in[0],0);
       close(in[0]);
       dup2(out[1],1);
       close(out[1]);
       execvp("/proc/self/exe",NULL);
      
    }
     else if(ret < 0){
      perror("fork");
    }
    
    else if(ret > 0){
      close(in[0]);
      close(out[1]);
      dup2(tmpin,0);
      dup2(tmpout,1);
      close(tmpin);
      close(tmpout);
      
      char str2[1000];
    
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
      close(out[0]);
     

      s = str2;
      for(int i = s.length() -1  ; i >= 0;i--){
      myunputc(s.at(i));
      }

    }

}


[\t\&\t]*  {
  /* Assume that file names have only alpha chars */
  yylval.cpp_string = new std::string(yytext);
  return Word;
}

[^ \t\n\>\|\&]*  {
  /* Assume that file names have only alpha chars */
  yylval.cpp_string = new std::string(yytext);
  return Word;
}
