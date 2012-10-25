/*
 * C_SWI_handler.c: This is the C portion of the SWI handler
 *
 * Author: Joe Battaglia <JABAT295.gmail.com>
 *         Hans Reichenbach <HansReich.gmail.com>
 *         Josh Thiry <josh.thiry@gmail.com>
 * Date:   10/24/2012
 */

#include "kernel.h"

void C_SWI_handler(unsigned int swi_num, unsigned register* regs){
	//Handle Shit
	switch(swi_num){
		case 0:
		  //not used by us
			break;
		case 1:
		  //exit
			break;
		case 2:
		  //not used
			break;
		case 3:
		  //read
			break;
	  case 4:
	    //write
	    break;
		default:
		  //not recognized, throw error
			break;
	}

	//Debug
	puts("We are Handling shit\n");
}
