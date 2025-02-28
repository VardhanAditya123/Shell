#ifndef simplcommand_hh
#define simplecommand_hh

#include <string>
#include <vector>

struct SimpleCommand {

  // Simple command is simply a vector of strings
  std::vector<std::string *> _argumentsArray;
  SimpleCommand();
  int ambig_count=0;
  bool w_check = false;
  ~SimpleCommand();
  void insertArgument( std::string * str);
  void expandWildcardsIfNecessary(std::string * str);
  void expandWildcard(char*, char*);
  void reset();
  void print();
  int number_args=0;

};

#endif
