.globl argmax

.text
# =================================================================
# FUNCTION: Given a int array, return the index of the largest
#   element. If there are multiple, return the one
#   with the smallest index.
# Arguments:
#   a0 (int*) is the pointer to the start of the array
#   a1 (int)  is the # of elements in the array
# Returns:
#   a0 (int)  is the first index of the largest element
# Exceptions:
#   - If the length of the array is less than 1,
#     this function terminates the program with error code 36
# =================================================================
argmax:
    # Prologue
    lw t1 0(a0)
    li t2 0
    li t3 0
    bgt a1 x0 loop_start
    li a0 36
    j exit

loop_start:
    beq a1 x0 loop_end
    addi a1 a1 -1
    lw t0 0(a0)
    bge t1 t0 loop_continue
    mv t1 t0
    mv t2 t3

loop_continue:
    addi a0 a0 4 
    addi t3 t3 1
    j loop_start

loop_end:
    # Epilogue
    mv a0 t2
    jr ra
