/*
 * Written by Dr. David A. Poplawski
 */
# ifndef DFILE_H
# define DFILE_H

# include <sys/types.h>
# include <unistd.h>

typedef struct {
    int fileno;
} DFile;

// function prototypes
DFile* dopen(char*, int);
off_t dalloc(DFile*, int);
int dfree(DFile*, off_t);
int dclose(DFile*);

# endif
