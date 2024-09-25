.data
source:
    .word   3
    .word   1
    .word   4
    .word   1
    .word   5
    .word   9
    .word   0
dest:
    .word   0
    .word   0
    .word   0
    .word   0
    .word   0
    .word   0
    .word   0
    .word   0
    .word   0
    .word   0

.text
main:
    # BEGIN PROLOGUE
    addi sp, sp, -20
    sw s0, 0(sp)
    sw s1, 4(sp)
    sw s2, 8(sp)
    sw s3, 12(sp)
    sw ra, 16(sp)
    # END PROLOGUE
    addi t0, x0, 0 # t0 = k = 0
    addi s0, x0, 0 # s0 = sum = 0
    la s1, source
    la s2, dest
loop:
    slli s3, t0, 2
    add t1, s1, s3
    lw t2, 0(t1)
    beq t2, x0, exit
    add a0, x0, t2 # 1
    addi sp, sp, -4
    sw t0, 0(sp)
    jal fun
    lw t0, 0(sp)
    addi sp, sp, 4
    add t3, s2, s3 # 4
    sw a0, 0(t3) # 5
    add s0, s0, a0 # 6
    addi t0, t0, 1
    jal x0, loop
exit:
    addi a0, x0, 1 # argument to ecall, 1 = execute print integer
    addi a1, s0, 0 # argument to ecall, the value to be printed
    ecall # print integer ecall
    # BEGIN EPILOGUE
    lw s0, 0(sp)
    lw s1, 4(sp)
    lw s2, 8(sp)
    lw s3, 12(sp)
    lw ra, 16(sp)
    addi sp, sp, 20
    # END EPILOGUE
    addi a0, x0, 10 # argument to ecall, 10 = terminate program
    ecall # terminate program

fun:
    addi t0, a0, 1 # t0 = x + 1
    sub t1, x0, a0 # t1 = -x
    mul a0, t0, t1 # a0 = (x + 1) * (-x)
    jr ra # return
