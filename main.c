/*
 * =====================================================================================
 *
 *       Filename:  main.c
 *
 *    Description:  main
 *
 *        Version:  1.0
 *        Created:  10/19/2019 02:35:30 PM
 *       Revision:  none
 *       Compiler:  gcc
 *
 *         Author:  ELLIOTT VILLARS (), elliottvillars@gmail.com
 *   Organization:  
 *
 * =====================================================================================
 */

#include "superblock.h"

int main(int argc, char * argv[])
{
	if(argc < 3)
	{
		printf("Requires three arguments\n");
		exit(1);
	}
	else{
		char * device = argv[1];
		open_device(device);
		tokenize(argv[2]);
		spr = get_superblock();
		if(!checkEXT2(spr))
		{
			gpd = get_group_descriptor(spr->s_first_data_block);
			printf("block bitmap -> %d\n",gpd->bg_block_bitmap);
			printf("inode bitmap -> %d\n",gpd->bg_inode_bitmap);
			printf("inode start block -> %d\n",gpd->bg_inode_table);
			in = get_inode_start(gpd->bg_inode_table);
			show_dir(in);
			printf("***********************************************************************************\n");
			search_path(in,gpd->bg_inode_table);
			//NOTE: dont modify inode buff when using in.


			//print

		}
		else
		{
			exit(1);
		}
	}
}
