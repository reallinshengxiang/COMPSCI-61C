.import ../src/utils.s
.import ../src/argmax.s

.data
.align 4
m0: .word 3 -42 432 7 -5 6 5 -114 2
msg0: .asciiz "Expected a0 to be 2 not: "

.globl main_test
.text
# main_test function for testing
main_test:
    # Prologue
    addi sp, sp, -8
    sw ra, 0(sp)
    sw s0, 4(sp)


    # load address to array m0 into a0
    la a0 m0

    # load 9 into a1
    li a1 9

    # call argmax function
    jal ra argmax

    # save all return values in the save registers
    mv s0 a0


    # check that a0 == 2
    li t0 2
    beq s0 t0 a0_eq_2
    # print error and exit
    la a0, msg0
    jal print_str
    mv a0 s0
    jal print_int
    # Print newline
    li a0 '\n'
    jal ra print_char
    # exit with code 8 to indicate failure
    li a0 8
    jal exit
    a0_eq_2:


    # exit normally
    # Epilogue
    lw ra, 0(sp)
    lw s0, 4(sp)
    addi sp, sp, 8

    li a0 0
    jal exit
