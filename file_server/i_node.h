//
//  i_node.h
//  file_server
//
//  Created by Adam on 5/29/18.
//  Copyright © 2018 Adam. All rights reserved.
//

#ifndef i_node_h
#define i_node_h

#include <stdio.h>

#include "data_block.h"
#include "constants.h"

#define NUM_DATA_BLOCK_POINTERS (int)10

typedef struct i_node_t i_node_t;

typedef i_node_t * i_node_handle;

struct i_node_t {
	file_t file_type;
	//for directories
	size_t num_inodes;
	i_node_handle * i_nodes;
	//for regular files
	size_t num_blocks;
	data_block_handle data_blocks[NUM_DATA_BLOCK_POINTERS]; //block of data blocks for reg file (each 4096 in size)
	int bitmap[NUM_DATA_BLOCK_POINTERS]; //bitmap for above data block if regular file
};

i_node_handle create_i_node(void);
void destroy_i_node(i_node_handle i_node);

void add_i_node(i_node_handle parent_i_node, i_node_handle new_i_node);

#endif
