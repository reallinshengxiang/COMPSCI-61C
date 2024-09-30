.import ../src/utils.s
.import ../src/read_matrix.s

.data
msg0: .asciiz "../tests/read-matrix-3/input.bin"
.align 4
m0: .word -1
.align 4
m1: .word -1
.align 4
m2: .word 15
msg1: .asciiz "Expected m0 to be:\n15\nInstead it is:\n"
.align 4
m3: .word 15
msg2: .asciiz "Expected m1 to be:\n15\nInstead it is:\n"
.align 4
m4: .word -5 2 1 8 12 12 -1 13 2 15 15 0 -5 5 10 -5 15 1 15 -3 1 3 9 8 14 0 15 -2 8 12 15 10 5 5 11 14 2 -4 0 -1 10 5 10 15 1 5 6 -5 6 12 13 6 13 8 8 -3 8 -4 -1 12 0 13 15 7 -2 5 -5 3 -3 12 1 -5 -2 10 -4 9 -3 9 3 -4 10 3 9 2 -1 6 11 15 7 7 -3 7 11 6 2 11 -4 0 -1 3 14 15 10 13 -1 9 -5 7 10 -1 -5 12 8 7 12 3 -5 4 2 -3 10 12 15 -2 2 1 -5 7 0 2 11 2 8 14 15 13 -2 7 13 7 8 13 0 5 8 6 7 0 -3 2 15 12 -1 12 10 15 -3 12 -1 -4 0 6 3 7 2 -4 7 -2 9 12 5 -1 12 -4 10 -3 -4 -4 10 2 15 3 10 0 7 -1 -4 7 9 8 -3 -5 6 9 11 5 13 14 7 0 9 9 -1 0 5 5 15 -5 -5 12 0 2 -5 0 9 -1 14 3 3 -1 0 7 5 14 13
msg3: .asciiz "Expected array pointed to by a0 to be:\n-5 2 1 8 12 12 -1 13 2 15 15 0 -5 5 10 -5 15 1 15 -3 1 3 9 8 14 0 15 -2 8 12 15 10 5 5 11 14 2 -4 0 -1 10 5 10 15 1 5 6 -5 6 12 13 6 13 8 8 -3 8 -4 -1 12 0 13 15 7 -2 5 -5 3 -3 12 1 -5 -2 10 -4 9 -3 9 3 -4 10 3 9 2 -1 6 11 15 7 7 -3 7 11 6 2 11 -4 0 -1 3 14 15 10 13 -1 9 -5 7 10 -1 -5 12 8 7 12 3 -5 4 2 -3 10 12 15 -2 2 1 -5 7 0 2 11 2 8 14 15 13 -2 7 13 7 8 13 0 5 8 6 7 0 -3 2 15 12 -1 12 10 15 -3 12 -1 -4 0 6 3 7 2 -4 7 -2 9 12 5 -1 12 -4 10 -3 -4 -4 10 2 15 3 10 0 7 -1 -4 7 9 8 -3 -5 6 9 11 5 13 14 7 0 9 9 -1 0 5 5 15 -5 -5 12 0 2 -5 0 9 -1 14 3 3 -1 0 7 5 14 13\nInstead it is:\n"

.globl main_test
.text
# main_test function for testing
main_test:
    # Prologue
    addi sp, sp, -8
    sw ra, 0(sp)
    sw s0, 4(sp)


    # load filename ../tests/read-matrix-3/input.bin into a0
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
    # check that m0 == [15]
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
    # check that m1 == [15]
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
    # check that array pointed to by a0 == [-5, 2, 1, 8, 12, 12, -1, 13, 2, 15, 15, 0, -5, 5, 10, -5, 15, 1, 15, -3, 1, 3, 9, 8, 14, 0, 15, -2, 8, 12, 15, 10, 5, 5, 11, 14, 2, -4, 0, -1, 10, 5, 10, 15, 1, 5, 6, -5, 6, 12, 13, 6, 13, 8, 8, -3, 8, -4, -1, 12, 0, 13, 15, 7, -2, 5, -5, 3, -3, 12, 1, -5, -2, 10, -4, 9, -3, 9, 3, -4, 10, 3, 9, 2, -1, 6, 11, 15, 7, 7, -3, 7, 11, 6, 2, 11, -4, 0, -1, 3, 14, 15, 10, 13, -1, 9, -5, 7, 10, -1, -5, 12, 8, 7, 12, 3, -5, 4, 2, -3, 10, 12, 15, -2, 2, 1, -5, 7, 0, 2, 11, 2, 8, 14, 15, 13, -2, 7, 13, 7, 8, 13, 0, 5, 8, 6, 7, 0, -3, 2, 15, 12, -1, 12, 10, 15, -3, 12, -1, -4, 0, 6, 3, 7, 2, -4, 7, -2, 9, 12, 5, -1, 12, -4, 10, -3, -4, -4, 10, 2, 15, 3, 10, 0, 7, -1, -4, 7, 9, 8, -3, -5, 6, 9, 11, 5, 13, 14, 7, 0, 9, 9, -1, 0, 5, 5, 15, -5, -5, 12, 0, 2, -5, 0, 9, -1, 14, 3, 3, -1, 0, 7, 5, 14, 13]
    ##################################
    # a0: exit code
    li a0, 2
    # a1: expected data
    la a1, m4
    # a2: actual data
    mv a2 s0
    # a3: length
    li a3, 225
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
