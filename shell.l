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

static  void yyunput (int c,char *buf_ptr  );

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

\$[^ \n]* {
   std::string s = yytext;
   s.replace(0,2,"");
   s.pop_back();
   yylval.cpp_string = new std::string(s);
   return Word;
}

[^ \t\n][^ \t\n]*  {
  /* Assume that file names have only alpha chars */
  yylval.cpp_string = new std::string(yytext);
  return Word;
}
