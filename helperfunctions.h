#pragma once
#include <vector>
#include <regex>
#include "ext4structs.h"

std::vector<std::string> split(std::string str, std::string sep);
std::vector<std::string> Split(const std::string& str, int splitLength);
int search(root* r, std::vector<std::string> vec);
void free_inode(inode*, bool*, int);
bool DoesFileExist(const std::string&);