.import ../src/utils.s
.import ../src/dot.s

.data
.align 4
m0: .word 1 2 3 4 5 6 7 8 9
.align 4
m1: .word 1 2 3 4 5 6 7 8 9

.globl main_test
.text
# main_test function for testing
main_test:

    # load address to array m0 into a0
    la a0 m0

    # load address to array m1 into a1
    la a1 m1

    # load 3 into a2
    li a2 3

    # load 0 into a3
    li a3 0

    # load 2 into a4
    li a4 2

    # call dot function
    jal ra dot
    # we expect dot to exit early with code 37

    # exit normally
    li a0 0
    jal exit
