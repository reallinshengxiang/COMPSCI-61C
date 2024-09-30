.globl classify

.text
# =====================================
# COMMAND LINE ARGUMENTS
# =====================================
# Args:
#   a0 (int)        argc
#   a1 (char**)     argv
#   a1[1] (char*)   pointer to the filepath string of m0
#   a1[2] (char*)   pointer to the filepath string of m1
#   a1[3] (char*)   pointer to the filepath string of input matrix
#   a1[4] (char*)   pointer to the filepath string of output file
#   a2 (int)        silent mode, if this is 1, you should not print
#                   anything. Otherwise, you should print the
#                   classification and a newline.
# Returns:
#   a0 (int)        Classification
# Exceptions:
#   - If there are an incorrect number of command line args,
#     this function terminates the program with exit code 31
#   - If malloc fails, this function terminates the program with exit code 26
#
# Usage:
#   main.s <M0_PATH> <M1_PATH> <INPUT_PATH> <OUTPUT_PATH>
classify:

        addi sp, sp, -76
        sw   s0, 0(sp)
        sw   s1, 4(sp)
        sw   s2, 8(sp)
        sw   s3, 12(sp)
        sw   s4, 16(sp)
        sw   s5, 20(sp)
        sw   s6, 24(sp)
        sw   s7, 28(sp)
        sw   s8, 32(sp)
        sw   s9, 36(sp)
        sw   t4, 40(sp)
        sw   a1, 48(sp)
        sw   a2, 52(sp)
        sw   a3, 56(sp)
        sw   a4, 60(sp)
        sw   a5, 64(sp)
        sw   a6, 68(sp)
        sw   ra, 72(sp)
        li t0,5
        bne a0, t0, error_31
        j start

error_31:
        li a0, 31 
        j exit
error_26:
        li a0, 26
        j exit
start:
    # Read pretrained m0
        mv s0,a0
        mv s1,a1
        mv s2,a2
        addi sp, sp, -60    
        sw   ra, 0(sp)             
        sw   a1, 4(sp)              
        sw   a2, 8(sp)              
        sw   a3, 12(sp) 
        sw   a4, 16(sp)
        sw   t0, 20(sp)
        sw   t1, 24(sp)
        sw   t2, 28(sp)
        sw   t3, 32(sp)
        sw   a0, 36(sp)
        sw   a5, 40(sp)
        sw   a6, 44(sp)
        sw   s0, 48(sp)
        sw   s1, 52(sp)
        sw   s2, 56(sp)
        
        li a0,8
        jal malloc
        li t0,0 
        beq a0,t0,error_26
        mv s3,a0
        mv a1,s3 
        addi a2,s3,4          
        lw a0,4(s1)
        jal ra,read_matrix
        mv s4,a0 

        lw   ra, 0(sp)               
        lw   a1, 4(sp)                
        lw   a2, 8(sp)                
        lw   a3, 12(sp)
        lw   a4, 16(sp)
        lw   t0, 20(sp)
        lw   t1, 24(sp)
        lw   t2, 28(sp)
        lw   t3, 32(sp)
        lw   a0, 36(sp)
        lw   a5, 40(sp)
        lw   a6, 44(sp) 
        lw   s0, 48(sp)
        lw   s1, 52(sp)
        lw   s2, 56(sp)               
        addi sp, sp, 60

    # Read pretrained m1
addi sp, sp, -68         
        sw   ra, 0(sp)             
        sw   a1, 4(sp)              
        sw   a2, 8(sp)              
        sw   a3, 12(sp) 
        sw   a4, 16(sp)
        sw   t0, 20(sp)
        sw   t1, 24(sp)
        sw   t2, 28(sp)
        sw   t3, 32(sp)
        sw   a0, 36(sp)
        sw   a5, 40(sp)
        sw   a6, 44(sp)
        sw   s0, 48(sp)
        sw   s1, 52(sp)
        sw   s2, 56(sp)
        sw   s3, 60(sp)
        sw   s4, 64(sp)
        
        li a0,8 
        jal malloc

        li t0,0 
        beq a0,t0,error_26
        mv s5,a0

        mv a1,s5 
        addi a2,s5,4            
        lw a0,8(s1)
    
        jal read_matrix
        mv s6,a0 

        lw   ra, 0(sp)               
        lw   a1, 4(sp)                
        lw   a2, 8(sp)                
        lw   a3, 12(sp)
        lw   a4, 16(sp)
        lw   t0, 20(sp)
        lw   t1, 24(sp)
        lw   t2, 28(sp)
        lw   t3, 32(sp)
        lw   a0, 36(sp)
        lw   a5, 40(sp)
        lw   a6, 44(sp)
        lw   s0, 48(sp)
        lw   s1, 52(sp) 
        lw   s2, 56(sp) 
        lw   s3, 60(sp)  
        lw   s4, 64(sp)              
        addi sp, sp, 68

    # Read input matrix

addi sp, sp, -76           
        sw   ra, 0(sp)             
        sw   a1, 4(sp)              
        sw   a2, 8(sp)              
        sw   a3, 12(sp) 
        sw   a4, 16(sp)
        sw   t0, 20(sp)
        sw   t1, 24(sp)
        sw   t2, 28(sp)
        sw   t3, 32(sp)
        sw   a0, 36(sp)
        sw   a5, 40(sp)
        sw   a6, 44(sp)
        sw   s0, 48(sp)
        sw   s1, 52(sp)
        sw   s2, 56(sp)
        sw   s3, 60(sp)
        sw   s4, 64(sp)
        sw   s5, 68(sp)
        sw   s6, 72(sp)

        li a0,8 
        jal malloc

        li t0,0 
        beq a0,t0,error_26 
        mv s7,a0

        mv a1,s7
        addi a2,s7,4        
        lw a0,12(s1)
    
        jal read_matrix
        mv s8,a0      

        lw   ra, 0(sp)               
        lw   a1, 4(sp)                
        lw   a2, 8(sp)                
        lw   a3, 12(sp)
        lw   a4, 16(sp)
        lw   t0, 20(sp)
        lw   t1, 24(sp)
        lw   t2, 28(sp)
        lw   t3, 32(sp)
        lw   a0, 36(sp)
        lw   a5, 40(sp)
        lw   a6, 44(sp)
        lw   s0, 48(sp)
        lw   s1, 52(sp)
        lw   s2, 56(sp)
        lw   s3, 60(sp)
        lw   s4, 64(sp)
        lw   s5, 68(sp) 
        lw   s6, 72(sp)               
        addi sp, sp, 76
    # Compute h = matmul(m0, input)

        addi sp, sp, -88           
        sw   ra, 0(sp)             
        sw   a1, 4(sp)              
        sw   a2, 8(sp)              
        sw   a3, 12(sp) 
        sw   a4, 16(sp)
        sw   t0, 20(sp)
        sw   t1, 24(sp)
        sw   t2, 28(sp)
        sw   t3, 32(sp)
        sw   a0, 36(sp)
        sw   a5, 40(sp)
        sw   a6, 44(sp)
        sw   s0, 48(sp)
        sw   s1, 52(sp)
        sw   s2, 56(sp)
        sw   s3, 60(sp)
        sw   s4, 64(sp)
        sw   s5, 68(sp)
        sw   s6, 72(sp)
        sw   s7, 76(sp)
        sw   s8, 80(sp)

        li a0,4
        lw t0,0(s3)
        mul a0,a0,t0
        lw t0,4(s7)
        mul a0,a0,t0
        jal malloc
        li t0,0
        beq a0,t0,error_26
        mv s9,a0
        mv a0,s4
        lw a1,0(s3)
        lw a2,4(s3)
        mv a3,s8
        lw a4,0(s7)
        lw a5,4(s7)
        mv a6,s9
        sw  s9,84(sp)

        jal ra,matmul

        lw  s9,84(sp)
        lw   ra, 0(sp)               
        lw   a1, 4(sp)                
        lw   a2, 8(sp)                
        lw   a3, 12(sp)
        lw   a4, 16(sp)
        lw   t0, 20(sp)
        lw   t1, 24(sp)
        lw   t2, 28(sp)
        lw   t3, 32(sp)
        lw   a0, 36(sp)
        lw   a5, 40(sp)
        lw   a6, 44(sp)
        lw   s0, 48(sp)
        lw   s1, 52(sp)
        lw   s2, 56(sp)
        lw   s3, 60(sp)
        lw   s4, 64(sp)
        lw   s5, 68(sp)
        lw   s6, 72(sp)
        lw   s7, 76(sp)
        lw   s8, 80(sp)                
        addi sp, sp, 88
    # Compute h = relu(h)

        addi sp, sp, -88           
        sw   ra, 0(sp)             
        sw   a1, 4(sp)              
        sw   a2, 8(sp)              
        sw   a3, 12(sp) 
        sw   a4, 16(sp)
        sw   t0, 20(sp)
        sw   t1, 24(sp)
        sw   t2, 28(sp)
        sw   t3, 32(sp)
        sw   a0, 36(sp)
        sw   a5, 40(sp)
        sw   a6, 44(sp)
        sw   s0, 48(sp)
        sw   s1, 52(sp)
        sw   s2, 56(sp)
        sw   s3, 60(sp)
        sw   s4, 64(sp)
        sw   s5, 68(sp)
        sw   s6, 72(sp)
        sw   s7, 76(sp)
        sw   s8, 80(sp)
        sw   s9, 84(sp)

        mv a0,s9
        li a1,0
        lw t0,0(s3)
        lw t1,4(s7)
        mul a1,t0,t1
        jal ra,relu

        lw   ra, 0(sp)               
        lw   a1, 4(sp)                
        lw   a2, 8(sp)                
        lw   a3, 12(sp)
        lw   a4, 16(sp)
        lw   t0, 20(sp)
        lw   t1, 24(sp)
        lw   t2, 28(sp)
        lw   t3, 32(sp)
        lw   a0, 36(sp)
        lw   a5, 40(sp)
        lw   a6, 44(sp)
        lw   s0, 48(sp)
        lw   s1, 52(sp)
        lw   s2, 56(sp)
        lw   s3, 60(sp)
        lw   s4, 64(sp)
        lw   s5, 68(sp)
        lw   s6, 72(sp)
        lw   s7, 76(sp)
        lw   s8, 80(sp)
        lw   s9, 84(sp)                
        addi sp, sp, 88
    # Compute o = matmul(m1, h)
        addi sp, sp, -88           
        sw   ra, 0(sp)             
        sw   a1, 4(sp)              
        sw   a2, 8(sp)              
        sw   a3, 12(sp) 
        sw   a4, 16(sp)
        sw   t0, 20(sp)
        sw   t1, 24(sp)
        sw   t2, 28(sp)
        sw   t3, 32(sp)
        sw   a0, 36(sp)
        sw   a5, 40(sp)
        sw   a6, 44(sp)
        sw   s0, 48(sp)
        sw   s1, 52(sp)
        sw   s2, 56(sp)
        sw   s3, 60(sp)
        sw   s4, 64(sp)
        sw   s5, 68(sp)
        sw   s6, 72(sp)
        sw   s7, 76(sp)
        sw   s8, 80(sp)
        sw   s9, 84(sp)

        li a0,4
        lw t0,0(s5)
        mul a0,a0,t0
        lw t0,0(s3)
        mul a0,a0,t0
        jal malloc
        li t0,0
        beq a0,t0,error_26
        mv t4,a0
        mv a0,s6
        lw a1,0(s5)
        lw a2,4(s5)
        mv a3,s9
        lw a4,0(s3)
        lw a5,4(s7)
        mv a6,t4
        addi sp, sp, -4           
        sw   t4, 0(sp)
        jal ra,matmul
        lw   t4, 0(sp)                
        addi sp, sp, 4

        lw   ra, 0(sp)               
        lw   a1, 4(sp)                
        lw   a2, 8(sp)                
        lw   a3, 12(sp)
        lw   a4, 16(sp)
        lw   t0, 20(sp)
        lw   t1, 24(sp)
        lw   t2, 28(sp)
        lw   t3, 32(sp)
        lw   a0, 36(sp)
        lw   a5, 40(sp)
        lw   a6, 44(sp)
        lw   s0, 48(sp)
        lw   s1, 52(sp)
        lw   s2, 56(sp)
        lw   s3, 60(sp)
        lw   s4, 64(sp)
        lw   s5, 68(sp)
        lw   s6, 72(sp)
        lw   s7, 76(sp)
        lw   s8, 80(sp)
        lw   s9, 84(sp)                
        addi sp, sp, 88
    # Write output matrix o
        addi sp, sp, -92           
        sw   ra, 0(sp)             
        sw   a1, 4(sp)              
        sw   a2, 8(sp)              
        sw   a3, 12(sp) 
        sw   a4, 16(sp)
        sw   t0, 20(sp)
        sw   t1, 24(sp)
        sw   t2, 28(sp)
        sw   t3, 32(sp)
        sw   a0, 36(sp)
        sw   a5, 40(sp)
        sw   a6, 44(sp)
        sw   s0, 48(sp)
        sw   s1, 52(sp)
        sw   s2, 56(sp)
        sw   s3, 60(sp)
        sw   s4, 64(sp)
        sw   s5, 68(sp)
        sw   s6, 72(sp)
        sw   s7, 76(sp)
        sw   s8, 80(sp)
        sw   s9, 84(sp)
        sw   t4, 88(sp)

        lw   a0, 16(a1)
        mv   a1, t4
        lw   a2, 0(s5)
        lw   a3, 4(s7)
        jal  ra, write_matrix


        lw   ra, 0(sp)               
        lw   a1, 4(sp)                
        lw   a2, 8(sp)                
        lw   a3, 12(sp)
        lw   a4, 16(sp)
        lw   t0, 20(sp)
        lw   t1, 24(sp)
        lw   t2, 28(sp)
        lw   t3, 32(sp)
        lw   a0, 36(sp)
        lw   a5, 40(sp)
        lw   a6, 44(sp)
        lw   s0, 48(sp)
        lw   s1, 52(sp)
        lw   s2, 56(sp)
        lw   s3, 60(sp)
        lw   s4, 64(sp)
        lw   s5, 68(sp)
        lw   s6, 72(sp)
        lw   s7, 76(sp)
        lw   s8, 80(sp)
        lw   s9, 84(sp)
        lw   t4, 88(sp)                
        addi sp, sp, 92
    # Compute and return argmax(o)

        addi sp, sp, -92           
        sw   ra, 0(sp)             
        sw   a1, 4(sp)              
        sw   a2, 8(sp)              
        sw   a3, 12(sp) 
        sw   a4, 16(sp)
        sw   t0, 20(sp)
        sw   t1, 24(sp)
        sw   t2, 28(sp)
        sw   t3, 32(sp)
        sw   a0, 36(sp)
        sw   a5, 40(sp)
        sw   a6, 44(sp)
        sw   s0, 48(sp)
        sw   s1, 52(sp)
        sw   s2, 56(sp)
        sw   s3, 60(sp)
        sw   s4, 64(sp)
        sw   s5, 68(sp)
        sw   s6, 72(sp)
        sw   s7, 76(sp)
        sw   s8, 80(sp)
        sw   s9, 84(sp)
        sw   t4, 88(sp)

        mv a0,t4
        lw a1,0(s5)
        lw t6,4(s7)
        mul a1,a1,t6
        jal ra,argmax
        mv  t5,a0

        lw   ra, 0(sp)               
        lw   a1, 4(sp)                
        lw   a2, 8(sp)                
        lw   a3, 12(sp)
        lw   a4, 16(sp)
        lw   t0, 20(sp)
        lw   t1, 24(sp)
        lw   t2, 28(sp)
        lw   t3, 32(sp)
        lw   a0, 36(sp)
        lw   a5, 40(sp)
        lw   a6, 44(sp)
        lw   s0, 48(sp)
        lw   s1, 52(sp)
        lw   s2, 56(sp)
        lw   s3, 60(sp)
        lw   s4, 64(sp)
        lw   s5, 68(sp)
        lw   s6, 72(sp)
        lw   s7, 76(sp)
        lw   s8, 80(sp)
        lw   s9, 84(sp)
        lw   t4, 88(sp)                
        addi sp, sp, 92
    # If enabled, print argmax(o) and newline
        addi sp, sp, -96           
        sw   ra, 0(sp)             
        sw   a1, 4(sp)              
        sw   a2, 8(sp)              
        sw   a3, 12(sp) 
        sw   a4, 16(sp)
        sw   t0, 20(sp)
        sw   t1, 24(sp)
        sw   t2, 28(sp)
        sw   t3, 32(sp)
        sw   a0, 36(sp)
        sw   a5, 40(sp)
        sw   a6, 44(sp)
        sw   s0, 48(sp)
        sw   s1, 52(sp)
        sw   s2, 56(sp)
        sw   s3, 60(sp)
        sw   s4, 64(sp)
        sw   s5, 68(sp)
        sw   s6, 72(sp)
        sw   s7, 76(sp)
        sw   s8, 80(sp)
        sw   s9, 84(sp)
        sw   t4, 88(sp)
        sw   t5, 92(sp)

        bne a2,x0,next
        mv a0,t5
        jal ra,print_int
        li a0,'\n'
        jal ra,print_char
next:

        lw   ra, 0(sp)               
        lw   a1, 4(sp)                
        lw   a2, 8(sp)                
        lw   a3, 12(sp)
        lw   a4, 16(sp)
        lw   t0, 20(sp)
        lw   t1, 24(sp)
        lw   t2, 28(sp)
        lw   t3, 32(sp)
        lw   a0, 36(sp)
        lw   a5, 40(sp)
        lw   a6, 44(sp)
        lw   s0, 48(sp)
        lw   s1, 52(sp)
        lw   s2, 56(sp)
        lw   s3, 60(sp)
        lw   s4, 64(sp)
        lw   s5, 68(sp)
        lw   s6, 72(sp)
        lw   s7, 76(sp)
        lw   s8, 80(sp)
        lw   s9, 84(sp)
        lw   t4, 88(sp)
        lw   t5, 92(sp)                
        addi sp, sp, 96

#free
        
        addi sp, sp, -12       
        sw   ra, 0(sp)             
        sw   t5, 4(sp)
        sw   t4, 8(sp)
        mv  a0,s4
        jal ra,free
        mv  a0,s3
        jal ra,free
        mv  a0,s6
        jal ra,free
        mv  a0,s9
        jal ra,free
        mv  a0,s8
        jal ra,free
        mv  a0,s5
        jal ra,free
        mv  a0,s7
        jal ra,free
        lw  t4, 8(sp)               
        mv  a0,t4
        jal ra,free
        lw  ra, 0(sp)
        lw  t5, 4(sp)               
        addi sp, sp, 12
        mv a0,t5
    lw s0,0(sp)
    lw s1,4(sp)
    lw s2,8(sp)
    lw s3,12(sp)
    lw s4,16(sp)
    lw s5,20(sp)
    lw s6,24(sp)
    lw s7,28(sp)
    lw s8,32(sp)
    lw s9,36(sp)
    lw t4,40(sp)
    lw a1,48(sp)
    lw a2,52(sp)
    lw a3,56(sp)
    lw a4,60(sp)
    lw a5,64(sp)
    lw a6,68(sp)
    lw ra,72(sp)
    addi sp,sp,76
    jr ra