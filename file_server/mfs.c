//
//  mfs.c
//  file_server
//
//  Created by Adam on 5/28/18.
//  Copyright © 2018 Adam. All rights reserved.
//

#include "mfs.h"

#include <fcntl.h>
#include <unistd.h>
#include <errno.h>
#include <string.h>
#include <stdlib.h>

#include "inode_table.h"
#include "data_block_table.h"

void init_reg_inode(inode_t * inode, int inum, int parent, int self, char * name, size_t size, size_t offset)
{
	inode->inum = inum;
	inode->size = size;
	inode->parentSelfDirs[0] = parent;
	inode->parentSelfDirs[1] = self;
	strncpy(inode->name, name, strlen(name) + 1);
	inode->file_type = MFS_REGULAR_FILE;
	inode->reg_num_blocks = (size / MFS_BLOCK_SIZE) + 1;
	inode->reg_block_offset = offset;
}

void init_dir_inode(inode_t * inode, int inum, int parent, int self, char * name)
{
	inode->inum = inum;
	inode->size = 0;
	inode->parentSelfDirs[0] = parent;
	inode->parentSelfDirs[1] = self;
	strncpy(inode->name, name, strlen(name) + 1);
	inode->file_type = MFS_DIRECTORY;
	inode->dir_num_inodes = 0; //set 0 to start
	memset(inode->dir_child_inums, -1, 50); //-1 all entries
}

int MFS_Init(char * hostname, int port)
{
	int open_fd = open(hostname, O_RDWR);
	if (open_fd == -1) {
		printf("File doesn't exist\n");
		int create_fd = open(hostname, O_RDWR | O_CREAT, S_IRUSR | S_IWUSR);
		if (create_fd == -1) { //error creating the file
			return -1;
		}
		
		inode_t root_inode;
		init_dir_inode(&root_inode, 0, 0, 0, "/");
		
		//create the inode table, put root in there
		inode_table_t inode_table;
		memset(inode_table.inode_bitmap, 0, NUM_INODES);
		inode_table.next_free = 0;
		inode_table.inode_bitmap[inode_table.next_free] = 1;
		inode_table.inodes[inode_table.next_free] = root_inode; //set root inode to be the first inode
		inode_table.next_free = 1; //root takes up the first one now
		if (write(create_fd, &inode_table, sizeof(inode_table_t)) == -1) {
			perror("Error writing inode table");
			return -1;
		}
		
		//create the data block table
		data_block_table_t * data_block_table = (data_block_table_t *)malloc(sizeof(data_block_table_t));
		data_block_table->next_free = 0;
		memset(data_block_table->data_block_bitmap, 0, NUM_BLOCKS);
		
		if (write(create_fd, data_block_table, sizeof(data_block_table_t)) == - 1) {
			perror("Error writing data block table");
			return -1;
		}
		
		return 0;
	}
	else {
		//read in the inode table
		inode_table_t inode_table;
		if (read(open_fd, &inode_table, sizeof(inode_table)) == -1) {
			perror("Error reading the inode table in");
			return -1;
		}
		print_inode(inode_table.inodes[0]);
		
		//read in the data block table
		data_block_table_t * data_table = (data_block_table_t *)malloc(sizeof(data_block_table_t));
		if (read(open_fd, data_table, sizeof(data_block_table_t)) == -1) {
			perror("Error reading the data table in");
			return -1;
		}
		
		return 0;
	}
}
