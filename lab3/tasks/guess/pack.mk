PROGS_TYPO_OBJS := guess.o
PROGS_TYPO_OBJS := $(PROGS_TYPO_OBJS:%=$(TDIR)/guess/%)
ALL_OBJS += $(PROGS_TYPO_OBJS)

$(TDIR)/bin/guess : $(TSTART) $(PROGS_TYPO_OBJS) $(TLIBC)
