#ifndef shell_hh
#define shell_hh

#include "command.hh"

struct Shell {

  static void prompt();
  static char* arg;
  static char* arg2;
  static Command _currentCommand;
};


#endif
