.globl squared_loss

.text
# =======================================================
# FUNCTION: Get the squared difference of 2 int arrays,
#   store in a third array and compute the sum
# Arguments:
#   a0 (int*) is the pointer to the start of arr0
#   a1 (int*) is the pointer to the start of arr1
#   a2 (int)  is the length of the arrays
#   a3 (int*) is the pointer to the start of the loss array

# Returns:
#   a0 (int)  is the sum of the squared loss
# Exceptions:
# - If the length of the array is less than 1,
#   this function terminates the program with error code 36.
# =======================================================
squared_loss:
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

    # Getting the difference of the elements and squaring
    sub t2 t5 t6
    mul t2 t2 t2
    add t1 t1 t2

    # Storing the squared difference into a3
    slli t3 t0 2 # t2 = index * sizeof(int)
    add t3 t3 a3
    sw t2 0(t3)

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
