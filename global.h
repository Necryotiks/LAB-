/*
 * =====================================================================================
 *
 *       Filename:  global.h
 *
 *    Description:  
 *
 *        Version:  1.0
 *        Created:  10/19/2019 01:23:23 PM
 *       Revision:  none
 *       Compiler:  gcc
 *
 *         Author:  ELLIOTT VILLARS (), elliottvillars@gmail.com
 *   Organization:  
 *
 * =====================================================================================
 */

#include <stdio.h>
#include <stdlib.h>
#include <fcntl.h>
#include <unistd.h>
#include <string.h>
#include <ext2fs/ext2_fs.h>

#define BLKSIZE 1024
typedef struct ext2_super_block SPR_BLK;
typedef struct ext2_group_desc GRP_DESC;
typedef struct ext2_inode INODE;
typedef struct ext2_dir_entry_2 DIRENT;
SPR_BLK *spr;
GRP_DESC * gpd;
INODE * in;
char SPR_BLK_buffer[1024];
char GRP_DESC_buffer[1024];
char INODE_buffer[1024];
char * name[64];
int fd;
int n;
int blksize;
int inodesize;


