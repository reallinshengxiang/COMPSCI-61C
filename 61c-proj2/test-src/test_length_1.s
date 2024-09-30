.import ../src/utils.s
.import ../src/../coverage-src/initialize_zero.s

.data
.align 4
m0: .word 0
msg0: .asciiz "Expected array pointed to by a0 to be:\n0\nInstead it is:\n"

.globl main_test
.text
# main_test function for testing
main_test:
    # Prologue
    addi sp, sp, -8
    sw ra, 0(sp)
    sw s0, 4(sp)


    # load 1 into a0
    li a0 1

    # call initialize_zero function
    jal ra initialize_zero

    # save all return values in the save registers
    mv s0 a0


    ##################################
    # check that array pointed to by a0 == [0]
    ##################################
    # a0: exit code
    li a0, 2
    # a1: expected data
    la a1, m0
    # a2: actual data
    mv a2 s0
    # a3: length
    li a3, 1
    # a4: error message
    la a4, msg0
    jal compare_int_array

    # exit normally
    # Epilogue
    lw ra, 0(sp)
    lw s0, 4(sp)
    addi sp, sp, 8

    li a0 0
    jal exit
