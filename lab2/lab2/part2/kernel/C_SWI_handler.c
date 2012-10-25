/*
 * C_SWI_handler.c: This is the C portion of the SWI handler
 *
 * Author: Joe Battaglia <JABAT295.gmail.com>
 *         Hans Reichenbach <HansReich.gmail.com>
 *         Josh Thiry <josh.thiry@gmail.com>
 * Date:   10/24/2012
 */
/*
void C_SWI_handler(unsigned swi_num, unsigned *regs){
	//Handle Shit
	switch(swi_num){
		case 0:
			//not used by us
			break;
		case 1:
			//exit
			exit(regs[0]);
			break;
		case 2:
			//not used
			break;
		case 3:
			//read
			return read(regs[0], regs[1], regs[2]);
			break;
		case 4:
			//write
			return write(regs[0], regs[1], regs[2]);
			break;
		default:
			//not recognized, throw error
			break;
	}

	//Debug
	puts("We are Handling shit\n");
}

//exits the kernel with a given status
void exit(int status) {

}

//read from a given file into a buffer for count bytes
ssize_t read(int fd, void *buf, size_t count) {

}

//write a buffer to stdout for count bytes
ssize_t write(int fd, const void *buf, size_t count) {

}*/
