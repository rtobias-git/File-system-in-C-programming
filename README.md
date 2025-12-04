File System Implementation

Operating Systems 2

You are required to implement a shell in the C language (command interpreter) that supports the following commands:

Create a directory (within the current directory):
mkdir <directory_name>

Example:
$ mkdir users

Change the current directory:
cd <path>

Examples:
$ cd /
$ cd users
$ cd ..
$ cd /test

Display the current directory:
pwd

Example:
$ pwd
/users

Create a file (in the current directory):
touch <filename>
<file_content>
<CTRL+D>

Example:
$ touch test.txt
But see here!
<CTRL+D>

Display the content of a file (in the current directory):
cat <filename>

Example:
$ cat test.txt
But see here!

List all files in the current directory:
ls

Example:
$ ls
d - 1     .                   256
d - 1     ..                  256
f - 2     file.txt            20
d - 3     folder              256

Explanation of attributes displayed:
file type (d = directory | f = file) - inode number
file name
file size in Bytes

Remove a file or directory (from the current directory):
rm <filename_or_directory>

Example:
$ rm file.txt

Display the current status of the file system:
stat

Example:
$ stat
Free space: 10240 Bytes
Free blocks: 80 Blocks
Block size: 128 Bytes

These commands should perform data persistence in a file system developed by you. 
This file system must use the file system of the operating system of your choice, meaning it should not directly access the hardware.
All storage for your file system should reside within the fs (filesystem) directory, which must be organized as follows:

fs/blocks (directory) or blocks.dat (file)
fs/inodes.dat
fs/freespace.dat
fs/superblock.dat

Considering the proposed organization, the superblock.dat file should store the file system characteristics (the quantities must be defined by you), as follows:
filesystem=kleberfs
blocksize=128
partitionsize=10240

Each data block can be represented as a file inside the blocks directory (e.g., block 20 would be the file 20.dat) and must support the data size defined in the blocksize attribute of the superblock.dat file.
Another option is to represent the blocks as a single blocks.dat file, where every X bytes (X being the block size) store the data of a given block. The file system must create all data blocks (as free) before its first use and cannot exceed the block limit supported by the partition,
which is defined in the partitionsize attribute of the superblock.dat file.

Every directory must always contain the directories (.) and (..) to indicate the current directory and the parent directory, respectively.
The directory entries should follow the format:
--------------------------------------------------------------
| file_name (max. 14 bytes) | inode number (8 bits) |
--------------------------------------------------------------

Inodes must have the same size and be stored sequentially in the inodes.dat file.
All inodes must be created (as free) before the first use of the file system (except the inode related to the root directory). 
The free block management structure should be defined by you and stored in the freespace.dat file, where, upon the first use of the file system,
all blocks should be marked as free (except for the block where the root directory is stored). It is important to note that all data stored in your file system must be accessible the next time the file system is used.
The evaluation of the project will be done based on the implementation presentation in class and a document describing the details of the implemented file system.
The document must also include details on how the directories were structured to provide good search performance for a file (do not use sequential or exhaustive search) and how the free space management structure was organized to find free blocks more quickly.
It is important that these design choices for the file system project are justified and well-founded.
