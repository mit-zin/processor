push 1
pop r1
in
next
pop r0
push r0
push 1
jbe end
push r0
push r1
mul
pop r1
push r0
push 1
sub
jmp next
end
push r1
out
hlt
