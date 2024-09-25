.globl malloc, free, exit

.text
malloc:
    mv a1 a0
    li a0 0x3CC
    addi a6 x0 1
    ecall
    jr ra

free:
    mv a1 a0
    li a0 0x3CC
    addi a6 x0 4
    ecall
    jr ra

exit:
    mv a1 a0
    li a0 17
    ecall
