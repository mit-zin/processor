;square solver
call main
hlt

main:
    call input
    call solver
    ret

input:
    in
    pop r0
    in
    pop r1
    in
    pop r2
    ret

solver:
    push r0
    push 0
    je linear_solver
    jmp square_solver
linear_solver:
    push r1
    push 0
    je b_is_0
    jmp b_isn't_0
b_is_0:
    push r2
    push 0
    je inf_roots
    jmp no_roots
inf_roots:
    call print_inf
    ret
no_roots:
    call print_no_roots
    ret
b_isn't_0:
    push 0
    push r2
    sub
    push r1
    div
    out
    ret
square_solver:
    push r1
    push r1
    mul
    push 4
    push r0
    push r2
    mul
    mul
    sub
    pop r2; Calculates the discriminant
    push r2
    push 0
    jae real_roots
    jmp no_roots
real_roots:
    push 0
    push r1
    sub
    push r2
    sqrt
    sub
    push 2
    div
    push r0
    div
    out
    push 0
    push r1
    sub
    push r2
    sqrt
    add
    push 2
    div
    push r0
    div
    out
    ret

*
Anton
loh
*

print_inf:          ; Prints "infinity roots"
    push 105
    chr
    push 110
    chr
    push 102
    chr
    push 105
    chr
    push 110
    chr
    push 105
    chr
    push 116
    chr
    push 121
    chr
    push 32
    chr
    push 114
    chr
    push 111
    chr
    push 111
    chr
    push 116
    chr
    push 115
    chr
    push 10
    chr
    ret

print_no_roots:    ; Prints "no roots"
    push 110
    chr
    push 111
    chr
    push 32
    chr
    push 114
    chr
    push 111
    chr
    push 111
    chr
    push 116
    chr
    push 115
    chr
    push 10
    chr
    ret
