#include <cstdio>
#include <cstdlib>
#include <string.h>
#include <iostream>
#include <regex.h>
#include "simpleCommand.hh"
#include <dirent.h>
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

char* arg = (char*)(str->c_str());
if (strchr(arg,'?')==NULL && strchr(arg,'*')==NULL) {
number_args+=1;
_argumentsArray.push_back(str);
return;
}
string reg;
string a = arg;
reg+='^';
for(unsigned int i = 0 ; i < a.length();i++){
  if(a.at(i)=='*'){
    reg+=".*";
  }
  else if(a.at(i)=='?')
  reg+='.';
  else if(a.at(i)=='.')
  reg+='.';
  else
  reg+=a.at(i);
  
}
reg+='$';

  regex_t re;	
	int result = regcomp( &re, reg.c_str(),  REG_EXTENDED|REG_NOSUB);
	if (result!=0) {
  perror("compile");
  return;
  }

  DIR * dir = opendir(".");
  if (dir == NULL) {
  perror("opendir");
  return;
}  

struct dirent * ent;
while ( (ent = readdir(dir))!= NULL) {
// Check if name matches
regmatch_t match;
result = regexec( &re, ent->d_name, 1, &match, 0 );
if (result ==0 ) {
number_args+=1;
string tmp = ent->d_name;
_argumentsArray.push_back(&tmp);
}
}
closedir(dir);
}

// Print out the simple command
void SimpleCommand::print() {
  for (auto & arg : _argumentsArray) {
    std::cout << "\"" << *arg << "\" \t";
  }
  // effectively the same as printf("\n\n");
  std::cout << std::endl;
}
