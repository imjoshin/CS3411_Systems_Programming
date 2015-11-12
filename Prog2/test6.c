# include <stdio.h>
# include "dfile.h"

// check 2 dallocs

int main() {

    DFile* myfile = dopen("testfile",152);
    printf("got 0x%x from dopen\n", myfile); if (myfile == NULL) perror("dopen");

    off_t b1 = dalloc(myfile,32);
    printf("got 0x%x from dalloc\n", b1); if (b1 < 0) perror("dalloc");

    off_t b2 = dalloc(myfile,16);
    printf("got 0x%x from dalloc\n", b2); if (b2 < 0) perror("dalloc");

    int ret = dclose(myfile);
    printf("got %d from dclose\n", ret); if (ret == -1) perror("dclose");

} // end of main function
