#ifndef shell_hh
#define shell_hh

#include "command.hh"

struct Shell {

  static void prompt();
  char** arg;
  static Command _currentCommand;
};

#endif
