# include <stdio.h>
# include "dfile.h"

// exact fit after allocs and frees

int main() {

    DFile* myfile = dopen("testfile",160);
    printf("got 0x%x from dopen\n", myfile); if (myfile == NULL) perror("dopen");

    off_t b12 = dalloc(myfile,12);
    printf("got 0x%x from dalloc\n", b12); if (b12 < 0) perror("dalloc");

    off_t b16 = dalloc(myfile,16);
    printf("got 0x%x from dalloc\n", b16); if (b16 < 0) perror("dalloc");

    off_t b20 = dalloc(myfile,20);
    printf("got 0x%x from dalloc\n", b20); if (b20 < 0) perror("dalloc");

    int fret = dfree(myfile,b12);
    printf("got %d from dfree\n", fret); if (fret < 0) perror("dfree");

    fret = dfree(myfile,b20);
    printf("got %d from dfree\n", fret); if (fret < 0) perror("dfree");

    off_t exact = dalloc(myfile,12);
    printf("got 0x%x from dalloc\n", exact); if (exact < 0) perror("dalloc");

    int ret = dclose(myfile);
    printf("got %d from dclose\n", ret); if (ret == -1) perror("dclose");

} // end of main function
