    .file   "part1-strTable.c"
    .text
    .align  2
    .global strTable
    .type   strTable, %function
strTable:
    @ args = 0, pretend = 0, frame = 0
    @ frame_needed = 0, uses_anonymous_args = 0
    stmfd   sp!, {r4, r5}   @ args r0=&src, r1=&dest, r2=srcLen, r3=destLen store r4-r5
    @ r0 = &src
    @ r1 = &dst
    @ r2 = srcLen
    @ r3 = dstLen
    @ r4 = cnt
    @ r5 = x

    mov r4, #0            @ cnt = 0
    cmp r4, r3            @ if(destLen <= 0)
    bge .L11              @ Then Exit Function, else continue
.L9:                      @ Main Loop
    ldrb r5, [r1, r4] @ zero_extendqisi2    @ x = dest[cnt]
    mov ip, r5
.L7:                      @ Inner Loop
    subs ip, ip, #23      @ ip -= 23
    bpl .L7               @ Inner Loop until ip is negative
    add ip, ip, #23       @ ip is now 0-23
    strb  r5, [r0, ip]    @ then src[ip] = x
    add r4, r4, #1        @ increment counter
    cmp r4, r3            @ if ( cnt < destLen)
    blt .L9               @ Main Loop
.L11:
    ldmfd   sp!, {r4, r5}
    mov pc, lr
    .size   strTable, .-strTable
    .ident  "GCC: (GNU) 3.4.5"
