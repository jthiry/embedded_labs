.file   "part1-strTable.c"
.text
.align  2
.global strTable
.type   strTable, %function
strTable:
  @ args = 0, pretend = 0, frame = 0
  @ frame_needed = 0, uses_anonymous_args = 0
  stmfd   sp!, {r4} @ args r0=&src, r1=&dest, r2=srcLen, r3=destLen? store r4-r6
  @ r0 = &src
  @ r1 = &dst
  @ r2 = srcLen
  @ r3 = dstLen & cnt
  @ r4 = x

  subs r3, r3, #1         @ cnt = destLen - 1
  bmi .L11                @ Then Exit Function, else continue
.L9:                      @ Main Loop
  ldrb    r4, [r1, r3] @ zero_extendqisi2     @ x = dest[cnt]
  mov ip, r4              @ ip = x
.L7:                      @ Inner Loop
  subs ip, ip, #23        @ ip -= 23
  bpl .L7                 @ Inner Loop until ip is negative
  add ip, ip, #23         @ ip is now 0-23
  cmp ip, r2              @ if (ip > srcLen)
  strleb  r4, [r0, ip]    @ then src[ip] = x
  subs r3, r3, #1         @ cnt = cnt - 1
  bpl .L9                 @ Main Loop
.L11:
  ldmfd   sp!, {r4}
  mov pc, lr
.size   strTable, .-strTable
.ident  "GCC: (GNU) 3.4.5"
