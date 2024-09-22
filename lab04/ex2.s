.data
n: .word 2
exp: .word 10

.text
main:
    # load the value of n into a0
    la a0 n
    lw a0 0(a0)

    # load the value of exp into a1
    la a1 exp
    lw a1 0(a1)

    # call ex2
    jal ra ex2

    # prints the output of ex2
    mv a1 a0
    li a0 1
    ecall # Print Result

    # exits the program
    li a0 17
    li a1 0
    ecall

# this is a recursive pow function
# a0 contains the base
# a1 contains the power to raise to
# the return value should be the result of a0^a1
#     where ^ is the exponent operator, not XOR
ex2:
    # Note: Add code BELOW without altering existing lines.
    addi sp, sp, -8
    sw s0 0(sp)
    sw ra 4(sp)

    # return 1 if a1 == 0
    beq a1 x0 ex2_zero_case

    # otherwise, return ex2(a0, a1-1) * a0
    mv s0 a0      # save a0 in s0
    addi a1 a1 -1 # decrement a1

    jal ra ex2    # call ex2(a0, a1-1)

    mul a0 a0 s0  # multiply ex2(a0, a1-1) by s0
                  # (which contains the value of a0)

    j ex2_end

ex2_zero_case:
    li a0 1

ex2_end:
    lw s0 0(sp)
    lw ra 4(sp)
    addi sp, sp, 8
    jr ra
