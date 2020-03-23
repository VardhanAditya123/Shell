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
#include <cstdio>
#include <string>
#include <stdio.h>
#include "string.h"
#include <regex.h>
#include <dirent.h>
#include <unistd.h>
#include <vector> 
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
  _argumentsArray.push_back(argument);
}

void SimpleCommand:: expandWildcardsIfNecessary(std::string * str){
// Return if arg does not contain ‘*’ or ‘?’

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
std::vector<string> vec = std::vector<string>();;
while ( (ent = readdir(dir))!= NULL) {
// Check if name matches
string tmp;
regmatch_t match;
tmp += (ent->d_name);
arg = (char*)(tmp.c_str());
result = regexec( &re, arg, 1, &match, 0 );
// std::string *myString = new std::string(tmp);
if (result == 0 ) {
  number_args+=1;
  vec.push_back(tmp);
  //  _argumentsArray.push_back(myString);
}

}
 std::sort (vec.begin(), vec.end()); 
for(auto s1 : vec){
    cout << s1.c_str << endl;
    string *myString = new string(s1.c_str());
    _argumentsArray.push_back(myString);
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
