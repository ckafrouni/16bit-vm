main:
mov $0x01 %r1
push %r1
push $0x01
call double
halt

double:
pop %r1
add %r1 %r1
push %r1
ret
