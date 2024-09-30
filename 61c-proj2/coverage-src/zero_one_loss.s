.globl zero_one_loss

.text
# =======================================================
# FUNCTION: Return a 0-1 classifer array
# Arguments:
#   a0 (int*) is the pointer to the start of arr0
#   a1 (int*) is the pointer to the start of arr1
#   a2 (int)  is the length of the arrays
#   a3 (int*) is the pointer to the start of the result (loss) array

# Returns:
#   NONE
# Exceptions:
# - If the length of the array is less than 1,
#   this function terminates the program with error code 36.
# =======================================================
zero_one_loss:
    # Checking to see if the stride and length is greater than 0.
    li t0 1
    blt a2 t0 exit_bad_len

    li t0 0 # t0 is the loop index "i"
    li t1 0 # t1 is the running total loss

loop_start:
    # Check loop end condition
    beq t0 a2 loop_end

    # Get addresses of "i"th element of both arrays
    slli t2 t0 2 # t2 = index * sizeof(int)
    add t3 t2 a0
    add t4 t2 a1

    # Load "i"th element of both arrays
    lw t5 0(t3)
    lw t6 0(t4)

    # Get address of "i"th element of the result array
    add t3 t2 a3

    beq t5 t6 load1

load0:
    sw x0 0(t3)
    j loop_cont

load1:
    li t2 1
    sw t2 0(t3)

loop_cont:
    # Increment loop index and array pointers
    addi t0 t0 1
    j loop_start

loop_end:
    # Move result into a0 and return
    mv a0 t1
    jr ra

exit_bad_len:
    li a0 36
    j exit
