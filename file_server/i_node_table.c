//
//  i_node_table.c
//  file_server
//
//  Created by Adam on 5/29/18.
//  Copyright © 2018 Adam. All rights reserved.
//

#include "i_node_table.h"

#include <stdlib.h>

i_node_table_handle create_i_node_table(void)
{
	i_node_table_handle new = (i_node_table_handle)malloc(sizeof(*new));
	memset(new->i_nodes, 0, NUM_INODES); //null out all the references
	memset(new->i_node_bitmap, 0, NUM_INODES); //zero out the inode bitmap (all empty)
	return new;
}

void destroy_i_node_table(i_node_table_handle i_node_table)
{
	free(i_node_table);
}
