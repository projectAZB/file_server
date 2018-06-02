//
//  mfs.c
//  file_server
//
//  Created by Adam on 5/28/18.
//  Copyright © 2018 Adam. All rights reserved.
//

#include "mfs.h"

#include <fcntl.h>
#include <unistd.h>
#include <errno.h>
#include <string.h>
#include <stdlib.h>
#include <assert.h>
#include <stdbool.h>

#include "inode_table.h"
#include "data_block_table.h"

int _file_image_fd;
data_block_table_t * _data_block_table;
inode_table_t _inode_table;

void init_reg_inode(inode_t * inode, int inum, int parent, int self, char * name)
{
	inode->inum = inum;
	inode->size = 0;
	inode->parentSelfDirs[0] = parent;
	inode->parentSelfDirs[1] = self;
	strncpy(inode->name, name, strlen(name) + 1);
	inode->file_type = MFS_REGULAR_FILE;
	inode->reg_num_blocks = 0;
	inode->reg_block_offset = -1;
	inode->dir_num_inodes = 0; //set 0 to start
	memset(inode->dir_child_inums, -1, MAX_INODES); //-1 all entries
}

void init_dir_inode(inode_t * inode, int inum, int parent, int self, char * name)
{
	inode->inum = inum;
	inode->size = 0;
	inode->parentSelfDirs[0] = parent;
	inode->parentSelfDirs[1] = self;
	strncpy(inode->name, name, strlen(name) + 1);
	inode->file_type = MFS_DIRECTORY;
	inode->dir_num_inodes = 0; //set 0 to start
	memset(inode->dir_child_inums, -1, MAX_INODES); //-1 all entries
	inode->reg_block_offset = -1;
	inode->reg_num_blocks = 0;
}

int MFS_Init(char * hostname, int port)
{
	_file_image_fd = open(hostname, O_RDWR);
	if (_file_image_fd == -1) {
		_file_image_fd = open(hostname, O_RDWR | O_CREAT, S_IRUSR | S_IWUSR);
		if (_file_image_fd == -1) { //error creating the file
			return -1;
		}
		
		inode_t root_inode;
		init_dir_inode(&root_inode, 0, 0, 0, "/");
		
		//create the inode table, put root in there
		memset(_inode_table.inode_bitmap, 0, NUM_INODES);
		_inode_table.next_free = 0;
		_inode_table.inode_bitmap[_inode_table.next_free] = 1;
		_inode_table.inodes[_inode_table.next_free] = root_inode; //set root inode to be the first inode
		_inode_table.next_free = 1; //root takes up the first one now
		if (write(_file_image_fd, &_inode_table, sizeof(inode_table_t)) == -1) {
			perror("Error writing inode table");
			return -1;
		}
		
		//create the data block table
		_data_block_table = (data_block_table_t *)malloc(sizeof(data_block_table_t));
		_data_block_table->next_free = 0;
		memset(_data_block_table->data_block_bitmap, 0, NUM_BLOCKS);
		
		if (write(_file_image_fd, _data_block_table, sizeof(data_block_table_t)) == - 1) {
			perror("Error writing data block table");
			return -1;
		}
		
		fsync(_file_image_fd);
		
		return 0;
	}
	else {
		//read in the inode table
		if (read(_file_image_fd, &_inode_table, sizeof(inode_table_t)) == -1) {
			perror("Error reading the inode table in");
			return -1;
		}
		print_inode(_inode_table.inodes[0]);
		
		//read in the data block table
		_data_block_table = (data_block_table_t *)malloc(sizeof(data_block_table_t));
		if (read(_file_image_fd, _data_block_table, sizeof(data_block_table_t)) == -1) {
			perror("Error reading the data table in");
			return -1;
		}
		
		return 0;
	}
}

int r_search(inode_t inode, int pinum, char * name)
{
	if (inode.file_type == MFS_DIRECTORY) { //it's a directory
		if (inode.inum == pinum) { //this is the parent directory
			//search through the children to see if there's any response
			for (int i = 0; i < inode.dir_num_inodes; i++)
			{
				int child_inum = inode.dir_child_inums[i];
				if (_inode_table.inode_bitmap[child_inum] <= 0) {
					continue;
				}
				inode_t child_inode = _inode_table.inodes[child_inum];
				if (strncmp(child_inode.name, name, strlen(name)) == 0) { //correct filename
					return child_inode.inum;
				}
			}
			return -1; //pinum matches, but no files with the same name
		}
		
		//if file isn't found, recursively search directory children
		for (int i = 0; i < inode.dir_num_inodes; i++)
		{
			int child_inum = inode.dir_child_inums[i];
			if (_inode_table.inode_bitmap[child_inum] <= 0) {
				continue;
			}
			inode_t child_inode = _inode_table.inodes[child_inum];
			if (child_inode.file_type == MFS_DIRECTORY) { //if directory, recursively search
				return r_search(child_inode, pinum, name);
			}
		}
	}
	else {	//should never be called on a non-directory file
		assert(false);
		return -1;
	}
	
	return -1;
}

int MFS_Lookup(int pinum, char * name)
{
	if (pinum >= _inode_table.next_free || pinum < 0) {
		return -1;
	}
	
	if (_inode_table.inode_bitmap[pinum] <= 0) {
		return -1;
	}
	
	if (strcmp(name, "..") == 0) { //parent directory
		inode_t parent = _inode_table.inodes[pinum];
		//get the parent inum, then inode
		int parent_inum = parent.parentSelfDirs[0];
		return parent_inum;
	}
	
	if (strcmp(name, ".") == 0) { //self directory
		inode_t parent = _inode_table.inodes[pinum];
		//get the self inum, then inode
		int self_inum = parent.parentSelfDirs[1];
		return self_inum;
	}
	
	inode_t root = _inode_table.inodes[0];
	int ret = r_search(root, pinum, name);
	return ret;
}

int MFS_Stat(int inum, MFS_Stat_t * m)
{
	if (inum < 0 || inum >= _inode_table.next_free) {
		return -1;
	}
	
	//if it was deleted or never created, return error
	if (_inode_table.inode_bitmap[inum] <= 0) {
		return -1;
	}
	inode_t inode = _inode_table.inodes[inum];
	if (inode.file_type == MFS_DIRECTORY) {
		m->blocks = 1;
		int num_nodes = inode.dir_num_inodes + 2;
		m->size = num_nodes * sizeof(MFS_DirEnt_t);
		m->type = MFS_DIRECTORY;
	}
	else { // inode.file_type == MFS_REGULAR_FILE
		m->blocks = inode.reg_num_blocks;
		m->size = MFS_BLOCK_SIZE * inode.reg_num_blocks;
		m->type = MFS_REGULAR_FILE;
	}
	return 0;
}

int MFS_Creat(int pinum, int type, char * name)
{
	if (pinum >= _inode_table.next_free || pinum < 0) {
		return -1;
	}
	
	if (_inode_table.inode_bitmap[pinum] == -1) {
		return -1;
	}
	inode_t parent_inode = _inode_table.inodes[pinum];
	if (parent_inode.file_type == MFS_REGULAR_FILE) { //can't add children to a regular file
		return -1;
	}
	if (_inode_table.inode_bitmap[pinum] == 0) { //file doesn't exist, create it
		parent_inode.dir_child_inums[parent_inode.dir_num_inodes] = _inode_table.next_free;
		(parent_inode.dir_num_inodes)++;
		_inode_table.inodes[parent_inode.inum] = parent_inode;
		if (type == MFS_DIRECTORY) {
			inode_t dir_inode;
			init_dir_inode(&dir_inode, _inode_table.next_free, pinum, _inode_table.next_free, name);
			_inode_table.inode_bitmap[_inode_table.next_free] = 1;
			_inode_table.inodes[_inode_table.next_free] = dir_inode;
			(_inode_table.next_free)++;
			
		}
		else { //type == MFS_REG_FILE
			inode_t reg_inode;
			init_reg_inode(&reg_inode, _inode_table.next_free, pinum, _inode_table.next_free, name);
			_inode_table.inode_bitmap[_inode_table.next_free] = 1;
			_inode_table.inodes[_inode_table.next_free] = reg_inode;
			(_inode_table.next_free)++;
		}
		
		//go to beginning of file, where the inode table is stored
		if (lseek(_file_image_fd, 0, SEEK_SET) == -1) {
			perror("Error seeking to beginning of file");
			return -1;
		}
		if (write(_file_image_fd, &_inode_table, sizeof(inode_table_t)) == -1) {
			perror("Error creating file");
			return -1;
		}
		
		fsync(_file_image_fd);
		
		return 0;
	}
	else { //file does exist, return a success _inode_table.inode_bitmap[pinum] == 1
		return 0;
	}
}

int MFS_Write(int inum, char * buffer, int block)
{
	if (sizeof(buffer) > MFS_BLOCK_SIZE || block > 9 || block < 0 || inum < 0 || inum >= _inode_table.next_free) {
		return -1;
	}
	
	if (_inode_table.inode_bitmap[inum] <= 0) { //if not allocated or unlinked, return -1
		return -1;
	}
	//get the inode
	inode_t inode_to_write = _inode_table.inodes[inum];
	if (inode_to_write.file_type == MFS_DIRECTORY) {
		return -1;
	}
	
	if (inode_to_write.reg_block_offset == -1) { //doesn't have data yet, allocate
		inode_to_write.reg_block_offset = _data_block_table->next_free;
		_data_block_table->next_free = _data_block_table->next_free + BLOCKS_PER_FILE; //add ten to get the next free 10 block
	}
	
	//copy the data from buffer to the datablock at the inode start + offset, fill in bitmap
	int total_data_offset = inode_to_write.reg_block_offset + block;
	memcpy(_data_block_table->blocks[total_data_offset].bytes, buffer, MFS_BLOCK_SIZE);
	_data_block_table->data_block_bitmap[total_data_offset] = 1;
	
	int block_count = 0;
	//calculate the blocks that are allocated for this file and put it in the inode
	//has to be done this way in case of an overwrite
	for (int i = 0; i < BLOCKS_PER_FILE; i++)
	{
		if (_data_block_table->data_block_bitmap[total_data_offset + i] == 1) {
			block_count++;
		}
	}
	inode_to_write.reg_num_blocks = block_count;
	
	//make sure to add the node back into the table so it can be updated
	_inode_table.inodes[inum] = inode_to_write;
	
	//offset to the beginnign to write the node table
	if (lseek(_file_image_fd, 0, SEEK_SET) == -1) {
		perror("Error offsetting to inode table");
		return -1;
	}
	if (write(_file_image_fd, &_inode_table, sizeof(inode_table_t)) == -1) {
		perror("Error writing inode table to disk");
		return -1;
	}
	
	//offset to after the inode table to write the data table
	if (lseek(_file_image_fd, sizeof(inode_table_t), SEEK_SET) == -1) {
		perror("Error offsetting to data table");
		return -1;
	}
	if (write(_file_image_fd, _data_block_table, sizeof(data_block_table_t)) == -1) {
		perror("Error writing image table to file");
		return -1;
	}
	
	fsync(_file_image_fd);
	return 0;
}

int MFS_Read(int inum, char * buffer, int block)
{
	if (block > 9 || block < 0 || inum < 0 || inum >= _inode_table.next_free) {
		return -1;
	}
	
	if (_inode_table.inode_bitmap[inum] <= 0) { //check if unlinked or uncreated
		return -1;
	}
	inode_t inode = _inode_table.inodes[inum];
	
	if (inode.file_type == MFS_DIRECTORY) {
		MFS_DirEnt_t dir;
		dir.inum = inode.inum;
		memcpy(dir.name, inode.name, strlen(inode.name) + 1);
		memcpy(buffer, &dir, sizeof(dir));
		return 0;
	}
	else { //inode.file_type == MFS_REGULAR_FILE
		if (inode.reg_block_offset == -1) { //no data allocated, invalid block
			return -1;
		}
		data_block_t data_block = _data_block_table->blocks[inode.reg_block_offset + block];
		memcpy(buffer, &data_block, sizeof(data_block_t));
		return 0;
	}
}

int MFS_Unlink(int pinum, char *name)
{
	if (pinum < 0 || pinum >= _data_block_table->next_free) {
		return -1;
	}
	
	if (_inode_table.inode_bitmap[pinum] <= 0) {
		return -1;
	}
	inode_t inode = _inode_table.inodes[pinum];
	
	if (inode.file_type == MFS_REGULAR_FILE) { // has to be a directory
		return -1;
	}
	
	//inode is a directory
	if (inode.dir_num_inodes > 0) { //dir isn't empty, error
		return -1;
	}
	
	// there are no children
	for (int i = 0; i < MAX_INODES; i++)
	{
		inode_t child = _inode_table.inodes[inode.dir_child_inums[i]];
		if (strcmp(child.name, name) == 0) { // name matches
			_inode_table.inode_bitmap[child.inum] = -1; //remove the inode from the inode bitmap
			child.inum = -1;
			_inode_table.inodes[inode.dir_child_inums[i]] = child;
		}
	}
	
	//go to beginning of file for write
	if (lseek(_file_image_fd, 0, SEEK_SET) == -1) {
		perror("Error going to file start");
		return -1;
	}
	
	if (write(_file_image_fd, &_inode_table, sizeof(inode_table_t)) == -1) {
		perror("Error writing the inode table");
		return -1;
	}
	
	fsync(_file_image_fd);
	
	return 0;
}
