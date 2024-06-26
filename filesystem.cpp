#include <string>
#include <math.h>
#include "commands.h"
#include <fstream>

using std::cout;
using std::endl;
using std::ofstream;
using std::ifstream;

int main()
{
	super_block sblock;
	root root_dir;
	inode inodes[2600];
	bool bitmap[4'000'000] = { false };
	if (!DoesFileExist("file.bin"))
	{
		int bitmap_allocation = ceil((sizeof(root_dir) + sizeof(bitmap) + sizeof(sblock) + sizeof(inodes))/(double)sblock.block_size);
		for (int i = 0; i < bitmap_allocation; i++)
		{
			bitmap[i] = true;
		}
		ofstream f;
		f.open("file.bin", std::ios::binary);
		f.write((char*)&sblock, sizeof(sblock));
		f.write((char*)bitmap, sizeof(bitmap));
		f.write((char*)&root_dir, sizeof(root_dir));
		f.write((char*)inodes, sizeof(inodes));
		f.close();
	}
	else
	{
		ifstream a;
		a.open("file.bin", std::ios::binary);
		a.read((char*)&sblock, sizeof(sblock));
		a.read((char*)bitmap, sizeof(bitmap));
		a.read((char*)&root_dir, sizeof(root_dir));
		a.read((char*)inodes, sizeof(inodes));
		a.close();
	}

	std::string s;
	bool flag = true;
	while (flag)
	{
		std::getline(std::cin, s);
		std::vector<std::string> vec = split(s, " ");
		if (vec.size() == 0)
			cout << "No command" << endl;
		else if (vec[0] == "mkdir")
		{
			mkdir(&root_dir, vec);
		}
		else if (vec[0] == "create")
		{
			create(&root_dir, vec, bitmap, inodes);
		}
		else if (vec[0] == "echo")
		{
			echo(vec, bitmap, &root_dir, inodes);
		}
		else if (vec[0] == "cat")
		{
			cat(vec, &root_dir, inodes);
		}
		else if (vec[0] == "put")
		{
			put(vec,bitmap,&root_dir,inodes);
		}
		else if (vec[0] == "get")
		{
			get(vec,&root_dir,inodes);
		}
		else if (vec[0] == "rm")
		{
			rm(vec, &root_dir, inodes,bitmap);
		}
		else if (vec[0] == "ls")
		{
			ls(vec,&root_dir);
		}
		else if (vec[0] == "cp")
		{
			cp(vec,&root_dir,inodes,bitmap);
		}
		else if (vec[0] == "mv")
		{
			mv(vec,bitmap,&root_dir,inodes);
		}
		else if (vec[0] == "rename")
		{
			rename(vec,&root_dir);
		}
		else if (vec[0] == "stat")
		{
			stat(vec,&root_dir,inodes);
		}
		else if (vec[0] == "exit")
			flag = false;
		else
			cout << "Unrecognised command" << endl;
	}
	ofstream f;
	f.open("file.bin", std::ios::binary | std::ios::in);
	f.seekp(0, std::ios::beg);
	f.write((char*)&sblock, sizeof(sblock));
	f.write((char*)bitmap, sizeof(bitmap));
	f.write((char*)&root_dir, sizeof(root_dir));
	f.write((char*)inodes, sizeof(inodes));
	f.close();
    return 0;
}