Design Decisions for 18-342 Lab3
Joseph Battaglia :: jbattagl :: jb295@buffalo.edu
Hans Reichenbach :: hreichen :: hreich25@gmail.com
Josh Thiry	 :: jthiry   :: josh.thiry@gmail.com


This code was adapted from the lab3_support code given by the TAs which included the prefetch abort handler code and additional skeleton code files

IRQ Decisions:

    minimize code in irq handler to increase responsiveness of code
    Disable reentrant irqs BUT...
    enable irqs during swis, allows us to:
        wake up sleep
        update the time during read/write/etc..


Timer Decisions:

    MR0 is used for the sleep function, it is not enabled initially
    MR1 is used for periodic timer updates, it is initally enabled and set to 5ms
    By using both match registers we are able to greatly simplify our sleep and irq handler code
    having MR1 trigger every 5ms to ensure well within 10ms resolution
    keep timer as a global variable in milliseconds and reset the OSTMR every time we get an interrupt triggered
        don’t have to worry about register overflow0


Sleep Decisions:

    First idea: only use one MR to periodically update global shared variable “kernel_time”
        have sleep busy waiting until “kernel_time” reaches the desired value
        problem: too much drift by sleeping purely in software
        solution: implement sleep closer in hardware (second idea)
    Second idea: MR1 is our period update, MR0 for sleep. only ever have ONE enabled
        sleep immediately disables MR1 from interrupting, then sets up MR0, then busy waits for MR0 to trigger
        problem: kernel_time does NOT get periodically updated during sleep
        solution: during sleep, dont worry about it. Immediately after sleep, update kernel time with the time in ms that passed during the sleep
    decided to modify the match register to trigger the sleep value change instead of counting kernel_time
    add the excess time from the count register to kernel_time at the beginning and end of sleep to account for slippage


Drift Correction Methods:

    when periodic updates get interrupted by sleep, immediately export the time counter to kernel_time before resetting the counter.
    reset the timer asap when handling it


IRQ Stack Placement Decision:

    placed at a0000000 because plenty of space between that and next used memory space
