#ifndef command_hh
#define command_hh

#include "simpleCommand.hh"

// Command Data Structure

struct Command {
  std::vector<SimpleCommand *> _simpleCommandsArray;
  std::string * _outFileName;
  std::string * _inFileName;
  std::string * _errFileName;
  bool _append;
  bool _backgnd;
  Command();
  void insertSimpleCommand( SimpleCommand * simpleCommand );

  void clear();
  void print();
  void execute();
  int subShell();

  static SimpleCommand *_currSimpleCommand;
};

#endif
