# 18342 - Lab 3 support code with pre-fetch abort handler which provides a
#         register dump
#
# Written for Verdex Pro boards (can be used with Basix but they already have one)
#
# Author : Chinmay Kamat (chinmaykamat@cmu.edu)
# Date Thu, 1 November 2012 18:41:05 EDT

* Wires in an exception handler to handle prefetch aborts

* If you need to see the contents of the regs after a particular instruction, insert a
     ldr pc, =0xdeadbeef
  in the code.

* The handler should dump registers as follows :

  PREFETCH ABORT - caused by pc = 0xdeadbeee
  Registers:
  r0: 0x00000000, r1: 0x00000001, r2: 0x00000002, r3: 0x00000003,
  r4: 0x00000004, r5: 0x00000005, r6: 0x00000006, r7: 0x00000007,
  r8: 0xa3ebefdc, r9: 0xa3ec01b0, r10: 0x00000000, r11: 0x00000000,
  r12: 0x600000d7, lr: 0x5c018f44, sp: 0xa3ebec9c, cpsr: 0x600000f3

  It will go in a while(1) loop  after this (letting you actually look at the contents - this
  being an improvement over the basix version, which reboots itself)

* Code has been very lightly tested (and documented). Bug reports welcome at <chinmaykamat@cmu.edu> :)
