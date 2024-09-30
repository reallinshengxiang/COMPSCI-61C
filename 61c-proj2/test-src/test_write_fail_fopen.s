.import ../src/utils.s
.import ../src/write_matrix.s

.data
msg0: .asciiz "../tests/write-matrix-1/output.bin"
.align 4
m0: .word 1 2 3 4 5 6 7 8 9

.globl main_test
.text
# main_test function for testing
main_test:

    # load filename ../tests/write-matrix-1/output.bin into a0
    la a0 msg0

    # load address to array m0 into a1
    la a1 m0

    # load 3 into a2
    li a2 3

    # load 3 into a3
    li a3 3

    # call write_matrix function
    jal ra write_matrix
    # we expect write_matrix to exit early with code 27

    # exit normally
    li a0 0
    jal exit
