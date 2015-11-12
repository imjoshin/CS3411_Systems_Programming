# include <stdio.h>
# include "dfile.h"

// test dopen errors

int main() {

    // too small
    DFile* file1 = dopen("testfile",24);
    printf("got 0x%x from dopen\n", file1); if (file1 == NULL) perror("dopen");

    // not multiple of 4
    DFile* file2 = dopen("testfile",99);
    printf("got 0x%x from dopen\n", file2); if (file2 == NULL) perror("dopen");

    // not writable
    DFile* file3 = dopen("/testfile",99);
    printf("got 0x%x from dopen\n", file3); if (file3 == NULL) perror("dopen");


} // end of main function
