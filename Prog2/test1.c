# include <stdio.h>
# include "dfile.h"

// check dopen/dclose

int main() {

    DFile* myfile = dopen("testfile",100);
    printf("got 0x%x from dopen\n", myfile); if (myfile == NULL) perror("dopen");

    int ret = dclose(myfile);
    printf("got %d from dclose\n", ret); if (ret == -1) perror("dclose");

} // end of main function
