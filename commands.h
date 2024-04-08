#include <iostream>
#include <fstream>
#include "helperfunctions.h"


void mkdir(root*,std::vector<std::string>);
void create(root*,std::vector<std::string>,bool*,inode*);
void put(std::vector<std::string>, bool*, root*, inode*);
void get(std::vector<std::string>, root*, inode*);
void ls(std::vector<std::string>,root*);
void cp(std::vector<std::string>, root*, inode*, bool*);
void mv(std::vector<std::string>, bool*, root*, inode*);
void rename(std::vector<std::string>,root*);
void echo(std::vector<std::string>,bool*,root*,inode*);
std::string cat(std::vector<std::string>,root*,inode*);
void rm(std::vector<std::string>, root*, inode*, bool*);
void echo(std::vector<std::string>, bool*, root*, inode*, std::string);
void stat(std::vector<std::string>,root*,inode*);
