mov $0x1 %r1

mov $0x6 %acc
loop1:
inc %r1
jne %r1 loop1

mov $0x1 %acc
loop2:
dec %r1
jne %r1 loop2

halt
