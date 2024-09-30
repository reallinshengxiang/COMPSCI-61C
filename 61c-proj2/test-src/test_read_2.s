.import ../src/utils.s
.import ../src/read_matrix.s

.data
msg0: .asciiz "../tests/read-matrix-2/input.bin"
.align 4
m0: .word -1
.align 4
m1: .word -1
.align 4
m2: .word 3
msg1: .asciiz "Expected m0 to be:\n3\nInstead it is:\n"
.align 4
m3: .word 1
msg2: .asciiz "Expected m1 to be:\n1\nInstead it is:\n"
.align 4
m4: .word 2 1 6
msg3: .asciiz "Expected array pointed to by a0 to be:\n2 1 6\nInstead it is:\n"

.globl main_test
.text
# main_test function for testing
main_test:
    # Prologue
    addi sp, sp, -8
    sw ra, 0(sp)
    sw s0, 4(sp)


    # load filename ../tests/read-matrix-2/input.bin into a0
    la a0 msg0

    # load address to array m0 into a1
    la a1 m0

    # load address to array m1 into a2
    la a2 m1

    # call read_matrix function
    jal ra read_matrix

    # save all return values in the save registers
    mv s0 a0


    ##################################
    # check that m0 == [3]
    ##################################
    # a0: exit code
    li a0, 2
    # a1: expected data
    la a1, m2
    # a2: actual data
    la a2, m0
    # a3: length
    li a3, 1
    # a4: error message
    la a4, msg1
    jal compare_int_array

    ##################################
    # check that m1 == [1]
    ##################################
    # a0: exit code
    li a0, 2
    # a1: expected data
    la a1, m3
    # a2: actual data
    la a2, m1
    # a3: length
    li a3, 1
    # a4: error message
    la a4, msg2
    jal compare_int_array

    ##################################
    # check that array pointed to by a0 == [2, 1, 6]
    ##################################
    # a0: exit code
    li a0, 2
    # a1: expected data
    la a1, m4
    # a2: actual data
    mv a2 s0
    # a3: length
    li a3, 3
    # a4: error message
    la a4, msg3
    jal compare_int_array

    # exit normally
    # Epilogue
    lw ra, 0(sp)
    lw s0, 4(sp)
    addi sp, sp, 8

    li a0 0
    jal exit
