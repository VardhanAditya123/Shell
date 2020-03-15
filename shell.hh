#ifndef shell_hh
#define shell_hh

#include "command.hh"

struct Shell {

  static void prompt();
  static char* pth;
  static Command _currentCommand;
};

#endif
