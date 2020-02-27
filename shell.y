
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

%token <cpp_string> WORD
%token NOTOKEN GREAT NEWLINE LESS GREATGREAT GREATGREATAMPERSAND GREATAMPERSAND PIPE AMPERSAND

%{
//#define yylex yylex
#include <cstdio>
#include "shell.hh"

void yyerror(const char * s);
int yylex();

%}

%%

goal:
  command_list
  ;


command_list:
  command_line |
  command_list command_line
;
/* command loop*/

command_line: simple_command
;


// ls a b > 
simple_command:	
  command_and_args iomodifier_opt NEWLINE {
    Shell::_currentCommand.execute();
  }
  | NEWLINE 
  | error NEWLINE { yyerrok; }
  ;


// >
iomodifier_opt:
  GREAT WORD {
    printf("   Yacc: insert output \"%s\"\n", $2->c_str());
    Shell::_currentCommand._outFileName = $2;
  }
  | /* can be empty */ 
  ;


// ls a b
command_and_args:  
  command_word argument_list {
    Shell::_currentCommand.
    insertSimpleCommand( Command::_currSimpleCommand );
  }
  ;


// a b
argument_list:
  argument_list argument
  | /* can be empty */
  ;


// a
argument:
  WORD {
    printf("   Yacc: insert argument \"%s\"\n", $1->c_str());
    Command::_currSimpleCommand->insertArgument( $1 );
  }
  ;

// ls
command_word:
  WORD {
    printf("   Yacc: insert command \"%s\"\n", $1->c_str());
    Command::_currSimpleCommand = new SimpleCommand();
    Command::_currSimpleCommand->insertArgument($1);
  }
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
