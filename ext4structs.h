#pragma once
#include <iostream>

struct super_block{
	int blocks_count = 4'000'000;
	int free_blocks_count = 4'000'000;
	short inodes_count = 2600;
	short free_inodes_count = 2600;
	short block_size = 5;
};
struct dir_entry {
	char name[50];
	int inode_number = -1;
};
struct dir
{
	char name[50];
	dir_entry files[50];
	int num_of_files = 0;
};

struct pointer {
	int block_pointer = -1;
	int offset = 0;
};
struct inode {

	time_t modified_time = 0;
	time_t creation_time = 0;
	time_t access_time = 0;
	pointer pointers[15];
};

struct root {
	
	dir directories[50];
	dir_entry files[100];
	int num_of_dir = 0;
	int num_of_files = 0;
};