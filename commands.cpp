#pragma once
#include "commands.h"
#include <chrono>

using std::cout;
using std::endl;

void mkdir(root* r,std::vector<std::string> vec)
{
	if (vec.size() != 2)
		cout << "The syntax of the command is incorrect" << endl;
	else
	{
		std::regex pattern("root/[a-z0-9]+$");
		if (std::regex_match(vec[1], pattern))
		{
			std::vector<std::string> path = split(vec[1], "/");
			for (int i = 0; i < r->num_of_dir; i++)
			{
				if (strcmp(r->directories[i].name, path[1].c_str())== 0)
				{
					cout << "Directory already exists" << endl;
					return;
				}
			}
			strcpy(r->directories[r->num_of_dir++].name, path[1].c_str());		
		}
		else
			cout << "The syntax of the command is incorrect" << endl;
	}
}
void create(root* r,std::vector<std::string> vec,bool* bitmap,inode* inodes)
{
	if (vec.size() != 2  && vec.size() != 3)
		cout << "The syntax of the command is incorrect" << endl;
	else
	{
		if (search(r, vec) != -1)
		{
			cout << "File already exists" << endl;
			return;
		}
		std::regex pattern1("root/[a-z0-9]+/[a-z0-9]+$");
		std::regex pattern2("root/[a-z0-9]+$");
		if (std::regex_match(vec[1], pattern1))
		{
			std::vector<std::string> path = split(vec[1], "/");
			for (int i = 0; i < r->num_of_dir; i++)
			{
				if (strcmp(r->directories[i].name, path[1].c_str()) == 0)
				{
					strcpy(r->directories[i].files[r->directories[i].num_of_files].name, path[2].c_str());
					int block = 0;
					while (bitmap[block] && block < 4'000'000)
						block++;
					if (block < 4'000'000)
					{
						bitmap[block] = true;
						for (int j = 0; j < 2660; j++)
						{
							if (inodes[j].pointers[0].block_pointer == -1)
							{
								inodes[j].pointers[0].block_pointer = block * 5;
								inodes[j].pointers[0].offset = 1;
								inodes[j].access_time = time(0);
								inodes[j].creation_time = time(0);
								inodes[j].modified_time = time(0);
								r->directories[i].files[r->directories[i].num_of_files++].inode_number = j;
								return;

							}
						}
					}
				}
			}
			cout << "Directory doesnt exist" << endl;
		}
		else if (std::regex_match(vec[1], pattern2))
		{
			std::vector<std::string> path = split(vec[1], "/");
			strcpy(r->files[r->num_of_files].name, path[1].c_str());
			int block = 0;
			while (bitmap[block] && block < 4'000'000)
				block++;
			if (block < 4'000'000)
			{
				bitmap[block] = true;
				for (int i = 0; i < 2660; i++)
				{
					if (inodes[i].pointers[0].block_pointer == -1)
					{
						inodes[i].pointers[0].block_pointer = block * 5;
						inodes[i].pointers[0].offset = 1;
						inodes[i].access_time = time(0);
						inodes[i].creation_time = time(0);
						inodes[i].modified_time = time(0);
						r->files[r->num_of_files++].inode_number = i;
						return;

					}
				}
			}
			
		}
		else
			cout << "The syntax of the command is incorrect" << endl;
	}
}
void put(std::vector<std::string> vec, bool* bitmap, root* r, inode* inodes)
{
	if (vec.size() != 3)
		cout << "The syntax of the command is incorrect" << endl;
	else
	{
		std::ifstream a;
		a.open(vec[2], std::ios::binary);

		if (a.is_open())
		{
			a.seekg(0, std::ios::end);
			int size = a.tellg();
			std::string file_content;
			a.seekg(0, std::ios::beg);
			char array[64000] = { 0 };
			a.read(array, 63999);
			file_content.append(array);
			a.close();
			echo(vec, bitmap, r, inodes, file_content);
		}
		else
			cout << "File doesn't exist" << endl;
	}
}
void get(std::vector<std::string> vec, root* r, inode* inodes)
{
	if (vec.size() != 2)
		cout << "The syntax of the command is incorrect" << endl;
	else
	{
		std::string file_content = cat(vec, r, inodes);
		if (file_content.size())
		{
			std::ofstream f;
			f.open("download", std::ios::binary);
			f.write(file_content.c_str(), file_content.size());
			f.close();
		}
	}
}
void ls(std::vector<std::string> vec,root* r)
{
	if (vec.size() != 2)
		cout << "The syntax of the command is incorrect" << endl;
	else
	{
		std::regex pattern("root/[a-z0-9]+$");
		if (std::regex_match(vec[1], pattern))
		{
			std::vector<std::string> path = split(vec[1], "/");
			{
				for (int i = 0; i < r->num_of_dir; i++)
				{
					if (strcmp(r->directories[i].name, path[1].c_str()) == 0)
					{
						for (int j = 0; j < r->directories[i].num_of_files; j++)
						{
							cout << r->directories[i].files[j].name << " ";
						}
						cout << endl;
						return;
					}
				}
			}
		}
		else if (vec[1] == "root")
		{
			for (int i = 0; i < r->num_of_dir; i++)
			{
				cout << r->directories[i].name << " ";
			}
			cout << endl;
			for (int i = 0; i < r->num_of_files; i++)
			{
				cout << r->files[i].name << " ";
			}
			cout << endl;
			return;
		}
		cout << "Directory doesn't exist" << endl;
	}
}
void cp(std::vector<std::string> vec,root* r,inode* inodes,bool* bitmap)
{
	int inode_number = search(r, vec);
	if (vec.size() != 3 || inode_number == -2)
		cout << "The syntax of the command is incorrect" << endl;
	else
	{
		if (inode_number == -1)
		{
			cout << "File doesn't exist" << endl;
			return;
		}
		std::string new_file = vec.back();
		vec.pop_back();
		std::string file_content = cat(vec, r, inodes);
		std::string old_file = vec.back();
		vec.pop_back();
		vec.push_back(new_file);
		vec.push_back(old_file);
		echo(vec, bitmap, r, inodes,file_content);
	}
}
void mv(std::vector<std::string> vec,bool* bitmap,root* r,inode* inodes)
{
	int inode_number = search(r, vec);
	if (vec.size() != 3 || inode_number == -2)
		cout << "The syntax of the command is incorrect" << endl;
	else
	{
		if (inode_number == -1)
		{
			cout << "File doesn't exist" << endl;
			return;
		}
		std::string new_file = vec.back();
		vec.pop_back();
		std::string file_content = cat(vec, r, inodes);
		std::string old_file = vec.back();
		vec.pop_back();
		vec.push_back(new_file);
		vec.push_back(old_file);
		echo(vec, bitmap, r, inodes, file_content);
		vec.pop_back();
		vec.pop_back();
		vec.push_back(old_file);
		rm(vec, r, inodes, bitmap);
		
	}
}
void rename(std::vector<std::string> vec,root* r)
{
	if (vec.size() != 2)
		cout << "The syntax of the command is incorrect" << endl;
	else
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
						{
							std::string s;
							cout << "> ";
							std::getline(std::cin, s);
							strcpy(r->directories[i].files[j].name, s.c_str());
							return;
						}
					}
				}
			}

		}
		else if (std::regex_match(vec[1], pattern2))
		{
			std::vector<std::string> path = split(vec[1], "/");
			for (int i = 0; i < r->num_of_dir; i++)
			{
				if (strcmp(r->directories[i].name, path[1].c_str()) == 0)
				{
					std::string s;
					cout << "> ";
					std::getline(std::cin, s);
					strcpy(r->directories[i].name, s.c_str());
					return;
				}
			}
			for (int i = 0; i < r->num_of_files; i++)
			{
				if (strcmp(r->files[i].name, path[1].c_str()) == 0)
				{
					std::string s;
					cout << "> ";
					std::getline(std::cin, s);
					strcpy(r->files[i].name, s.c_str());
					return;
				}
			}
		}
		cout << "No such file or directory" << endl;
	}
}
void echo(std::vector<std::string> vect, bool* bitmap,root* r,inode* inodes)
{
	int inode_number = search(r, vect);
	if (vect.size() != 2  || inode_number == -2)
		cout << "The syntax of the command is incorrect" << endl;
	else
	{
		if (inode_number == -1)
		{
			create(r, vect, bitmap, inodes);
			inode_number = search(r, vect);
			if (inode_number == -1)
				return;
		}
		std::string s;
		cout << "> ";
		std::getline(std::cin, s);
		if (s.size() == 0)
			s.append(" ");
		int blocks = ceil((double)s.size() / 5);
		std::vector < std::string > vec = Split(s, 5);
		std::ofstream f;
		std::vector<int> indexes;
		f.open("file.bin", std::ios::binary | std::ios::in);
		for (int j = 0; j < blocks; j++)
		{
			for (int i = 0; i < 4'000'000; i++)
			{
				if (!bitmap[i])
				{
					bitmap[i] = true;
					indexes.push_back(i);
					f.seekp(i * 5, std::ios::beg);
					f.write(vec[j].c_str(), vec[j].size());
					break;
				}
			}
		}
		f.close();
		int position = 0;
		inodes[inode_number].access_time = time(0);
		inodes[inode_number].modified_time = time(0);
		while (inodes[inode_number].pointers[position].block_pointer != -1)
			position++;
		if (blocks == 1)
		{
			inodes[inode_number].pointers[position].block_pointer = indexes.at(0) * 5;
			inodes[inode_number].pointers[position].offset++;
			return;
		}
		for (int i = 0; i < blocks-1; i++)
		{
			if (inodes[inode_number].pointers[position].block_pointer == -1)
			{
				inodes[inode_number].pointers[position].block_pointer = indexes.at(i) * 5;
				inodes[inode_number].pointers[position].offset++;
			}
			if ((indexes.at(i + 1) - indexes.at(i)) == 1)
			{
				inodes[inode_number].pointers[position].offset++;
			}
			else
				position++;
		}
		if((indexes.at(blocks-1)- indexes.at(blocks-2))>1)
			inodes[inode_number].pointers[position].block_pointer = indexes.at(blocks-1) * 5;
	}
}
std::string cat(std::vector<std::string> vect,root* r,inode* inodes)
{
	int inode_number = search(r, vect);
	if (vect.size() != 2 || inode_number == -2)
	{
		cout << "The syntax of the command is incorrect" << endl;
		return "";
	}
	else
	{
		if (inode_number == -1)
		{
			cout << "File doesn't exist" << endl;
			return "";
		}
		std::string read, temp;
		std::ifstream a;
		cout << endl;
		a.open("file.bin", std::ios::binary | std::ios::in);
		inodes[inode_number].access_time = time(0);
		int i = 1;
		while (inodes[inode_number].pointers[i].block_pointer != -1)
		{
			int remaining_blocks = 0;
			for (; remaining_blocks < inodes[inode_number].pointers[i].offset; remaining_blocks++)
			{
				a.seekg(inodes[inode_number].pointers[i].block_pointer + remaining_blocks*5, std::ios::beg);
				char c[6] = { 0 };
				a.read(c, 5);
				for (int j = 0; j < 5; j++)
				{
					if (c[j] != '\0')
					{
						temp.clear();
						temp.append(c + j, 1);
						read.append(temp);
					}
				}
			}
			i++;
		}
		a.close();
		cout << read << endl;
		return read;
	}
}
void rm(std::vector<std::string> vec,root *r,inode* inodes,bool* bitmap)
{
	if (vec.size() != 2 && vec.size() != 3)
		cout << "The syntax of the command is incorrect" << endl;
	else
	{
		if (vec[1] == "-r")
		{
			std::regex pattern("root/[a-z0-9]+$");
			if (std::regex_match(vec[2], pattern))
			{
				std::vector<std::string> path = split(vec[2], "/");
				for (int i = 0; i < r->num_of_dir; i++)
				{
					if (strcmp(r->directories[i].name, path[1].c_str()) == 0)
					{
						for (int j = 0; j < r->directories[i].num_of_files; j++)
						{
							int inode_number = r->directories[i].files[j].inode_number;
							r->directories[i].files[j].inode_number = -1;
							strcpy(r->directories[i].files[j].name, "");
							free_inode(inodes, bitmap, inode_number);
						}
						int k = i + 1;
						r->directories[i].num_of_files = r->directories[k].num_of_files;
						strcpy(r->directories[i].name, r->directories[k].name);
						for (int count = 0; count < r->directories[k].num_of_files; count++)
						{
							r->directories[i].files[count].inode_number = r->directories[k].files[count].inode_number;
							strcpy(r->directories[i].files[count].name, r->directories[k].files[count].name);
						}
						while (k < r->num_of_dir)
						{
							for (int count = 0; count < r->directories[k].num_of_files; count++)
							{
								r->directories[k].files[count].inode_number = -1;
								strcpy(r->directories[k].files[count].name, "");
							}
							for (int count = 0; count < r->directories[k + 1].num_of_files; count++)
							{
								r->directories[k].files[count].inode_number = r->directories[k + 1].files[count].inode_number;
								strcpy(r->directories[k].files[count].name, r->directories[k + 1].files[count].name);
							}
							strcpy(r->directories[k].name, r->directories[k + 1].name);
							r->directories[k].num_of_files = r->directories[k + 1].num_of_files;
							k++;
						}
						r->num_of_dir--;
						return;
					}
				}
			}
			cout << "Directory doesnt exist" << endl;
			return;
		}
		else
		{
			int inode_number = search(r, vec);
			if (inode_number < 0)
			{
				cout << "File doesn't exist" << endl;
				return;
			}
			std::regex pattern("root/[a-z0-9]+$");
			if (std::regex_match(vec[1], pattern))
			{
				for (int i = 0; i < r->num_of_files; i++)
				{
					if (r->files[i].inode_number == inode_number)
					{
						strcpy(r->files[i].name, r->files[i + 1].name);
						r->files[i].inode_number = r->files[i + 1].inode_number;
						int j = i + 1;
						while (r->files[j].inode_number == -1)
						{
							strcpy(r->files[j].name, r->files[j + 1].name);
							r->files[j].inode_number = r->files[j + 1].inode_number;
							j++;
						}
						free_inode(inodes, bitmap, inode_number);
						r->num_of_files--;
						return;
					}
				}
			}
			else
			{
				std::vector<std::string> path = split(vec[1], "/");
				for (int i = 0; i < r->num_of_dir; i++)
				{
					if (strcmp(r->directories[i].name, path[1].c_str()) == 0)
					{
						for (int j = 0; j < r->directories[i].num_of_files; j++)
						{
							if (r->directories[i].files[j].inode_number == inode_number)
							{
								strcpy(r->directories[i].files[j].name, r->directories[i].files[j + 1].name);
								r->directories[i].files[j].inode_number = r->directories[i].files[j + 1].inode_number;
								int k = j + 1;
								while (r->directories[i].files[k].inode_number == -1)
								{
									strcpy(r->directories[i].files[k].name, r->directories[i].files[k + 1].name);
									r->directories[i].files[k].inode_number = r->directories[i].files[k + 1].inode_number;
									k++;
								}
								free_inode(inodes, bitmap, inode_number);
								r->directories[i].num_of_files--;
								return;
							}
						}
					}
				}
			}
		}
	}
}
void stat(std::vector<std::string> vec,root* r,inode* inodes)
{
	if (vec.size() != 2)
		cout << "The syntax of the command is incorrect" << endl;
	else
	{
		int inode_number = search(r, vec);
		if (inode_number < 0)
		{
			cout << "File doesn't exist" << endl;
			return;
		}
		cout << "Inode: " << inode_number << endl;
		char* dt = ctime(&inodes[inode_number].creation_time);
		cout << "Creation: " << dt;
		dt = ctime(&inodes[inode_number].access_time);
		cout << "Access: " << dt;
		dt = ctime(&inodes[inode_number].modified_time);
		cout << "Modify: " << dt;
		cout << "Blocks: ";
		int size = 0;
		for (int i = 0; inodes[inode_number].pointers[i].block_pointer != -1 && i < 15; i++)
		{
			int count = 0;
			while (count < inodes[inode_number].pointers[i].offset)
			{
				cout << inodes[inode_number].pointers[i].block_pointer / 5 + count << " ";
				count++;
				size++;
			}
		}
		cout << endl;
		cout << "Size: " << size * 5 << "B" << endl;
	}
}

void echo(std::vector<std::string> vect, bool* bitmap,root* r,inode* inodes,std::string s)
{
	int inode_number = search(r, vect);
	if (vect.size() != 3  || inode_number == -2)
		cout << "The syntax of the command is incorrect" << endl;
	else
	{
		if (inode_number == -1)
		{
			create(r, vect, bitmap, inodes);
			inode_number = search(r, vect);
			if (inode_number == -1)
				return;
		}
		if (s.size() == 0)
			s.append(" ");
		int blocks = ceil((double)s.size() / 5);
		std::vector < std::string > vec = Split(s, 5);
		std::ofstream f;
		std::vector<int> indexes;
		f.open("file.bin", std::ios::binary | std::ios::in);
		for (int j = 0; j < blocks; j++)
		{
			for (int i = 0; i < 4'000'000; i++)
			{
				if (!bitmap[i])
				{
					bitmap[i] = true;
					indexes.push_back(i);
					f.seekp(i * 5, std::ios::beg);
					f.write(vec[j].c_str(), vec[j].size());
					break;
				}
			}
		}
		f.close();
		int position = 0;
		inodes[inode_number].access_time = time(0);
		inodes[inode_number].modified_time = time(0);
		while (inodes[inode_number].pointers[position].block_pointer != -1)
			position++;
		if (blocks == 1)
		{
			inodes[inode_number].pointers[position].block_pointer = indexes.at(0) * 5;
			inodes[inode_number].pointers[position].offset++;
			return;
		}
		for (int i = 0; i < blocks-1; i++)
		{
			if (inodes[inode_number].pointers[position].block_pointer == -1)
			{
				inodes[inode_number].pointers[position].block_pointer = indexes.at(i) * 5;
				inodes[inode_number].pointers[position].offset++;
			}
			if ((indexes.at(i + 1) - indexes.at(i)) == 1)
			{
				inodes[inode_number].pointers[position].offset++;
			}
			else
				position++;
		}
		if((indexes.at(blocks-1)- indexes.at(blocks-2))>1)
			inodes[inode_number].pointers[position].block_pointer = indexes.at(blocks-1) * 5;
	}
}