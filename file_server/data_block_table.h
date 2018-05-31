//
//  data_block_table.h
//  file_server
//
//  Created by Adam on 5/29/18.
//  Copyright © 2018 Adam. All rights reserved.
//

#ifndef data_block_table_h
#define data_block_table_h

#include "constants.h"
#include "data_block.h"

typedef struct data_block_table_t data_block_table_t;

struct data_block_table_t {
	unsigned int next_free;
	int data_block_bitmap[NUM_BLOCKS];
	data_block_t blocks[NUM_BLOCKS]; //will be 4096
};

#endif 
