/*
 * =====================================================================================
 *
 *       Filename:  superblock.c
 *
 *    Description:  Write a C program, showblock, which displays the disk blocks of a file 
 in an EXT2 file system.
 *
 *        Version:  1.0
 *        Created:  10/19/2019 01:16:35 PM
 *       Revision:  none
 *       Compiler:  gcc
 *
 *         Author:  ELLIOTT VILLARS (), elliottvillars@gmail.com
 *   Organization:  
 *
 * =====================================================================================
 */

#include "superblock.h"
void open_device(char * dev_path)
{
	fd = open(dev_path,O_RDWR);	
	if(fd < 0)
	{
		printf("Device could not be opened, exiting.\n");
		exit(1);
	}
}
void close_device()
{
	close(fd);
}
SPR_BLK * get_superblock()
{
	if(fd < 0)
	{
		printf("Device could not be opened, exiting.\n");
		exit(1);
	}
	else
	{
		int success  = 0;
		if((success = get_block(fd,1,SPR_BLK_buffer) < 0)) //What do we compare to?
		{
			printf("Superblock retrieval failed.\n");
			return NULL;
		}
		else
		{
			spr = (SPR_BLK*)SPR_BLK_buffer;
			return spr;
		}
		
	}
}
int checkEXT2(SPR_BLK * spr)
{

	if(spr != NULL)
	{
		if(spr->s_magic == 0xEF53)
		{
			printf("EXT2 filesystem confirmed, superblock loaded.\n");
			return 0;
		}
		else
		{
			printf("Unable to confirm existence of EXT2 filesystem.\n");
			return 1;

		}
	}
}
GRP_DESC * get_group_descriptor(int GRP_OFF)
{
	int blk = GRP_OFF + 1;

	if(fd < 0)
	{
		printf("Device could not be opened, exiting.\n");
		exit(1);
	}
	else
	{
		int success = 0;
		if((success = get_block(fd,blk,GRP_DESC_buffer)) < 0)
		{
			printf("Group descriptor block retrieval failed.\n");
			return NULL;
		}
		else
		{
			return (GRP_DESC *)GRP_DESC_buffer;
		}

	}



}
INODE  * get_inode_start(int INODE_START)
{

	if(fd < 0)
	{
		printf("Device could not be opened, exiting.\n");
		exit(1);
	}
	else
	{
		int success = 0;
		if((success = get_block(fd,INODE_START,INODE_buffer)) < 0)
		{
			printf("Inode start block retrieval failed.\n");
			return NULL;
		}
		else
		{
			return (INODE *)INODE_buffer + 1;
		}

	}
}
int get_block(int device_fd, int blk, char * buf)
{
	lseek(device_fd,blk*BLKSIZE,SEEK_SET);
	return read(device_fd,buf,BLKSIZE);
}
int search(INODE * ip,char * target)
{
	char sbuf[BLKSIZE], temp[256];
	DIRENT *dp;
	char *cp;
	int i;

	for (i=0; i < 12; i++){  // assume DIR at most 12 direct blocks
		if (ip->i_block[i] == 0)
			break;
		printf("i_block number -> %d\n",ip->i_block[i]);
		// YOU SHOULD print i_block[i] number here
		get_block(fd, ip->i_block[i], sbuf);

		dp = (DIRENT *)sbuf;
		cp = sbuf;

		while(cp < sbuf + BLKSIZE){
			strncpy(temp, dp->name, dp->name_len);
			temp[dp->name_len] = 0;
			/*printf("%4d %4d %4d %s\n", */
					/*dp->inode, dp->rec_len, dp->name_len, temp);*/
			if(!strcmp(target,temp))
			{
				printf("Found %s with inode %d\n",target,dp->inode);
				return dp->inode;
			}
			else
			{

			cp += dp->rec_len;
			dp = (DIRENT *)cp;
			}
		}
	}	
	return 0;
}
void search_path(INODE * ip,int inodes_start)
{
	int ino, blk, offset;
         /*INODE *ip -> root inode*/

         for (int i=0; i < n; i++){
	     ino = search(ip, name[i]);

             if (ino==0){
                printf("can't find %s\n", name[i]);
                exit(1);
             }

             // Mailman's algorithm: Convert (dev, ino) to INODE pointer
             blk    = (ino - 1) / 8 + inodes_start;
             offset = (ino - 1) % 8;
	     get_block(fd, blk, INODE_buffer);
	     ip = (INODE *)INODE_buffer + offset;   // ip -> new INODE
         }
	 printf("\nPRINTING DIRECT BLOCKS.\n");
	 for(int i = 0; i < 12; i++)
		 printf("%d ",ip->i_block[i]);
	 printf("\n");
	 if(ip->i_block[12])
	 {
		 printf("\nPRINTING INDIRECT BLOCKS.\n");
		 unsigned int ubuf[256];
		 lseek(fd,ip->i_block[12]*BLKSIZE,SEEK_SET);
		 read(fd,ubuf,BLKSIZE);
		 for(int i = 0; i < 256; i++)
		 {
			 printf("%d ",ubuf[i]);
		 }

	 }
	 if(ip->i_block[13])
	 {
		 printf("\nPRINTING DOUBLE INDIRECT BLOCKS.\n");
		 unsigned int ubuf[256];//HOLD 2x indirect
		 unsigned int vbuf[256];//hold 1x indirect
		 lseek(fd,ip->i_block[13]*BLKSIZE,SEEK_SET);
		 read(fd,ubuf,BLKSIZE);
		 for(int i = 0; i < 256; i++)
		 {
			 if(ubuf[i] == 0)
				 break;
			 lseek(fd,ubuf[i]*BLKSIZE,SEEK_SET);
			 read(fd,vbuf,BLKSIZE);
			 for(int j = 0; j < 256; j++)
			 {
				 if(vbuf[j] == 0)
					 break;
				 printf("%d ",vbuf[j]);
			 }

		 }

	 }
	 //TODO:Mailman's algorithn to print indirect blocks 


}
int show_dir(INODE * ip)
{
	char sbuf[BLKSIZE], temp[256];
	DIRENT *dp;
	char *cp;
	int i;

	for (i=0; i < 12; i++){  // assume DIR at most 12 direct blocks
		if (ip->i_block[i] == 0)
			break;
		printf("i_block number -> %d\n",ip->i_block[i]);
		// YOU SHOULD print i_block[i] number here
		get_block(fd, ip->i_block[i], sbuf);

		dp = (DIRENT *)sbuf;
		cp = sbuf;

		while(cp < sbuf + BLKSIZE){
			strncpy(temp, dp->name, dp->name_len);
			temp[dp->name_len] = 0;
			printf("%4d %4d %4d %s\n", 
					dp->inode, dp->rec_len, dp->name_len, temp);

			cp += dp->rec_len;
			dp = (DIRENT *)cp;
		}
	}	
}
void tokenize(char * pathname)
{
	int i = 0;
	n = 0;
	char * token;
	token = strtok(pathname,"/");
	while(token != NULL)
	{
		printf("token[%d] -> %s\n",i,token);
		name[i] = (char *)malloc(strlen(token) * sizeof(char));
		strcpy(name[i],token);
		token = strtok(NULL,"/");
		i++;
	}
	n = i;

}
