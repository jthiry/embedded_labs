/*
 * C_SWI_handler.h: This is the header file for the swi handler and syscalls
 *
 * Author: Joe Battaglia <JABAT295.gmail.com>
 *         Hans Reichenbach <HansReich.gmail.com>
 *         Josh Thiry <josh.thiry@gmail.com>
 * Date:   10/24/2012
 */

//necessary libs

//prototypes
int c_swi_handler(unsigned swi_num, unsigned * regs);
void c_exit(int status);
ssize_t c_read(int fd, void *buf, size_t count);
ssize_t c_write(int fd, const void *buf, size_t count);
