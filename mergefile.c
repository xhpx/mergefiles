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
char g_cur_dir[MAXPATHLEN];

int parse_args(int argc, char **argv)
{
	if (argc < 2) {
		return -1;
	}

	int opt;
	while ((opt = getopt(argc, argv, "d:s:h")) != -1) {
		switch (opt) {
			case 'd':
				strncpy(g_dest_dir, optarg, sizeof(g_dest_dir));
				break;
			case 's':
				strncpy(g_src_dir, optarg, sizeof(g_src_dir));
				break;
			case 'h':
			case '?':
				return -1;
			default:
				printf("Invalidate parameters!\n");
				return -1;

		}
	}

	return 0;
}

void usage()
{
	printf( "Usage: merge[OPTIONS...]\n"
			"      -d < destination dir>        Run as a daemon.\n"
			"      -s <source dir>              Login interval.\n"
			"      -h                           Show this help message.\n"
		  );

}
/*
int get_src_file(const char *fpath, const struct stat *sb, int typeflag)
{
	printf("g_cur_dir:%s\n", g_cur_dir);
	if (strcmp(fpath, g_cur_dir) == 0) 
			printf("fpath: %s\n", fpath);
		
	return 0;
}
*/

/*
int get_src_file(const char *dirname, char **filename) 

{

	DIR *dir;
	struct dirent *ptr;
	char base[MAXPATHLEN];

	if ((dir = opendir(dirname)) == NULL) {
		perror("Open destination directory  error...");
		return -1;
	}

	while ((ptr = readdir(dir)) !=NULL) {
			memset(base,0, MAXPATHLEN);
			strcpy(base, dirname);
		if ((strcmp(ptr->d_name, ".") == 0) || (strcmp(ptr->d_name, "..") == 0))  {
			continue;
		} else if (ptr->d_type == DT_REG)  {
			strcat(base, "/");
			strcat(base, ptr->d_name);
			printf("file :%s\n", base);
			;
		} else if (ptr->d_type == DT_DIR) {
			strcat(base, "/");
			strcat(base,ptr->d_name);
			get_dst_file(base);
		} 

	}
	
	return 0;
}
*/

int get_src_file(const char *filepath) 
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
			//printf("file :%s\n", base);
			char srcfile[MAXPATHLEN] = {0};
			char dstfile[MAXPATHLEN] = {0};
			//printf("str_dstr: %s\n", base);
			strcpy(dstfile, g_dest_dir);
			strcat(dstfile, base + strlen(g_src_dir) ); 
			printf("dstfile: %s\n\n", dstfile);


			char cmdbuf[2048] = {0};


			/*
			 * here only copy the file  that already have included in the  destination directory
			 */
			if (access(dstfile, F_OK) == 0) {
				sprintf(cmdbuf, "cp %s %s", base, dstfile); 
				printf( " cmd###: %s\n", cmdbuf);
				system(cmdbuf);
			}


		} else if (ptr->d_type == DT_DIR) {
			strcat(base, "/");
			strcat(base,ptr->d_name);
			get_src_file(base);
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

	//if (nftw(g_src_dir, cp_file, 1024, FTW_CONTINUE) < 0) {
	 if (get_src_file(g_src_dir) < 0) {
		perror("get file error");
		return -1;
	}


	

	return 0;
}
