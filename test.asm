mov $0x00 %r1
loop:
inc %r1
mov $0x06 %acc
jne %r1 loop
halt
