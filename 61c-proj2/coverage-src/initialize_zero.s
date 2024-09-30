.globl initialize_zero

.text
# =======================================================
# FUNCTION: Initialize a zero array with the given length
# Arguments:
#   a0 (int) size of the array

# Returns:
#   a0 (int*)  is the pointer to the zero array
# Exceptions:
# - If the length of the array is less than 1,
#   this function terminates the program with error code 36.
# - If malloc fails, this function terminats the program with exit code 26.
# =======================================================
initialize_zero:
    # Checking to see if the stride and length is greater than 0.
    li t0 1
    blt a0 t0 exit_bad_len

    addi sp sp -8
    sw a0 0(sp)
    sw ra 4(sp)

    slli a0 a0 2 # get how many bytes to allocate
    jal malloc
    beqz a0 error_malloc # exit if malloc failed

    lw a1 0(sp) # load back a0
    li t0 0 # t0 is the loop index "i"

loop_start:

    # Check loop end condition
    beq t0 a1 loop_end

    # Get addresses of "i"th element of both arrays
    slli t2 t0 2 # t2 = index * sizeof(int)
    add t3 t2 a0

    # Save 0 to the "i"th element of both arrays
    sw x0 0(t3)

    # Increment loop index and array pointers
    addi t0 t0 1
    j loop_start

loop_end:
    lw ra 4(sp)
    addi sp sp 8
    jr ra

exit_bad_len:
    li a0 36
    j exit

error_malloc:
    li a0 26
    j exit
