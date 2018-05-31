//
//  inode_table.h
//  file_server
//
//  Created by Adam on 5/30/18.
//  Copyright © 2018 Adam. All rights reserved.
//

#ifndef inode_table_h
#define inode_table_h

#include "constants.h"

#include "inode.h"

typedef struct inode_table_t inode_table_t;

struct inode_table_t
{
	unsigned int next_free; //also used to set inums
	int inode_bitmap[NUM_INODES];
	inode_t inodes[NUM_INODES]; //will be 4096
};

void print_inode_table(inode_table_t inode_table)
{
	printf(" first_free: %u, first_inode_name: %s, first_inode_inum: %d", inode_table.next_free, inode_table.inodes[0].name, inode_table.inodes[0].inum);
}

#endif
