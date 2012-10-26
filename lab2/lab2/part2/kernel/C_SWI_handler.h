/*
 * C_SWI_handler.h: This is the header file for the swi handler and syscalls
 *
 * Author: Joe Battaglia <JABAT295.gmail.com>
 *         Hans Reichenbach <HansReich.gmail.com>
 *         Josh Thiry <josh.thiry@gmail.com>
 * Date:   10/24/2012
 */

//necessary libs
#include <exports.h>
#include "bits/fileno.h"
#include "bits/errno.h"

//prototypes
int C_SWI_handler(unsigned swi_num, unsigned * regs);
void exit(int status);
ssize_t read(int fd, void *buf, size_t count);
ssize_t write(int fd, const void *buf, size_t count);

// assembly functions
//extern void EXIT(int status);
