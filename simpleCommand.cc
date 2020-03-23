#include <cstdio>
#include <cstdlib>
#include "command.hh"
#include "shell.hh"
#include <unistd.h>
#include <iostream>
#include <string.h>
#include <assert.h>
#include <stdlib.h>
#include <limits.h>
#include <algorithm>
#include <bits/stdc++.h>

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


std::vector<std::string> vect = std::vector<string>();
void SimpleCommand::expandWildcardsIfNecessary(std::string * str){
// Return if arg does not contain ‘*’ or ‘?’
std::vector<std::string> vect = std::vector<string>();
char* arg = (char*)(str->c_str());
if (strchr(arg,'?')==NULL && strchr(arg,'*')==NULL) {
Command::_currSimpleCommand->insertArgument(str);

return;
}

else{
  expandWildcard(NULL,(char*)(str->c_str()));

}
}


void SimpleCommand::expandWildcard(char * prefix, char *suffix) {

if (suffix[0]== 0) {
// suffix is empty. Put prefix in argument.
vect.push_back(prefix);
return;
}
// Obtain the next component in the suffix
// Also advance suffix.
char * s = strchr(suffix, '/');
char component[1024];

if (s!=NULL){ // Copy up to the first “/”
if(s!=suffix)
strncpy(component,suffix, s-suffix);
else
component[0]='\0';
suffix = s + 1;
}
else { // Last part of path. Copy whole thing.
strcpy(component, suffix);
suffix = suffix + strlen(suffix);
}

char newPrefix[1024];
if ( strchr(component,'?')==NULL && strchr(component,'*')==NULL) {

if(prefix==NULL){
sprintf(newPrefix,"%s",  component);
}
else
sprintf(newPrefix,"%s/%s", prefix, component);

expandWildcard(newPrefix, suffix);
return;

}

string reg;
string a = component;
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
  //  DIR * dir;
   char* dir2;
	if(prefix == NULL)
	{
		strcpy(dir2 , ".");
	}else if(!strcmp("", prefix))
	{
		dir2 = strdup("/");
	}
	else
	{
		dir2 = prefix;
	}

  DIR *dir = opendir(dir2);

  if (dir == NULL) {
  perror("opendir");
  return;
}  

struct dirent * ent;
int c = 0;
while ( (ent = readdir(dir))!= NULL) {
// Check if name matches
string tmp;
regmatch_t match;
tmp += (ent->d_name);
char* arg = (char*)(tmp.c_str());
result = regexec( &re, arg, 1, &match, 0 );
if (result == 0  ) {
if(prefix==NULL){
sprintf(newPrefix,"%s",  ent->d_name);
}
else
sprintf(newPrefix,"%s/%s", prefix, ent->d_name);
   expandWildcard(newPrefix,suffix);
   
}
}
sort(vect.begin(),vect.end());
//  cout << reg << endl;
 

for(auto str: vect){
  
  if(reg.compare("^..*$")==0){
  if( str.at(0) =='.'  )
  Command ::_currSimpleCommand->insertArgument(new string(str)); 
  }
  else{
    
  if(str.at(str.size()-1) !='.' && str.at(0) !='.'  )
  Command ::_currSimpleCommand->insertArgument(new string(str)); 
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
