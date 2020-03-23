#include <cstdio>
#include <cstdlib>
#include <iostream>
#include "command.hh"
#include "shell.hh"
#include <unistd.h>
#include <sys/types.h>
#include <sys/wait.h>
#include <iostream>
#include <string.h>
#include <assert.h>
#include <stdlib.h>
#include <limits.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <algorithm>
#include <bits/stdc++.h> 
using namespace std;

SimpleCommand::SimpleCommand() {
  _argumentsArray = std::vector<string*>();
}

SimpleCommand::~SimpleCommand() {
  // iterate over all the arguments and delete them
  for (auto & arg : _argumentsArray) {
    delete arg;
  }
}

 void SimpleCommand::insertArgument(string* argument ) {
  // simply add the argument to the vector
  // cout << argument[0] << " ";
   number_args+=1;
  //  cout << *argument << endl;
  _argumentsArray.push_back(argument);
}

void expandWildcardsIfNecessary(std::string * str){
// Return if arg does not contain ‘*’ or ‘?’
std::vector<string> vec;
char* arg = (char*)(str->c_str());
if (strchr(arg,'?')==NULL && strchr(arg,'*')==NULL) {
Command::_currSimpleCommand->insertArgument(str);

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

string tmp;
regmatch_t match;
tmp += (ent->d_name);
arg = (char*)(tmp.c_str());
result = regexec( &re, arg, 1, &match, 0 );

if (result == 0 ) {
 vec.push_back(tmp);
}

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
