//
//  inode.h
//  file_server
//
//  Created by Adam on 5/30/18.
//  Copyright © 2018 Adam. All rights reserved.
//

#ifndef inode_h
#define inode_h

typedef struct inode_t inode_t;

struct inode_t
{
	int parentSelfDirs[2];
	int inum;
	char name[252];
	int file_type;
	size_t size;
	//for directories
	size_t dir_num_inodes;
	int dir_child_inums[50];
	//for regular files
	size_t reg_num_blocks;
	int reg_block_offset;
};

void print_inode(inode_t inode) {
	printf(" inum: %d\n name: %s\n file type: %d\n size: %lu\n num_inodes: %lu\n num_blocks: %lu block_offset: %d\n", inode.inum, inode.name, inode.file_type, inode.size, inode.dir_num_inodes, inode.reg_num_blocks, inode.reg_block_offset);
}

#endif 
