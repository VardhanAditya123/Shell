
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
#include <algorithm>
using namespace std;

void yyerror(const char * s);
int yylex();
void expandWildcardsIfNecessary(string*s);
void expandWildcard(char* prefix , char* suffix);
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
arg_list Word{expandWildcardsIfNecessary($2);}
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

 void SimpleCommand::insertArgument(string* argument ) {
  // simply add the argument to the vector
  // cout << argument[0] << " ";
   number_args+=1;
  _argumentsArray.push_back(argument);
}

void expandWildcardsIfNecessary(std::string * str){
// Return if arg does not contain ‘*’ or ‘?’
std::vector<std::string> vect = std::vector<string>();
char* arg = (char*)(str->c_str());
if (strchr(arg,'?')==NULL && strchr(arg,'*')==NULL) {
Command::_currSimpleCommand->insertArgument(str);

return;
}

else{
  expandWildcard(NULL,(char*)(str->c_str()));
}
}

vector<string>vect;
void expandWildcard(char * prefix, char *suffix) {

if (suffix[0]== 0) {
// suffix is empty. Put prefix in argument.
vect.push_back(prefix);
return;
}
// Obtain the next component in the suffix
// Also advance suffix.
char * s = strchr(suffix, '/');
char component[1024];

if (s!=NULL){ // Copy up to the first “/”
if(s!=suffix)
strncpy(component,suffix, s-suffix);
else
component[0]='\0';
suffix = s + 1;
}
else { // Last part of path. Copy whole thing.
strcpy(component, suffix);
suffix = suffix + strlen(suffix);
}

char newPrefix[1024];
if ( strchr(component,'?')==NULL && strchr(component,'*')==NULL) {

if(prefix==NULL){
sprintf(newPrefix,"%s",  component);
}
else
sprintf(newPrefix,"%s/%s", prefix, component);

expandWildcard(newPrefix, suffix);
return;

}

string reg;
string a = component;
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
  //  DIR * dir;
   char* dir2;
	if(prefix == NULL)
	{
		strcpy(dir2 , ".");
	}else if(!strcmp("", prefix))
	{
		dir2 = strdup("/");
	}
	else
	{
		dir2 = prefix;
	}

  DIR *dir = opendir(dir2);

  if (dir == NULL) {
  perror("opendir");
  return;
}  

struct dirent * ent;
int c = 0;
while ( (ent = readdir(dir))!= NULL) {
// Check if name matches
string tmp;
regmatch_t match;
tmp += (ent->d_name);
char* arg = (char*)(tmp.c_str());
result = regexec( &re, arg, 1, &match, 0 );
if (result == 0  ) {
if(prefix==NULL){
sprintf(newPrefix,"%s",  ent->d_name);
}
else
sprintf(newPrefix,"%s/%s", prefix, ent->d_name);
   expandWildcard(newPrefix,suffix);
   
}
}
sort(vect.begin(),vect.end());
for(auto str: vect){
  if(str.at(str.size()-1) !='.' && str.at(0) !='.'  )
  Command ::_currSimpleCommand->insertArgument(new string(str));
}
}


#if 0
main()
{
  yyparse();
}
#endif
