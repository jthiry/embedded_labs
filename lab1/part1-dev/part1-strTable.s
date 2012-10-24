    @ Group Number 15
    @ Authors: Joseph Battaglia, Hans Reichenbach, Josh Thiry
    .file   "part1-strTable.c"
    .text
    .align  2
    .global strTable
    .type   strTable, %function
strTable:
    @ args = 0, pretend = 0, frame = 0
    @ frame_needed = 0, uses_anonymous_args = 0
    stmfd   sp!, {r4}    @ args r0=&src, r1=&dest, r2=srcLen, r3=destLen? store r4
    @ r0 = &src
    @ r1 = &dst
    @ r2 = srcLen
    @ r3 = dstLen & cnt
    @ r4 = x
    
    @
    @ register rearranging was removed
    @
    subs r3, r3, #1         @ cnt = destLen - 1
    bmi .L11                @ Then Exit Function, else continue
.L9:                        @ Main Loop
    ldrb    r4, [r1, r3] @ zero_extendqisi2     @ x = dest[cnt]
    mov ip, r4              @ ip = x
.L7:                        @ Inner Loop: ip = x%23
    subs ip, ip, #23        @ ip -= 23
    bpl .L7                 @ GOTO Inner Loop until ip is negative
    add ip, ip, #23         @ ip is now 0-23
    @ This loop was modfied to have only a single subs expression that executes
    @ and sets the cpsr for the branch. because our branch ends with ip 
    @ negative, we must add 23 to get our final result:
    @ ip = x % 23


	  cmp	ip, r2              @ if (ip > srcLen)
    
    @ two loads were removed: 
    @ one was a repeat load from dest[cnt] into x
    @ the second one was only used to compare the value, and NOT store if they
    @ were equal
    @ instead of loading and comparing, we just always store the byte into src,
    @ the result is the same because equal values are just overridden 
    @ This allows us to remove a branch instruction

    strleb  r4, [r0, ip]    @ then src[ip] = x
    @ Also, because there is now only one instruction in this part of the code
    @ (the store) we can make it conditional on the compare, thereby removing 
    @ another branch
    
    @ These last few lines are left the same, although further optimization 
    @ would be to reverse the loop to count DOWN instead up up, which would 
    @ save you the cmp instruction by doing a subs
    @ Note: doing this would also allow you to use the dstLen register as the 
    @ counting register, freeing up a register so you only need to push/pop r4
    @ onto the stack

    subs r3, r3, #1         @ cnt = cnt - 1
    bpl .L9                 @ Main Loop

.L11:
    ldmfd   sp!, {r4}
    mov pc, lr
.size   strTable, .-strTable
.ident  "GCC: (GNU) 3.4.5"
