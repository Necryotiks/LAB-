/*
 * =====================================================================================
 *
 *       Filename:  superblock.h
 *
 *    Description:  Write a C program, showblock, which displays the disk blocks of a file 
 in an EXT2 file system.
 *
 *        Version:  1.0
 *        Created:  10/19/2019 01:14:31 PM
 *       Revision:  none
 *       Compiler:  gcc
 *
 *         Author:  ELLIOTT VILLARS (), elliottvillars@gmail.com
 *   Organization:  
 *
 * =====================================================================================
 */
#include "global.h"
SPR_BLK *  get_superblock();
GRP_DESC * get_group_descriptor(int GRP_OFF);
INODE  * get_inode_start(int INODE_START);
void open_device(char * path);
void close_device();
int checkEXT2(SPR_BLK * supblk);
int get_block(int device_fd, int blk, char * buf);
int show_dir(INODE * ip);
void search_path(INODE* root_ip,int inodes_start);
void tokenize(char * pathname);
