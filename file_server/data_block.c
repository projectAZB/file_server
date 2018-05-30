//
//  data_block.c
//  file_server
//
//  Created by Adam on 5/29/18.
//  Copyright © 2018 Adam. All rights reserved.
//

#include "data_block.h"

#include <stdlib.h>

data_block_handle create_data_block()
{
	data_block_handle new = (data_block_handle)malloc(*new);
	return new;
}

void destroy_data_block(data_block_handle data_block)
{
	free(data_block);
}
