//
//  data_block.h
//  file_server
//
//  Created by Adam on 5/29/18.
//  Copyright © 2018 Adam. All rights reserved.
//

#ifndef data_block_h
#define data_block_h

#include "constants.h"

typedef unsigned char byte;

typedef struct data_block_t {
	byte bytes[MFS_BLOCK_SIZE];
} data_block_t;

#endif
