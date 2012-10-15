/*
 * stdlib.h: Declares C standard library functions
 *
 * Author: Mike Kasick <mkasick@andrew.cmu.edu>
 * Date:   Sun, 07 Oct 2007 01:38:16 -0400
 */

#ifndef STDLIB_H
#define STDLIB_H

#include "bits/types.h"

void exit(int status);
int read(int fildes, void *buf, size_t nbyte);
int write(int fildes, const void *buf, size_t nbyte);

#endif /* STDLIB_H */
