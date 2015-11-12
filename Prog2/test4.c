# include <stdio.h>
# include "dfile.h"

// check bad dalloc

int main() {

    // first file
    DFile* file1 = dopen("testfile1",100);
    printf("got 0x%x from dopen\n", file1); if (file1 == NULL) perror("dopen");

    // not a multiple of 4
    off_t a = dalloc(file1,17);
    printf("got 0x%x from dalloc\n", a); if (a < 0) perror("dalloc");

    // second file
    DFile* file2 = dopen("testfile2",200);
    printf("got 0x%x from dopen\n", file2); if (file2 == NULL) perror("dopen");

    // not enough spce
    off_t b = dalloc(file2,172);
    printf("got 0x%x from dalloc\n", b); if (b < 0) perror("dalloc");

} // end of main function
