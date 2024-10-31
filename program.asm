in
pop r0
in
pop r1
in
pop r2
push r0
push 0
jne square
push r1
push 0
jne linear
push r2
push 0
jne noroots
hlt
noroots
hlt
linear
push 0
push r2
sub
push r1
div
out
hlt
square
push r1
push r1
mul
push 4
push r0
push r2
mul
mul
sub
pop r2
push r2
push 0
jb noroots
push r2
sqrt
pop r2
push 0
push r1
sub
push r2
sub
push r0
push 2
mul
div
out
push 0
push r1
sub
push r2
add
push r0
push 2
mul
div
out
hlt
