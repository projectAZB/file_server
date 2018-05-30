//
//  mfs.c
//  file_server
//
//  Created by Adam on 5/28/18.
//  Copyright © 2018 Adam. All rights reserved.
//

#include "mfs.h"
#include <fcntl.h>
#include <errno.h>

int MFS_Init(char * hostname, int port)
{
	int open_status = open(hostname, O_RDWR);
	if (open_status == -1) { //file image doesn't exist, create
		printf("File Image doesn't exist");
		return -1;
	}
	else { //file image does exist
		printf("File Image does exist");
		return 0;
	}
	
}
