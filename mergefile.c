/*
 *=============================================================================
 *
 *		File Name:	mergefile.c
 *
 *	  Description:	This file finished all function.
 *
 *		  Version:	1.0
 *		  Created:  Wed Sep  7 15:33:24 2016
 *		 Compiler:  gcc
 *
 *    	   Author:  XuHongping 
 *    	   E-Mail:  mohists@hotmail.com 
 *   	  Company:  BLUDON
 *=============================================================================
 */

#define _GNU_SOURCE 500
#include <stdio.h>
#include <stdint.h>
#include <string.h>
#include <ftw.h>
#include <stdio.h>
#include <unistd.h>
#include <sys/types.h>
#include <dirent.h>

#define MAXPATHLEN 1024 

char g_dest_dir[MAXPATHLEN];
char g_src_dir[MAXPATHLEN];
int g_listflag, g_force_flag; 

int parse_args(int argc, char **argv)
{
	if (argc < 2) {
		return -1;
	}

	int opt;
	int dflag=0;
	int sflag=0;
	while ((opt = getopt(argc, argv, "d:s:lfh")) != -1) {
		switch (opt) {
			case 'd':
				strncpy(g_dest_dir, optarg, sizeof(g_dest_dir));
				dflag = 0;
				break;
			case 's':
				strncpy(g_src_dir, optarg, sizeof(g_src_dir));
				sflag = 0;
				break;
			case 'l':
				g_listflag = 1;
				break;
			case 'f':
				g_force_flag = 1;
				break;
			case 'h':
			case '?':
				return -1;
			default:
				printf("Invalidate parameters!\n");
				return -1;
		}
	}

	if (dflag == 0 || sflag == 0) {
		printf("Option -d and -s is absolutely necessary.\n");
		return -1;
	}

	return 0;
}

void usage()
{
	printf( "Usage: merge[OPTIONS...]\n"
			"      -d < destination dir>     Destination directory.\n"
			"      -s <source dir>           Source directory.\n"
			"      -l                        Display all not same files.\n"
			"      -f                        Force copy files that doesn't exist in the destination dir.\n"
			"      -h                        Show this help message.\n"
		  );

}

int traversal_src_dir(const char *filepath) 
{

	DIR *dir;
	struct dirent *ptr;
	char base[MAXPATHLEN];

	if ((dir = opendir(filepath)) == NULL) {
		perror("Open destination directory  error...");
		return -1;
	}

	while ((ptr = readdir(dir)) !=NULL) {
			memset(base,0, MAXPATHLEN);
			strcpy(base, filepath);
		if ((strcmp(ptr->d_name, ".") == 0) || (strcmp(ptr->d_name, "..") == 0))  {
			continue;
		} else if (ptr->d_type == DT_REG)  {
			strcat(base, "/");
			strcat(base, ptr->d_name);
			char dstfile[MAXPATHLEN] = {0};
			strcpy(dstfile, g_dest_dir);
			strcat(dstfile, base + strlen(g_src_dir) ); 

			char cmdbuf[2048] = {0};
			/*
			 * here only copy the file  that already have included in the  
			 * destination directory
			 */
			if (access(dstfile, F_OK) == 0) {
				sprintf(cmdbuf, "cp %s %s", base, dstfile); 
				printf( " cmd###: %s\n", cmdbuf);
				system(cmdbuf);
			}

			/*
			 * list  all files that doesn't exist in the destination directory.
			 */

			if (g_listflag == 1)  {
				if (access(dstfile, F_OK) != 0) {
					printf("+ base:%s\n", base);
				}
			}

			/*
			 * copy all files to the destination directory.
			 */

			if (g_force_flag == 1) 
				system(cmdbuf);

		} else if (ptr->d_type == DT_DIR) {
			strcat(base, "/");
			strcat(base,ptr->d_name);
			traversal_src_dir(base);
		} 

	}
	
	return 0;
}

int main(int argc, char **argv)
{

	if (parse_args(argc, argv) < 0) {
		usage();
		return -1;
	}

	 if (traversal_src_dir(g_src_dir) < 0) {
		return -1;
	}

	return 0;
}
