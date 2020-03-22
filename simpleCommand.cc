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

// void SimpleCommand::expandWildcardsIfNecessary(std::string * str){
// // Return if arg does not contain ‘*’ or ‘?’

// char* arg = (char*)(str->c_str());
// if (strchr(arg,'?')==NULL && strchr(arg,'*')==NULL) {
// number_args+=1;
// _argumentsArray.push_back(str);
// return;
// }
// string reg;
// string a = arg;
// reg+='^';
// for(unsigned int i = 0 ; i < a.length();i++){
//   if(a.at(i)=='*'){
//     reg+=".*";
//   }
//   else if(a.at(i)=='?')
//   reg+='.';
//   else if(a.at(i)=='.')
//   reg+='.';
//   else
//   reg+=a.at(i);
  
// }
// reg+='$';

//   regex_t re;	
// 	int result = regcomp( &re, reg.c_str(),  REG_EXTENDED|REG_NOSUB);
// 	if (result!=0) {
//   perror("compile");
//   return;
//   }

//   DIR * dir = opendir(".");
//   if (dir == NULL) {
//   perror("opendir");
//   return;
// }  

// struct dirent * ent;
// string tmp;
// string *tmp_ptr;
// while ( (ent = readdir(dir))!= NULL) {
// // Check if name matches
// regmatch_t match;
// tmp = ent->d_name;
// tmp_ptr = &tmp;
// arg = (char*)(tmp.c_str());

// result = regexec( &re, arg, 1, &match, 0 );

// if (result == 0 ) {
 
//   number_args+=1;
//   cout << *tmp_ptr << " ";
//   _argumentsArray.push_back(tmp_ptr);
  
// }

// }
// cout << "LASST" << endl;
// closedir(dir);
//  regfree(&re);
 
// }

// Print out the simple command
void SimpleCommand::print() {
  for (auto & arg : _argumentsArray) {
    std::cout << "\"" << *arg << "\" \t";
  }
  // effectively the same as printf("\n\n");
  std::cout << std::endl;
}
