//
//  constants.h
//  file_server
//
//  Created by Adam on 5/29/18.
//  Copyright © 2018 Adam. All rights reserved.
//

#ifndef constants_h
#define constants_h

#include <stdio.h>

#define MFS_BLOCK_SIZE (int)4096

#define NUM_INODES (int)4096
#define NUM_BLOCKS (int)4096

#define CLIENT_TIMEOUT (int)5

typedef enum file_t {
	MFS_DIRECTORY = 0,
	MFS_REGULAR_FILE = 1
} file_t;

#endif
