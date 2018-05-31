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

#include "mfs.h"

int main(int argc, char * argv[])
{
	/*
	if (argc != 3) {
		fprintf(stderr, "Incorrect number of arguments passed. Only specify port number and file system image\n");
		exit(EXIT_FAILURE);
	}
	
	// exactly 3 arguments (default one plus port number and file system image)
	char * port_num_string = argv[1];
	char * ptr;
	long port_number = strtol(port_num_string, &ptr, 10);
	if (port_number == 0) {
		fprintf(stderr, "Incorrect port number format. Must be a positive integer.\n");
		exit(EXIT_FAILURE);
	}
	
	char * file_system_image = argv[2];
	 */
	
	long port_number = 8080;
	char file_system_image[] = "taco";
	
	printf("Port Number: %ld, File System Image: %s\n", port_number, file_system_image);
	
	int mfs_ret = MFS_Init(file_system_image, port_number);
	
	return 0;
}
