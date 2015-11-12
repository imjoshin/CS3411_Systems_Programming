#include <unistd.h>
#include "dfile.h"
#include <sys/types.h>
#include <errno.h>
#include <stdlib.h>
#include <stdio.h>
/**
 * Authors: Riley Hirn and Josh Johnson
 *
 * Allocate space in an open file.
 *
 * @param df A pointer to the DFile structure for the file.
 * @param size The number of bytes to allocate (must be a multiple of 4).
 *
 * @return a file pointer to the first data byte, or -1 if an error.
 *         "errno" is set to EINVAL if the size is invalid.
 *         "errno" is set to ENOMEM if there isn't enough space.
 */
off_t dalloc(DFile* df, int size) {
	int fd = df->fileD;
	int newSize;
	int newNode;
	int extra = sizeof(int) + sizeof(off_t);
	off_t ret;
	int i;
	int err;
	off_t nul = 0;

	if(size % 4 != 0) {
		errno = EINVAL;
		return -1;
	}

	if(df->sizes[0] >=  size + ((2 * extra) + 4)) {
		err = lseek(fd, df->nodes[0] + sizeof(off_t), SEEK_SET);//start of size
		if(err == -1)	return -1;

		newSize = df->sizes[0] - size - (2 * extra);
		newNode = df->nodes[0];


		err = write(fd, &newSize, sizeof(int));//end of size
		if(err == -1)	return -1;

		err = lseek(fd, newSize, SEEK_CUR);//start of size2
		if(err == -1)	return -1;


		err = write(fd, &newSize, sizeof(int));//end of size2
		if(err == -1)	return -1;

		err = lseek(fd, 2 * sizeof(off_t), SEEK_CUR);//start of size1r
		if(err == -1)	return -1;

		i = -size;


		err = write(fd, &i, sizeof(int));//end of size1r
		if(err == -1)	return -1;

		ret = lseek(fd, 0, SEEK_CUR);
		if(ret == -1)	return -1;

		err = lseek(fd, size, SEEK_CUR);// start of size2r
		if(err == -1)	return -1;


		err = write(fd, &i, sizeof(int));//end of size2r
		if(err == -1)	return -1;

		if(newSize > df->sizes[1]) {
			df->sizes[0] = newSize;
			err = lseek(fd, extra + newSize + sizeof(int) + sizeof(off_t), SEEK_SET);//start of next
			if(err == -1)	return -1;

			err = write(fd, &(df->nodes[1]), sizeof(off_t));
			if(err == -1)	return -1;
		} else{
			for(i = 0; i < df->numNodes - 1; i++) {
				if(df->sizes[i + 1] > newSize) {
					df->sizes[i] = df->sizes[i + 1];
					df->nodes[i] = df->nodes[i + 1];
				} else {
					break;
				}
			}
			df->sizes[i] = newSize;
			df->nodes[i] = newNode;

			err = lseek(fd, df->nodes[i], SEEK_SET); //start of prev
			if(err == -1)	return -1;


			err = write(fd, &(df->nodes[i-1]), sizeof(off_t));
			if(err == -1)	return -1;

			err = lseek(fd, df->nodes[i] + extra + df->sizes[i] + sizeof(int), SEEK_SET);// start of next
			if(err == -1)	return -1;


			if(i != df->numNodes - 1) {
				err = write(fd, &(df->nodes[i+1]), sizeof(off_t));
				if(err == -1)	return -1;
			} else {
				err = write(fd, &nul, sizeof(off_t));
				if(err == -1)	return -1;
			}

			err = lseek(fd, df->nodes[i-1] + extra + df->sizes[i-1] + sizeof(int), SEEK_SET);//next of the one before
			if(err == -1)	return -1;

			err = write(fd, &(df->nodes[i]), sizeof(off_t));
			if(err == -1)	return -1;

			if(df->sizes[i+1] != -1) {
				err = lseek(fd, df->nodes[i+1], SEEK_SET);
				if(err == -1)	return -1;


				err = write(fd, &(df->nodes[i]), sizeof(off_t));
				if(err == -1)	return -1;
			}

			err = lseek(fd, 0, SEEK_SET);
			if(err == -1)	return -1;

			err = write(fd, &(df->nodes[0]), sizeof(off_t));
			if(err == -1)	return -1;

			err = lseek(fd, df->nodes[0], SEEK_SET);
			if(err == -1)	return -1;

			err = write(fd, &nul, sizeof(off_t));
			if(err == -1)	return -1;
		}
	} else {
		for(i = 0; i < df->numNodes; i++) {
			if(df->sizes[i] == size)
				break;
		}
		if(i == df->numNodes) {
			errno = ENOMEM;
			return -1;
		}
		
		if(i == 0) {
			err = lseek(fd, 0, SEEK_SET);	
			if(err == -1)	return -1;

			err = write(fd, &(df->nodes[i + 1]), sizeof(off_t));
			if(err == -1)	return -1;
		}

		err = lseek(fd, df->nodes[i] + sizeof(off_t), SEEK_SET);
		if(err == -1)	return -1;
	
		newSize = -size;
		err = write(fd, &newSize, sizeof(int));
		if(err == -1)	return -1;

		err = lseek(fd, size, SEEK_CUR);
		if(err == -1)	return -1;

		err = write(fd, &newSize, sizeof(int));
		if(err == -1)	return -1;

		ret = df->nodes[i] + extra;


		if(i != 0) {
			err = lseek(fd, df->nodes[i-1] + extra + df->sizes[i-1] + sizeof(int), SEEK_SET);
			if(err == -1)	return -1;


			//write previos node's next 
			err = write(fd, &(df->nodes[i+1]), sizeof(off_t));
			if(err == -1)	return -1;
		}

		if(df->sizes[i+1] != -1) {
			err = lseek(fd, df->nodes[i+1], SEEK_SET);
			if(err == -1)	return -1;

			err = write(fd, &(df->nodes[i-1]), sizeof(off_t));
			if(err == -1)	return -1;
		}

		for(i = i; i < df->numNodes - 1; i++) {
			df->sizes[i] = df->sizes[i+1];
			df->nodes[i] = df->nodes[i+1];
		}

		df->numNodes--;
	}

	return ret;
}
