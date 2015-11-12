# include <stdio.h>
# include "dfile.h"

int main() {

    DFile* myfile = dopen("testfile",300);
    printf("got 0x%x from dopen\n", myfile); if (myfile == NULL) perror("dopen");

    off_t b1 = dalloc(myfile,36);
    printf("got 0x%x from dalloc\n", b1); if (b1 == -1) perror("dalloc");
    off_t b2 = dalloc(myfile,16);
    printf("got 0x%x from dalloc\n", b2); if (b2 == -1) perror("dalloc");
    off_t b3 = dalloc(myfile,24);
    printf("got 0x%x from dalloc\n", b3); if (b3 == -1) perror("dalloc");

    int result;
    result = dfree(myfile,b1); if (result == -1) perror("dfree");
    printf("got %d from dfree\n", result); if (result == -1) perror("dfree");
    result = dfree(myfile,b2); if (result == -1) perror("dfree");
    printf("got %d from dfree\n", result); if (result == -1) perror("dfree");
    result = dfree(myfile,b3); if (result == -1) perror("dfree");
    printf("got %d from dfree\n", result); if (result == -1) perror("dfree");

    off_t b4 = dalloc(myfile,16);
    printf("got 0x%x from dalloc\n", b4); if (b4 == -1) perror("dalloc");
    off_t b5 = dalloc(myfile,12);
    printf("got 0x%x from dalloc\n", b5); if (b5 == -1) perror("dalloc");

    result = dclose(myfile);
    printf("got %d from dclose\n", result); if (result == -1) perror("dclose");

} // end of main function
