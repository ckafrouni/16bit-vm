mov $0x1 %r1
mov $0x6 %r2

loop1:
inc %r1
jne %r1 %r2 loop1

mov $0x1 %r2

loop2:
dec %r1
jne %r1 %r2 loop2

halt
