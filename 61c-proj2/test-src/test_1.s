.import ../src/utils.s
.import ../src/../coverage-src/squared_loss.s

.data
.align 4
m0: .word 5 5 5 5 5
.align 4
m1: .word 6 6 6 6 6
.align 4
m2: .word -1 -1 -1 -1 -1
msg0: .asciiz "Expected a0 to be 12 not: "
.align 4
m3: .word 1 1 1 1 1
msg1: .asciiz "Expected m2 to be:\n1 1 1 1 1\nInstead it is:\n"

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

    # load address to array m1 into a1
    la a1 m1

    # load 5 into a2
    li a2 5

    # load address to array m2 into a3
    la a3 m2

    # call squared_loss function
    jal ra squared_loss

    # save all return values in the save registers
    mv s0 a0


    # check that a0 == 12
    li t0 12
    beq s0 t0 a0_eq_12
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
    a0_eq_12:


    ##################################
    # check that m2 == [1, 1, 1, 1, 1]
    ##################################
    # a0: exit code
    li a0, 2
    # a1: expected data
    la a1, m3
    # a2: actual data
    la a2, m2
    # a3: length
    li a3, 5
    # a4: error message
    la a4, msg1
    jal compare_int_array

    # exit normally
    # Epilogue
    lw ra, 0(sp)
    lw s0, 4(sp)
    addi sp, sp, 8

    li a0 0
    jal exit
