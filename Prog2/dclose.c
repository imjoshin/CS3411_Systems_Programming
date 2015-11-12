#include <unistd.h>
#include "dfile.h"
#include <sys/types.h>
#include <stdlib.h>
/**
 * Authors: Riley Hirn and Josh Johnson
 *
 * Close an existing DFile.
 *
 * @param df A DFile structure.
 *
 * @return 0 if successful, -1 otherwise.
 */
int dclose(DFile* df) {
	//free(df->nodes);
	//free(df->sizes);
	//free(df);
	return close(df->fileD);
}
