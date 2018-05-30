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

typedef struct __MFS_Stat_t {
	file_t type;   // MFS_DIRECTORY or MFS_REGULAR
	size_t size;   // bytes
	int blocks; // number of blocks allocated to file
	// note: no permissions, access times, etc.
} MFS_Stat_t;

typedef MFS_Stat_t * MFS_Stat_handle;

typedef struct __MFS_DirEnt_t {
	int  inum;      // inode number of entry (-1 means entry not used)
	char name[252]; // up to 252 bytes of name in directory (including \0)
} MFS_DirEnt_t;

#endif
