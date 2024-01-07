__start:
jmp main

loop1:
inc %r1
jne %r1 loop1
jmp end

main:
mov $0x1 %r1
mov $0x6 %acc
jmp loop1

end:
mov $0xff %r2
halt
