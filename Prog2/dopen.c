#include <unistd.h>
#include "dfile.h"
#include <fcntl.h>
#include <sys/types.h>
#include <stdlib.h>
#include <stdio.h>
#include <errno.h>


/**
 * Authors: Riley Hirn and Josh Johnson
 *
 * Open a new file.
 *
 * @param name The file name of the file.
 * @param initSize The number of bytes in the file.
 *                 Must be at least 36 bytes and a multiple of 4.
 *
 * @return a pointer to a DFile structure, or NULL if there are any problems.
 *         "errno" is set to EINVAL if the size is invalid.
 */

DFile* dopen(char* name, int initSize) {
	DFile * ret;
	int err;
	off_t off;
	int extra = sizeof(int) + sizeof(off_t);
	int i;
	int fd = open(name, O_CREAT | O_TRUNC | O_RDWR, 0750);
	initSize = initSize - 32;

	if(fd < 0){
		errno = EINVAL;
 		return NULL;
	}
	if(initSize < 36 || initSize % 4 != 0){
		errno = EINVAL;
		return NULL;
	}

	ret = (DFile*)malloc(sizeof(DFile));
	if(ret == NULL) return NULL;


	err = lseek(fd, 0, SEEK_SET);
	if(err == -1)	return NULL;

	off = sizeof(off_t);
//printf("writing '%d' at '%x'\n", off, lseek(fd, 0, SEEK_CUR));

	err = write(fd, &off, sizeof(off_t));
	if(err == -1)	return NULL;




	off = 0;	
//printf("writing '%d' at '%x'\n", off, lseek(fd, 0, SEEK_CUR));
	err = write(fd, &off, sizeof(off_t));
	if(err == -1)	return NULL;

//printf("writing '%d' at '%x'\n", initSize, lseek(fd, 0, SEEK_CUR));
	err = write(fd, &initSize, sizeof(int));
	if(err == -1)	return NULL;


	err = lseek(fd, initSize, SEEK_CUR);
	if(err == -1)	return NULL;

//printf("writing '%d' at '%x'\n", initSize, lseek(fd, 0, SEEK_CUR));
	err = write(fd, &initSize, sizeof(int));
	if(err == -1)	return NULL;
	
	off = 0;	
//printf("writing '%d' at '%x'\n", off, lseek(fd, 0, SEEK_CUR));
	err = write(fd, &off, sizeof(off_t));
	if(err == -1)	return NULL;

	ret->numNodes = 1;
	ret->numNodesAv = 5;
	ret->fileD = fd;
	ret->nodes = (off_t*)malloc(sizeof(off_t) * 5);
	ret->nodes[0] = sizeof(off_t);
	ret->sizes = (int*)malloc(sizeof(int) * 5);
	ret->sizes[0] = initSize;
	if(ret->nodes == 0 || ret->sizes == 0) return NULL;

	for(i = 1; i < 5; i++)
		ret->sizes[i] = -1;

	return ret;
}
