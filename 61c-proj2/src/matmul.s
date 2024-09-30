.globl matmul
.text
# =======================================================
# FUNCTION: Matrix Multiplication of 2 integer matrices
#   C = matmul(A, B)
# Arguments:
#   a0 (int*)  Pointer to the start of matrix A
#   a1 (int)   Number of rows (height) of matrix A
#   a2 (int)   Number of columns (width) of matrix A
#   a3 (int*)  Pointer to the start of matrix B
#   a4 (int)   Number of rows (height) of matrix B
#   a5 (int)   Number of columns (width) of matrix B
#   a6 (int*)  Pointer to the start of matrix C (result)
# Returns:
#   None (void), result stored in matrix C
# Exceptions:
#   If dimensions are invalid, exit with code 38
# =======================================================
matmul:

        ble a1, x0, error_exit
        ble a2, x0, error_exit
        ble a4, x0, error_exit
        ble a5, x0, error_exit
        bne a2, a4, error_exit
        j start

error_exit:
        li a0, 38 
        j exit
        
start:
        li t0,0
        li t1,0
        li t2,0
        li t3,0
k1_loop:
        
        bge  t1, a1, end_k1
        li t2,0
k2_loop:

        bge  t2, a5, end_k2

        addi sp, sp, -52           
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
        
        li t3,0
        mul t3,a5,t1
        slli t3,t3,2
        slli t2,t2,2
        add t3,t3,t2
        add t3,t3,a6
        add a1,a3,t2
        slli t1,t1,2
        mul t1,t1,a2
        add a0,a0,t1
        li a3,1
        add a4,x0,a5
        sw  t3,48(sp)
        jal ra dot
        lw  t3,48(sp)
        sw a0,0(t3)

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
        addi sp, sp, 52
        
        addi t2,t2,1
        j k2_loop
end_k2:
        addi t1,t1,1
        j k1_loop
end_k1:
        jr ra                  