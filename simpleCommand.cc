#include <cstdio>
#include <cstdlib>
#include <string.h>
#include <iostream>

#include "simpleCommand.hh"
using namespace std;

SimpleCommand::SimpleCommand() {
  _argumentsArray = std::vector<std::string *>();
}

SimpleCommand::~SimpleCommand() {
  // iterate over all the arguments and delete them
  for (auto & arg : _argumentsArray) {
    delete arg;
  }
}

 void SimpleCommand::insertArgument( std::string * argument ) {
  // simply add the argument to the vector
   number_args+=1;
  _argumentsArray.push_back(argument);
}

void SimpleCommand::expandWildcardsIfNecessary(std::string * str)
{
// Return if arg does not contain ‘*’ or ‘?’
char* arg = (char*)(str);
if (strchr(arg,'?')==NULL && strchr(arg,'!')==NULL) {
number_args+=1;
_argumentsArray.push_back(*str);
}
}

// Print out the simple command
void SimpleCommand::print() {
  for (auto & arg : _argumentsArray) {
    std::cout << "\"" << *arg << "\" \t";
  }
  // effectively the same as printf("\n\n");
  std::cout << std::endl;
}
