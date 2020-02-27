
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
%token NOTOKEN GREAT NEWLINE LESS GREATGREAT GREATGREATAMPERSAND GREATAMPERSAND PIPE AMPERSAND

%{
//#define yylex yylex
#include <cstdio>
#include "shell.hh"

void yyerror(const char * s);
int yylex();

%}

%%

goal: command_list;
arg_list:

arg_list Word
| /*empty*/
;

cmd_and_args:
Word arg_list
;

pipe_list:
pipe_list PIPE cmd_and_args
| cmd_and_args
;

// >
io_modifier:
GREATGREAT Word
| GREAT Word
| GREATGREATAMPERSAND Word
| GREATAMPERSAND Word
| LESS Word

;

io_modifier_list:

io_modifier_list io_modifier
| /*empty*/
;

background_optional:
AMPERSAND
| /*empty*/
;


command_line:
pipe_list io_modifier_list
background_optional NEWLINE
| NEWLINE /*accept empty cmd line*/
| error NEWLINE{yyerrok;}
;
/*error recovery*/


command_list:
command_line |
command_list command_line
;
/* command loop*/


%%

void
yyerror(const char * s)
{
  fprintf(stderr,"%s", s);
}

#if 0
main()
{
  yyparse();
}
#endif
