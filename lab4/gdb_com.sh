set architecture armv5te
target remote localhost:1234
add-symbol-file kernel/kernel 0xa3000000
add-symbol-file tasks/bin/simple_mutex 0xa0000000
