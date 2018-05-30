//
//  i_node_table.h
//  file_server
//
//  Created by Adam on 5/29/18.
//  Copyright © 2018 Adam. All rights reserved.
//

#ifndef i_node_table_h
#define i_node_table_h

#include "constants.h"

#include "i_node.h"

typedef struct i_node_table_t i_node_table_t;

typedef i_node_table_t * i_node_table_handle;

struct i_node_table_t {
	int i_node_bitmap[NUM_INODES];
	i_node_handle i_nodes[NUM_INODES]; //will be 4096
};

i_node_table_handle create_i_node_table(void);
void destroy_i_node_table(i_node_table_handle i_node_table);

#endif 
