//
//  data_block_table.c
//  file_server
//
//  Created by Adam on 5/29/18.
//  Copyright © 2018 Adam. All rights reserved.
//

#include "data_block_table.h"

#include <stdlib.h>

data_block_table_handle create_data_block_table(void)
{
	data_block_table_handle new = (data_block_table_handle)malloc(sizeof(*new));
	new->first_free = 0;
	memset(new->blocks, 0, NUM_BLOCKS); //null out the references
	memset(new->data_block_bitmap, 0, NUM_BLOCKS); //zero out the bitmap (all empty)
	return new;
}

void destroy_data_block_table(data_block_table_handle data_block_table)
{
	unsigned int first_free = data_block_table->first_free;
	for (int i = 0; i < first_free; i++)
	{
		destroy_data_block(data_block_table->blocks[i]);
	}
	free(data_block_table);
}
