.import ../src/utils.s
.import ../src/classify.s
.import ../src/argmax.s
.import ../src/dot.s
.import ../src/matmul.s
.import ../src/read_matrix.s
.import ../src/relu.s
.import ../src/write_matrix.s

.data
msg0: .asciiz "Expected a0 to be 2 not: "

.globl main_test
.text
# main_test function for testing
main_test:
    # Prologue
    addi sp, sp, -8
    sw ra, 0(sp)
    sw s0, 4(sp)


    # load 1 into a2
    li a2 1

    # call classify function
    jal ra classify

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

    # we expect classify to exit early with code 26

    # exit normally
    # Epilogue
    lw ra, 0(sp)
    lw s0, 4(sp)
    addi sp, sp, 8

    li a0 0
    jal exit
