# include <stdio.h>
# include "dfile.h"

// check dalloc then dfree

int main() {

    DFile* myfile = dopen("testfile",100);
    printf("got 0x%x from dopen\n", myfile); if (myfile == NULL) perror("dopen");

    off_t b = dalloc(myfile,32);
    printf("got 0x%x from dalloc\n", b); if (b < 0) perror("dalloc");

    int fret = dfree(myfile,b);
    printf("got %d from dfree\n", fret); if (fret < 0) perror("dfree");

    int ret = dclose(myfile);
    printf("got %d from dclose\n", ret); if (ret == -1) perror("dclose");

} // end of main function
