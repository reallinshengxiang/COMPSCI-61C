.globl dot

.text
# =======================================================
# FUNCTION: Dot product of 2 int arrays
# Arguments:
#   a0 (int*) is the pointer to the start of arr0
#   a1 (int*) is the pointer to the start of arr1
#   a2 (int)  is the number of elements to use
#   a3 (int)  is the stride of arr0
#   a4 (int)  is the stride of arr1
# Returns:
#   a0 (int)  is the dot product of arr0 and arr1
# Exceptions:
#   - If the number of elements to use is less than 1,
#     this function terminates the program with error code 36
#   - If the stride of either array is less than 1,
#     this function terminates the program with error code 37
# =======================================================
dot:

    # Prologue
    ble a2 x0 exit_36
    ble a3 x0 exit_37
    ble a4 x0 exit_37
    li t0 4
    li t2 0
    mul a3 a3 t0
    mul a4 a4 t0
    j loop_start
    
exit_36:
    li a0 36
    j exit
exit_37:
    li a0 37
    j exit
    

loop_start:
    beq a2 x0 loop_end
    addi a2 a2 -1
    lw t0 0(a0)
    lw t1 0(a1)
    add a0 a0 a3
    add a1 a1 a4
    mul t1 t0 t1
    add t2 t2 t1
    j loop_start

loop_end:


    # Epilogue
    mv a0 t2

    jr ra
