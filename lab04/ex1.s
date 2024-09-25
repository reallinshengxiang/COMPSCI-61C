.globl pow inc_arr

.data
fail_message: .asciiz "%s test failed\n"
pow_string: .asciiz "pow"
inc_arr_string: .asciiz "inc_arr"

success_message: .asciiz "Tests passed.\n"
array:
    .word 1 2 3 4 5
exp_inc_array_result:
    .word 2 3 4 5 6

.text
main:
    # pow: should return 2 ** 7 = 128
    li a0, 2
    li a1, 7
    jal ra pow
    li t0, 128 # verifies that pow returned the right value
    beq a0, t0, next_test
    la a0, pow_string
    j failure

next_test:
    # inc_arr: increments "array" in place
    la a0, array
    li a1, 5
    jal ra inc_arr
    jal ra check_arr # Verifies inc_arr returned the right value
    # all tests pass, exit normally
    li a0, 4
    la a1, success_message
    ecall
    li a0, 10
    ecall

# Computes a0 to the power of a1.
# This is analogous to the following C pseudocode:
#
# uint32_t pow(uint32_t a0, uint32_t a1) {
#     uint32_t s0 = 1;
#     while (a1 != 0) {
#         s0 *= a0;
#         a1 -= 1;
#     }
#     return s0;
# }
#
pow:
    # BEGIN PROLOGUE
    # FIXME: Need to save the callee saved register(s)
    addi sp, sp, -4
    sw s0, 0(sp)
    # END PROLOGUE
    li s0, 1
pow_loop:
    beq a1, zero, pow_end
    mul s0, s0, a0
    addi a1, a1, -1
    j pow_loop
pow_end:
    mv a0, s0
    # BEGIN EPILOGUE
    # FIXME: Need to restore the callee saved register(s)
    lw s0, 0(sp)
    addi sp, sp, 4
    # END EPILOGUE
    jr ra

# Increments the elements of an array in-place.
# a0 holds the address of the start of the array, and a1 holds
# the number of elements it contains.
#
# This function calls the "helper_fn" function, which takes in an
# address as argument and increments the 32-bit value stored there.
inc_arr:
    # BEGIN PROLOGUE
    # FIXME: What other registers need to be saved?
    addi sp, sp, -12
    sw ra, 0(sp)
    sw s0, 4(sp)
    sw s1, 8(sp)
    # END PROLOGUE
    mv s0, a0 # Copy start of array to saved register
    mv s1, a1 # Copy length of array to saved register
    li t0, 0 # Initialize counter to 0
inc_arr_loop:
    beq t0, s1, inc_arr_end
    slli t1, t0, 2 # Convert array index to byte offset
    add a0, s0, t1 # Add offset to start of array
    # Prepare to call helper_fn
    #
    # FIXME: Add code to preserve the value in t0 before we call helper_fn
    addi sp, sp, -4
    sw t0, 0(sp)
    # Also ask yourself this: why don't we need to preserve t1?
    #
    jal ra helper_fn
    # FIXME: Restore t0
    lw t0, 0(sp)
    addi sp, sp, 4
    # Finished call for helper_fn
    addi t0, t0, 1 # Increment counter
    j inc_arr_loop
inc_arr_end:
    # BEGIN EPILOGUE
    # FIXME: What other registers need to be restored?
    lw ra, 0(sp)
    lw s0, 4(sp)
    lw s1, 8(sp)
    addi sp, sp, 12
    # END EPILOGUE
    jr ra

# This helper function adds 1 to the value at the memory address in a0.
# It doesn't return anything.
# C pseudocode for what it does: "*a0 = *a0 + 1"
#
# This function also violates calling convention, but it might not
# be reported by the Venus CC checker (try and figure out why).
# You should fix the bug anyway by filling in the prologue and epilogue
# as appropriate.
helper_fn:
    # BEGIN PROLOGUE
    # FIXME: YOUR CODE HERE
    addi sp, sp, -4
    sw s0, 0(sp)
    # END PROLOGUE
    lw t1, 0(a0)
    addi s0, t1, 1
    sw s0, 0(a0)
    # BEGIN EPILOGUE
    # FIXME: YOUR CODE HERE
    lw s0, 0(sp)
    addi sp, sp, 4
    # END EPILOGUE
    jr ra

# YOU CAN IGNORE EVERYTHING BELOW THIS COMMENT

# Checks the result of inc_arr, which should contain 2 3 4 5 6 after
# one call.
# You can safely ignore this function; it has no errors.
check_arr:
    la t0, exp_inc_array_result
    la t1, array
    addi t2, t1, 20 # Last element is 5*4 bytes off
check_arr_loop:
    beq t1, t2, check_arr_end
    lw t3, 0(t0)
    lw t4, 0(t1)
    beq t3, t4, continue
    la a0, inc_arr_string
    j failure
continue:
    addi t0, t0, 4
    addi t1, t1, 4
    j check_arr_loop
check_arr_end:
    jr ra


# prints a failure message, then terminates the program
# Since we don't return back to the caller, this is like executing an exception
# inputs: a0 = the name of the test that failed
failure:
	mv a3, a0 # load the name of the test that failed
    li a0, 4 # String print ecall
    la a1, fail_message

    ecall
    li a0, 10 # Exit ecall
    ecall
