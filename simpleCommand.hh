#ifndef simplcommand_hh
#define simplecommand_hh

#include <string>
#include <vector>

struct SimpleCommand {

  // Simple command is simply a vector of strings
  std::vector<std::string *> _argumentsArray;
  int ambig_count=0;
  SimpleCommand();
  ~SimpleCommand();
  void insertArgument( std::string * argument );
  void print();
};

#endif
