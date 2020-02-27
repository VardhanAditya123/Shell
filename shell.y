
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
pipe_list PIPE cmd_and_args
| cmd_and_args
;


io_modifier_list:
io_modifier_list io_modifier
| /*empty*/
;


// >
io_modifier:
GREATGREAT Word{Shell::_currentCommand._outFileName = $2;}
| GREAT Word{Shell::_currentCommand._outFileName = $2;}
| GREATGREATAMPERSAND Word{Shell::_currentCommand._outFileName = $2;}
| GREATAMPERSAND Word{Shell::_currentCommand._outFileName = $2;}
| LESS Word{Shell::_currentCommand._outFileName = $2;}

;



cmd_and_args:

Word{Command::_currSimpleCommand = new SimpleCommand(); Command::_currSimpleCommand->insertArgument($1);} arg_list  
;


arg_list:
arg_list Word{Command::_currSimpleCommand->insertArgument($2);}
| /*empty*/
;



background_optional:
AMPERSAND
| /*empty*/
;


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
