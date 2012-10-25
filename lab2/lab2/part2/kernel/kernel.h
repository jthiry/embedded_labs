/*
 * kernel.h: Kernel C header
 *
 * Author: Joe Battaglia <JABAT295.gmail.com>
 *         Hans Reichenbach <HansReich.gmail.com>
 *         Josh Thiry <josh.thiry@gmail.com>
 * Date:   10/24/2012
 */

#include <exports.h>

void C_SWI_handler(unsigned swi_num, unsigned * regs);
