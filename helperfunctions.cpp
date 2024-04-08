#include <fstream>
#include "helperfunctions.h"

std::vector<std::string> split(std::string str, std::string sep) {
	char* cstr = const_cast<char*>(str.c_str());
	char* current;
	std::vector<std::string> arr;
	current = strtok(cstr, sep.c_str());
	while (current != NULL) {
		arr.push_back(current);
		current = strtok(NULL, sep.c_str());
	}
	return arr;
}

bool DoesFileExist(const std::string& name) {
	if (FILE *file = fopen(name.c_str(), "r")) {
		fclose(file);
		return true;
	}
	else {
		return false;
	}
}

int search(root* r, std::vector<std::string> vec)
{
	std::regex pattern1("root/[a-z0-9]+/[a-z0-9]+$");
	std::regex pattern2("root/[a-z0-9]+$");
	if (std::regex_match(vec[1], pattern1))
	{
		std::vector<std::string> path = split(vec[1], "/");
		for (int i = 0; i < r->num_of_dir; i++)
		{
			if (strcmp(r->directories[i].name, path[1].c_str()) == 0)
			{
				for (int j = 0; j < r->directories[i].num_of_files; j++)
				{
					if (strcmp(r->directories[i].files[j].name, path[2].c_str()) == 0)
						return r->directories[i].files[j].inode_number;
				}				
			}
		}
		return -1;
	}
	else if (std::regex_match(vec[1], pattern2))
	{
		std::vector<std::string> path = split(vec[1], "/");
		for (int i = 0; i < r->num_of_files; i++)
		{
			if (strcmp(r->files[i].name, path[1].c_str()) == 0)
			{
				return r->files[i].inode_number;
			}
		}
		return -1;
	}
	return -2;
}

std::vector<std::string> Split(const std::string& str, int splitLength)
{
	int NumSubstrings = str.length() / splitLength;
	std::vector<std::string> ret;

	for (auto i = 0; i < NumSubstrings; i++)
	{
		ret.push_back(str.substr(i * splitLength, splitLength));
	}

	if (str.length() % splitLength != 0)
	{
		ret.push_back(str.substr(splitLength * NumSubstrings));
	}
	return ret;
}



void free_inode(inode* inodes, bool* bitmap,int inode_number)
{
	std::ofstream f;
	f.open("file.bin", std::ios::binary | std::ios::in);
	std::vector<int> blocks;
	for (int i = 0; i < 15; i++)
	{
		for (int j = 0; j < inodes[inode_number].pointers[i].offset; j++)
		{
			blocks.push_back(inodes[inode_number].pointers[i].block_pointer / 5 + j);
		}
		inodes[inode_number].pointers[i].block_pointer = -1;
		inodes[inode_number].pointers[i].offset = 0;
	}
	char erase[6] = { 0 };
	for (int i = 0; i < blocks.size(); i++)
	{
		bitmap[blocks[i]] = false;
		f.seekp(blocks[i] * 5, std::ios::beg);
		f.write(erase, 5);
	}
	f.close();
	
}