.globl read_matrix

.text
# ==============================================================================
# FUNCTION: Allocates memory and reads in a binary file as a matrix of integers
#
# FILE FORMAT:
#   The first 8 bytes are two 4 byte ints representing the # of rows and columns
#   in the matrix. Every 4 bytes afterwards is an element of the matrix in
#   row-major order.
# Arguments:
#   a0 (char*) is the pointer to string representing the filename
#   a1 (int*)  is a pointer to an integer, we will set it to the number of rows
#   a2 (int*)  is a pointer to an integer, we will set it to the number of columns
# Returns:
#   a0 (int*)  is the pointer to the matrix in memory
# Exceptions:
#   - If malloc returns an error,
#     this function terminates the program with error code 26
#   - If you receive an fopen error or eof,
#     this function terminates the program with error code 27
#   - If you receive an fclose error or eof,
#     this function terminates the program with error code 28
#   - If you receive an fread error or eof,
#     this function terminates the program with error code 29
# ==============================================================================
read_matrix:

    # Prologue
    addi sp sp -20
    sw ra 0(sp)
    sw s0 4(sp) 
    sw s1 8(sp) 
    sw s2 12(sp) 
    sw s3 16(sp) 
    #sw s4 20(sp)
    #sw s5 24(sp)

    mv s1 a1
    mv s2 a2

    li a1 0
    jal fopen
    li t0 -1
    beq a0 t0 exit_27 
    
    mv s0 a0
    mv a1 s1
    li a2 4 
    jal fread
    #mv s4 a0
    li t0 4
    bne a0 t0 exit_29
    
    mv a0 s0
    mv a1 s2
    li a2 4
    jal fread
    #mv s5 a0
    li t0 4
    bne a0 t0 exit_29
    
    li a0 4
    lw t0 0(s1)
    mul a0 a0 t0
    lw t0 0(s2)
    mul a0 a0 t0
    jal malloc
    li t0 0
    beq a0 t0 exit_26
    mv s3 a0
     
    mv a1 s3
    li a2 4
    lw t0 0(s1)
    mul a2 a2 t0
    lw t0 0(s2)
    mul a2 a2 t0
    mv a0 s0
    jal fread
    li t1 4
    lw t0 0(s1)
    mul t1 t1 t0
    lw t0 0(s2)
    mul t1 t1 t0
    bne t1 a0 exit_29
    
    mv a0 s0
    jal fclose
    li t0 0
    bne a0 t0 exit_28
   
    # Epilogue
    
    mv a0 s3
    #srai a1 s4 2
    #srai a2 s5 2
    
    lw ra 0(sp)
    lw s0 4(sp) 
    lw s1 8(sp) 
    lw s2 12(sp)
    lw s3 16(sp)
    #lw s4 20(sp)
    #lw s5 24(sp)
    addi sp sp 20

    jr ra


exit_26:
    li a0 26
    j exit

exit_27:
    li a0 27
    j exit

exit_28:
    li a0 28
    j exit

exit_29:
    li a0 29
    j exit