;factorial
call main
hlt

main:
    in
    pop r0
    call factorial
    push r0
    out
    ret

factorial:
    push r0
    push 1
    ja recursion
    push 1    ; If 0 or 1
    pop r0
    ret
recursion:    ; If > 1
    push r0
    push r0
    push 1
    sub
    pop r0
    call factorial
    push r0
    mul
    pop r0
    ret

