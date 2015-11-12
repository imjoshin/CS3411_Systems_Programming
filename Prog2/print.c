#include "dfile.h"
#include <stdio.h>


void print(DFile* df, off_t printptr) {
	int i;
	int err;
	int fd = df->fileD;
	off_t prev;
	int size1;
	int size2;
	off_t next;

	for(i = 0; i < df->numNodes; i++)
		printf("\nsizes[%d]: %d", i, df->sizes[i]);


	for(i = 0; i < df->numNodes; i++)
		printf("\nnodes[%d]: %x", i, df->nodes[i]);

	printf("\n");

/*
	if(printptr == 0) {
		for(i = 0; i < df->numNodes; i++) {
			err = lseek(fd, df->nodes[i], SEEK_SET);
		
			err = read(fd, &prev, sizeof(off_t));
			err = read(fd, &size1, sizeof(int));

			err = lseek(fd, df->sizes[i], SEEK_CUR);

			err = read(fd, &size2, sizeof(int));
			err = read(fd, &next, sizeof(off_t));

			printf("free--- prev: %x    size1: %d    size2: %d    next: %x\n", prev, size1, size2, next);
		}
	} else {
		err = lseek(fd, printptr, SEEK_SET);

		err = read(fd, &prev, sizeof(off_t));
		err = read(fd, &size1, sizeof(int));

		err = lseek(fd, size1, SEEK_SET);

		err = read(fd, &size2, sizeof(int));
		err = read(fd, &next, sizeof(off_t));

		printf("prev: %d\nsize1: %d\nsize2: %d\nnext: %d\n", prev, size1, size2, next);
	}
*/

}
