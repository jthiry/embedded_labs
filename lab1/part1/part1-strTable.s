	.file	"part1-strTable.c"
	.text
	.align	2
	.global	strTable
	.type	strTable, %function
strTable:
	@ args = 0, pretend = 0, frame = 0
	@ frame_needed = 0, uses_anonymous_args = 0
	stmfd	sp!, {r4, r5, r6} ; args r0=&src, r1=&dest, r2=srcLen, r3=destLen? store r4-r6
	mov	r5, r0 ; r5 = &src
	mov	r4, r2 ; r4 = srcLen 
	mov	r0, r3 ; r0 = destLen
	mov	r2, #0 ; r2 = counter = 0
	cmp	r2, r3  ; if(destLen <= 0)
	bge	.L11 ; Then Exit Function, else continue
.L9: ; Main Loop
	ldrb	r3, [r1, r2]	@ zero_extendqisi2 ; r3 = dest[counter]
	sub	ip, r3, #23 ; ip = r3 - 23
	cmp	ip, #22 ; if( ip <= 22)
	ble	.L13 ; Then skip inner loop
.L7: ; Inner Loop
	sub	ip, ip, #23 ; ip -= 23
	cmp	ip, #22 ; if (ip > 22)
	bgt	.L7 ; Inner Loop
.L13:
	cmp	ip, r4 ; if (ip > srcLen)
	bgt	.L4 ; Then skip this part
	ldrb	r3, [r5, ip]	@ zero_extendqisi2 ; r3 = src[ip]
	ldrb	r6, [r1, r2]	@ zero_extendqisi2 ; r6 = dest[counter]
	cmp	r3, r6 ; if ( r3 =/= r6 )
	strneb	r6, [r5, ip] ; then src[ip] = r6
.L4: ; That part was skipped
	add	r2, r2, #1 ; increment counter
	cmp	r2, r0 ; if ( counter < destLen)
	blt	.L9 ; Main Loop
.L11:
	ldmfd	sp!, {r4, r5, r6}
	mov pc, lr
	.size	strTable, .-strTable
	.ident	"GCC: (GNU) 3.4.5"

;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;	
;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;	
;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;	
    
    .file	"part1-strTable.c"
	.text
	.align	2
	.global	strTable
	.type	strTable, %function
strTable:
	@ args = 0, pretend = 0, frame = 0
	@ frame_needed = 0, uses_anonymous_args = 0
	stmfd	sp!, {r4, r5, r6, lr} ; args r0=&src, r1=&dest, r2=srcLen, r3=destLen? store r4-r6
    ; r0 = &src
    ; r1 = &dst
    ; r2 = srcLen
    ; r3 = dstLen
    ; r4 = cnt
    ; r5 = ldr dst
	
    mov	r4, #0 ; cnt = 0
	cmp	r4, r3  ; if(destLen <= 0)
	bge	.L11 ; Then Exit Function, else continue
.L9 ; Main Loop
    ldr r5, [r1, r4] ; r5 = dst[cnt] (word)
    and ip, r5, #0xFF ; ip = first byte of load
.L7 ; inner loop
    sub ip, ip, #23
    cmp ip, #22
    bgt .L7

    
    

    

    
.L11:
	ldmfd	sp!, {r4, r5, r6, lr}
    mov pc, lr
	.size	strTable, .-strTable
	.ident	"GCC: (GNU) 3.4.5"
