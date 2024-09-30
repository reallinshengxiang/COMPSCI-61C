.import ../../src/read_matrix.s
.import ../../src/write_matrix.s
.import ../../src/matmul.s
.import ../../src/dot.s
.import ../../src/relu.s
.import ../../src/argmax.s
.import ../../src/utils.s
.import ../../src/classify.s

.globl main
# This is a dummy main function which imports and calls the classify function.
# While it just exits right after, it could always call classify again.
.data
print_msg: .string "Two classifications:\n"

fname: .string "main_chain.s"

simple0_m0: .string "../tests/chain-1/batch0-m0.bin"
simple0_m1: .string "../tests/chain-1/batch0-m1.bin"
simple0_input: .string "../tests/chain-1/batch0-input.bin"
simple0_output: .string "../tests/chain-1/batch0-output.bin"

larger0_m0: .string "../tests/chain-1/batch1-m0.bin"
larger0_m1: .string "../tests/chain-1/batch1-m1.bin"
larger0_input: .string "../tests/chain-1/batch1-input.bin"
larger0_output: .string "../tests/chain-1/batch1-output.bin"

.text
main:
    # BEGIN MAIN CHAIN
    # allocate spaces for 5 pointers on the stack
    addi sp, sp, -20
    # load first argument on the stack
    la t0, fname
    sw t0, 0(sp)
    # load filepath for m0 on the stack
    la t0, simple0_m0
    sw t0, 4(sp)
    # load filepath for m1 on the stack
    la t0, simple0_m1
    sw t0, 8(sp)
    # load filepath for input on the stack
    la t0, simple0_input
    sw t0, 12(sp)
    # load filepath for output on the stack
    la t0, simple0_output
    sw t0, 16(sp)

    # load a0, a1, and a2 as arguments, preparing to call classify
    li a0 5
    mv a1 sp
    li a2 1
    # call the `classify` function
    jal classify

    # load back the stack
    addi sp, sp 16

    # save output of first classify on the stack
    sw a0, 0(sp)

    # allocate space and load filepath on the stack
    addi sp, sp, -20
    la t0, fname
    sw t0, 0(sp)
    la t0, simple0_m0
    sw t0, 4(sp)
    la t0, simple0_m1
    sw t0, 8(sp)
    la t0, simple0_input
    sw t0, 12(sp)
    la t0, simple0_output
    sw t0, 16(sp)

    # load arguments and call classify
    li a0 5
    mv a1 sp
    li a2 1
    jal classify

    # save output of second classify
    mv s1 a0

    # load back the stack pointer for the second classify call
    addi sp, sp, 20

    # load the result of the first classify call
    lw s0 0(sp)
    # restore the stack pointer to before the first classify call
    addi sp, sp, 4

    # print message
    la a0 print_msg
    jal print_str

    # print result of first classify
    mv a0 s0
    jal print_int

    li a0 '\n'
    jal print_char

    # print result of second classify
    mv a0 s1
    jal print_int

    li a0 '\n'
    jal print_char
    # END MAIN CHAIN

    # BEGIN MAIN CHAIN

    # load filepath on the stack
    addi sp, sp, -20
    la t0, fname
    sw t0, 0(sp)
    la t0, simple0_m0
    sw t0, 4(sp)
    la t0, simple0_m1
    sw t0, 8(sp)
    la t0, simple0_input
    sw t0, 12(sp)
    la t0, simple0_output
    sw t0, 16(sp)

    # load arguments and call classify
    li a0 5
    mv a1 sp
    li a2 1
    jal classify

    addi sp, sp 16

    # save output of first classify
    sw a0, 0(sp)

    # load filepath for second classify
    addi sp, sp, -20
    la t0, fname
    sw t0, 0(sp)
    la t0, larger0_m0
    sw t0, 4(sp)
    la t0, larger0_m1
    sw t0, 8(sp)
    la t0, larger0_input
    sw t0, 12(sp)
    la t0, larger0_output
    sw t0, 16(sp)

    # load arguments and call classify
    li a0 5
    mv a1 sp
    li a2 1
    jal classify

    # save output of second classify
    mv s1 a0

    # load back output of first classify, restore sp
    addi sp, sp, 20
    lw s0 0(sp)
    addi sp, sp, 4

    # print message
    la a0 print_msg
    jal print_str

    # print first classify output
    mv a0 s0
    jal print_int

    li a0 '\n'
    jal print_char

    # print second classify output
    mv a0 s1
    jal print_int

    li a0 '\n'
    jal print_char
    # END MAIN CHAIN

    # BEGIN MAIN CHAIN
    # Repeat the same process as the last two
    addi sp, sp, -20
    la t0, fname
    sw t0, 0(sp)
    la t0, larger0_m0
    sw t0, 4(sp)
    la t0, larger0_m1
    sw t0, 8(sp)
    la t0, larger0_input
    sw t0, 12(sp)
    la t0, larger0_output
    sw t0, 16(sp)

    li a0 5
    mv a1 sp
    li a2 1
    jal classify
    addi sp, sp 16

    sw a0, 0(sp)
    addi sp, sp, -20

    la t0, fname
    sw t0, 0(sp)
    la t0, larger0_m0
    sw t0, 4(sp)
    la t0, larger0_m1
    sw t0, 8(sp)
    la t0, larger0_input
    sw t0, 12(sp)
    la t0, larger0_output
    sw t0, 16(sp)

    li a0 5
    mv a1 sp
    li a2 1
    jal classify

    mv s1 a0

    addi sp, sp, 20
    lw s0 0(sp)
    addi sp, sp, 4

    la a0 print_msg
    jal print_str

    mv a0 s0
    jal print_int

    li a0 '\n'
    jal print_char

    mv a0 s1
    jal print_int

    li a0 '\n'
    jal print_char
    # END MAIN CHAIN

    li a0 0
    jal exit
