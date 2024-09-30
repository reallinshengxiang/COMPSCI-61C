.globl relu

.text
# ==============================================================================
# FUNCTION: Performs an inplace element-wise ReLU on an array of ints
# Arguments:
#   a0 (int*) is the pointer to the array
#   a1 (int)  is the # of elements in the array
# Returns:
#   None
# Exceptions:
#   - If the length of the array is less than 1,
#     this function terminates the program with error code 36
# ==============================================================================
relu:
    # Prologue
    bgt a1 x0 loop_start
    li a0 36
    j exit

loop_start:

   beq a1 x0 loop_end
   addi a1 a1 -1
   lw t0 0(a0)
   bge t0 x0 loop_continue
   sw x0 0(a0)

loop_continue:
   addi a0 a0 4 
   j loop_start

loop_end:


    # Epilogue


    jr ra
