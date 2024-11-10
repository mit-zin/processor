*
circle
size of ram 51x51
*
call main
hlt

main:
    call input
    call fill
    call print
    ret

input:
    in
    pop r0   ; r0 - radius
    ret

*
r1 - y coordinate
r2 - x coordinate
*

fill:
    push 0
    pop r1
cycle_y_fill:
    push 0
    pop r2
cycle_x_fill:
    call paint_cell
    push r2
    push 1
    add
    pop r2
    push r2
    push 51
    jb cycle_x_fill
    push r1
    push 1
    add
    pop r1
    push r1
    push 51
    jb cycle_y_fill
    ret

paint_cell:
    push r1
    push 25
    sub
    pop r3
    push r3
    push r3
    mul
    push r2
    push 25
    sub
    pop r3
    push r3
    push r3
    mul
    add
    push r0
    push r0
    mul
    jbe star
    jmp dot
star:
    push r1
    push 51
    mul
    push r2
    add
    pop r3
    push 42
    pop [r3]
    ret
dot:
    push r1
    push 51
    mul
    push r2
    add
    pop r3
    push 46
    pop [r3]
    ret

print:
    push 0
    pop r1
cycle_y_print:
    push 10
    chr
    push 0
    pop r2
cycle_x_print:
    call print_cell
    push r2
    push 1
    add
    pop r2
    push r2
    push 51
    jb cycle_x_print
    push r1
    push 1
    add
    pop r1
    push r1
    push 51
    jb cycle_y_print
    push 10
    chr
    ret

*Печатает каждую ячейку по три
три раза, чтобы круг не
выглядел слишком узким*

print_cell:
    push r1
    push 51
    mul
    push r2
    add
    pop r3
    push [r3]
    chr
    push [r3]
    chr
    push [r3]
    chr
    ret
