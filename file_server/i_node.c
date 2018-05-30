//
//  i_node.c
//  file_server
//
//  Created by Adam on 5/29/18.
//  Copyright © 2018 Adam. All rights reserved.
//

#include "i_node.h"

#include <stdlib.h>

i_node_handle create_i_node(void)
{
	i_node_handle new = (i_node_handle)malloc(sizeof(*new));
	new->num_inodes = 0;
	new->num_blocks = 0;
	new->i_nodes = (i_node_handle *)malloc(sizeof(i_node_handle));
	return new;
}

void destroy_i_node(i_node_handle i_node)
{
	int num_inodes = i_node->num_inodes;
	for (int i = 0; i < num_inodes; i++)
	{
		//recursive delete, bottoms out at 0
		destroy_i_node(i_node);
	}
	free(i_node->i_nodes);
	free(i_node);
}

void add_i_node(i_node_handle parent_i_node, i_node_handle new_i_node)
{
	(parent_i_node->num_inodes)++;
	parent_i_node->i_nodes = (i_node_handle *)realloc(parent_i_node->i_nodes, (parent_i_node->num_inodes * sizeof(i_node_handle));
	parent_i_node->i_nodes[parent_i_node->num_inodes - 1] = new_i_node;
}
