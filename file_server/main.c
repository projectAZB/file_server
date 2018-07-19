//
//  main.c
//  file_server
//
//  Created by Adam on 5/28/18.
//  Copyright © 2018 Adam. All rights reserved.
//

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <assert.h>
#include <stdbool.h>

#include "mfs.h"

int main(int argc, char * argv[])
{
	MFS_Init("localhost", 10003);
	
	// /dir1
	int rc;
	
	rc = MFS_Unlink(0, "dir1");
	if (rc == -1) assert(false);
	
	return 0;
}
