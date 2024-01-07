main:
mov $0x01 %r1
mov $0x02 %r2
push %r1
push %r2
jmp double

end:
halt

double:
pop %r3
pop %r4
add %r1 %r3 %r4
jmp end
