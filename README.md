
## Overview
A simplified ext4-like filesystem, written in C++. This file system uses some of the data structures used in file systems, such as:
- Index nodes
- Super block
- Bitmaps and others.


## How it works
This implementation uses a single binary file, which is used as a container for the file system. The binary file is treated just like a hard drive would be treated by a file system - it gets split into blocks, each with the same size, which allows for allocation/deletion and segmentation of the disk for the different types of data that need to be stored.

### File structures

#### Super block
The super block structure contains metadata about the file system. The system can refer to it for any information about itself, such as:
- The amount of bytes each block takes
- The total amount of blocks in the container
- The number of inodes

#### Bitmaps
Bitmaps serve as a way to map bits to blocks. For the needs of the system, 1 means the block is allocated (taken), 0 means the block is unallocated (free).

#### Index nodes
Index nodes are a combination of metadata about a file (or directory, they are also treated as files) and a collection of pointers to blocks in the data block region. Each pointer in the direct block list points to a data block on the disk. Whenever a user wants, for example, to read a file, the file system takes all the blocks and concatenates them together to represent the file, which isn't actually saved contiguously. 



### File system commands

#### Command reference
The system supports some of the basic Unix file system commands, such as:
-  `mkdir` - creates a new directory
-  `ls` - lists the content of a directory
-  `cp` - copies a file
-  `mv` - moves a file
-  `rm` - removes a file
-  `cat` - shows the content of a file
-  `create` - creates a new file
-  `put` - imports a file from the user's machine
-  `get` - exports a file from the simulated file system to the user's machine
-  `stat` - displays information about the specified file
-  `echo` - writes to a file
-  `rename` - renames a file


