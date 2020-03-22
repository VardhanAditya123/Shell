
/*
 * CS-252
 * shell.y: parser for shell
 *
 * This parser compiles the following grammar:
 *
 *	cmd [arg]* [> filename]
 *
 * you must extend it to understand the complete shell grammar
 *
 * NOTICE: This lab is property of Purdue University. You should not for any reason make this code public.
 */

%code requires 
{
#include <string>

#if __cplusplus > 199711L
#define register      // Deprecated in C++11 so remove the keyword
#endif
}

%union
{
  char        *string_val;
  // Example of using a c++ type in yacc
  std::string *cpp_string;
}

%token <cpp_string> Word
%token NOTOKEN GREAT NEWLINE LESS GREATGREAT GREATGREATAMPERSAND GREATAMPERSAND PIPE AMPERSAND TWOGREAT QUOTE CDNEWLINE

%{
//#define yylex yylex
#include <cstdio>
#include "shell.hh"
#include <string.h>
#include <string>
#include <stdio.h>
#include "command.hh"
#include "string.h"
#include <sys/types.h>
#include <regex.h>
#include <dirent.h>
#include <unistd.h>
#include <assert.h>
#include <iostream>
#include <vector>
using namespace std;
void yyerror(const char * s);
int yylex();
void expandWildcardsIfNecessary(string*s);
%}

%%

goal: command_list;


// ls -l | grep a
command_list:
command_line{
    Shell::_currentCommand.execute();
  } |
command_list command_line{
    Shell::_currentCommand.execute();
  }
;
/* command loop*/


command_line:
pipe_list io_modifier_list
background_optional NEWLINE {Shell::_currentCommand.insertSimpleCommand( Command::_currSimpleCommand );}
| NEWLINE 
| error NEWLINE{yyerrok;}
;
/*error recovery*/


pipe_list:
pipe_list PIPE{Shell::_currentCommand.insertSimpleCommand(Command::_currSimpleCommand);} cmd_and_args
| cmd_and_args
;


io_modifier_list:
io_modifier_list io_modifier
| /*empty*/
;


// >
io_modifier:
GREATGREAT Word{Shell::_currentCommand._outFileName = $2;Shell::_currentCommand._append=true;Command::_currSimpleCommand->ambig_count+=1;}
| GREAT Word{Shell::_currentCommand._outFileName = $2;Command::_currSimpleCommand->ambig_count+=1;}
| GREATGREATAMPERSAND Word{Shell::_currentCommand._outFileName = $2; Shell::_currentCommand._errFileName = $2;Shell::_currentCommand._append=true;Command::_currSimpleCommand->ambig_count+=1;}
| GREATAMPERSAND Word{Shell::_currentCommand._outFileName = $2 ;Shell::_currentCommand._errFileName = $2;Command::_currSimpleCommand->ambig_count+=1;}
| LESS Word{Shell::_currentCommand._inFileName = $2;Command::_currSimpleCommand->ambig_count+=1;}
| TWOGREAT Word{Shell::_currentCommand._errFileName = $2;Command::_currSimpleCommand->ambig_count+=1;}

;



cmd_and_args:

Word{Command::_currSimpleCommand = new SimpleCommand();expandWildcardsIfNecessary($1);} arg_list  
;


arg_list:
arg_list Word{ expandWildcardsIfNecessary($2);}
| /*empty*/
;



background_optional:
AMPERSAND{Shell::_currentCommand._backgnd = true ;}
| /*empty*/
;


%%

void
yyerror(const char * s)
{
  fprintf(stderr,"%s", s);
}

void expandWildcardsIfNecessary(std::string * str){
// Return if arg does not contain ‘*’ or ‘?’
std::vector<string*> vec;
char* arg = (char*)(str->c_str());
if (strchr(arg,'?')==NULL && strchr(arg,'*')==NULL) {
Command::_currSimpleCommand->insertArgument(str);

return;
}
string reg;
string a = arg;
reg+='^';
for(unsigned int i = 0 ; i < a.length();i++){
  if(a.at(i)=='*'){
    reg+=".*";
  }
  else if(a.at(i)=='?')
  reg+='.';
  else if(a.at(i)=='.')
  reg+='.';
  else
  reg+=a.at(i);
  
}
reg+='$';

  regex_t re;	
	int result = regcomp( &re, reg.c_str(),  REG_EXTENDED|REG_NOSUB);
	if (result!=0) {
  perror("compile");
  return;
  }

  DIR * dir = opendir(".");
  if (dir == NULL) {
  perror("opendir");
  return;
}  

struct dirent * ent;

while ( (ent = readdir(dir))!= NULL) {
// Check if name matches

string tmp;
regmatch_t match;
tmp = (ent->d_name);
arg = (char*)(tmp.c_str());
result = regexec( &re, arg, 1, &match, 0 );

if (result == 0 ) {
 vec.push_back(&tmp);
}

}


for(int i = 0 ; i < vec.size() ; i++){
  // cout << vec.at(i) << endl;
  
  string s1  = (vec.at(i));
   cout << s1 <<" "<< *s1 << endl;
  Command::_currSimpleCommand->insertArgument(&s1);
}
closedir(dir);
 regfree(&re);
 
}

#if 0
main()
{
  yyparse();
}
#endif
