; Authors: Joe Battaglia, Hans Reichanbach, Josh Thiry
; Assembly code for Lab1 Section 6.2

	.file	"hello.c"
	.section	.rodata
	.align	2
.LC0:
	.ascii	"Hello World!\000"
	.text
	.align	2

;New label for the "Goodbye World!" string
.LC1:
    .ascii "Goodbye World!\000"
    .text
    .align  2
	.global	main
	.type	main, %function
main:
	@ Function supports interworking.
	@ args = 0, pretend = 0, frame = 0
	@ frame_needed = 1, uses_anonymous_args = 0
	mov	ip, sp
	
    stmfd	sp!, {fp, ip, lr, pc}
	    sub	fp, ip, #4
	
        ldr	r0, .L3			
	    bl	puts				;branching to the first word
    
        ldr r0, .L3+4			;adding 4 so we will branch to the second word
        bl puts					;branching to the second word

	    mov	r3, #42				;terminating with exit status 42
	    mov	r0, r3

	    sub	sp, fp, #12
	ldmfd	sp, {fp, sp, lr}
	bx	lr
.L4:
	.align	2
.L3:
	.word	.LC0
	.word   .LC1
    .size	main, .-main
	.ident	"GCC: (GNU) 4.1.2"
