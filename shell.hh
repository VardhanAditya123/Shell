#ifndef shell_hh
#define shell_hh

#include "command.hh"

struct Shell {

  static void prompt();
  static int last;
  static Command _currentCommand;
};

#endif
