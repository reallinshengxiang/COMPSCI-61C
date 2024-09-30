##############################################################
# Do not modify! (But feel free to use the functions provided)
##############################################################

#define c_print_int 1
#define c_print_str 4
#define c_atoi 5
#define c_sbrk 9
#define c_print_char 11
#define c_openFile 13
#define c_readFile 14
#define c_writeFile 15
#define c_closeFile 16
#define c_exit2 17
#define c_fflush 18
#define c_feof 19
#define c_ferror 20
#define c_printHex 34

# ecall wrappers
.globl print_int, print_str, atoi, sbrk, exit, print_char, fopen, fread, fwrite, fclose, fflush, ferror, print_hex

# helper functions
.globl file_error, print_int_array, malloc, free, print_num_alloc_blocks, num_alloc_blocks

# unittest helper functions
.globl compare_int_array

# Calling Convention Functions
.globl randomizeCallerSavedRegs, randomizeCallerSavedRegsBesidesA0, randomizeCalleeSavedRegs, checkCalleeSavedRegs, randomizeCalleeSavedRegs2, checkCalleeSavedRegs2

.data
static_space_for_sp: .word 0 0 0
error_string: .string "This library file should not be directly called!"
static_space_for_sp2: .word 0 0 0
error_string2: .string "This library file should not be directly called!"


.text
# Exits if you run this file
main:
    la a0 error_string
    jal print_str
    li a0 1
    jal exit
# End main


#================================================================
# void print_int(int a0)
# Prints the integer in a0.
# args:
#   a0 = integer to print
# return:
#   void
#================================================================
print_int:
    mv a1 a0
    li a0 c_print_int
    ecall
    jr ra


#================================================================
# void print_str(char *a0)
# Prints the null-terminated string at address a0.
# args:
#   a0 = address of the string you want printed.
# return:
#   void
#================================================================
print_str:
    mv a1 a0
    li a0 c_print_str
    ecall
    jr ra


#================================================================
# int atoi(char* a0)
# Returns the integer version of the string at address a0.
# args:
#   a0 = address of the string you want to turn into an integer.
# return:
#   a0 = Integer representation of string
#================================================================
atoi:
    mv a1 a0
    li a0 c_atoi
    ecall
    jr ra


#================================================================
# void *sbrk(int a0)
# Allocates a0 bytes onto the heap.
# args:
#   a0 = Number of bytes you want to allocate.
# return:
#   a0 = Pointer to the start of the allocated memory
#================================================================
sbrk:
    mv a1 a0
    li a0 c_sbrk
    ecall
    jr ra


#================================================================
# void noreturn exit(int a0)
# Exits the program with error code a0.
# args:
#   a0 = Exit code.
# return:
#   This program does not return.
#================================================================
exit:
    mv a1 a0
    li a0 c_exit2
    ecall


#================================================================
# void print_char(char a0)
# Prints the ASCII character in a0 to the console.
# args:
#   a0 = character to print
# return:
#   void
#================================================================
print_char:
    mv a1 a0
    li a0 c_print_char
    ecall
    jr ra


#================================================================
# int fopen(char *a0, int a1)
# Opens file with name a0 with permissions a1.
# args:
#   a0 = filepath
#   a1 = permissions (0, 1, 2, 3, 4, 5 = r, w, a, r+, w+, a+)
# return:
#   a0 = file descriptor
#================================================================
fopen:
    mv a2 a1
    mv a1 a0
    li a0 c_openFile
    ecall
    #FOPEN_RETURN_HOOK
    jr ra


#================================================================
# int fread(int a0, void *a1, size_t a2)
# Reads a2 bytes of the file into the buffer a1.
# args:
#   a0 = file descriptor
#   a1 = pointer to the buffer you want to write the read bytes to.
#   a2 = Number of bytes to be read.
# return:
#   a0 = Number of bytes actually read.
#================================================================
fread:
    mv a3 a2
    mv a2 a1
    mv a1 a0
    li a0 c_readFile
    ecall
    #FREAD_RETURN_HOOK
    jr ra


#================================================================
# int fwrite(int a0, void *a1, size_t a2, size_t a3)
# Writes a2 * a3 bytes from the buffer in a1 to the file descriptor a0.
# args:
#   a0 = file descriptor
#   a1 = Buffer to read from
#   a2 = Number of items to read from the buffer.
#   a3 = Size of each item in the buffer.
# return:
#   a0 = Number of elements writen. If this is less than a2,
#    it is either an error or EOF. You will also need to still flush the fd.
#================================================================
fwrite:
    mv a4 a3
    mv a3 a2
    mv a2 a1
    mv a1 a0
    li a0 c_writeFile
    ecall
    #FWRITE_RETURN_HOOK
    jr ra


#================================================================
# int fclose(int a0)
# Closes the file descriptor a0.
# args:
#   a0 = file descriptor
# return:
#   a0 = 0 on success, and EOF (-1) otherwise.
#================================================================
fclose:
    mv a1 a0
    li a0 c_closeFile
    ecall
    #FCLOSE_RETURN_HOOK
    jr ra


#================================================================
# int fflush(int a0)
# Flushes the data to the filesystem.
# args:
#   a0 = file descriptor
# return:
#   a0 = 0 on success, and EOF (-1) otherwise.
#================================================================
fflush:
    mv a1 a0
    li a0 c_fflush
    ecall
    jr ra


#================================================================
# int ferror(int a0)
# Returns a nonzero value if the file stream has errors, otherwise it returns 0.
# args:
#   a0 = file descriptor
# return:
#   a0 = Nonzero falue if the end of file is reached. 0 Otherwise.
#================================================================
ferror:
    mv a1 a0
    li a0 c_ferror
    ecall
    jr ra


#================================================================
# void print_hex(int a0)
#
# args:
#   a0 = The word which will be printed as a hex value.
# return:
#   void
#================================================================
print_hex:
    mv a1 a0
    li a0 c_printHex
    ecall
    jr ra




#================================================================
# void* malloc(int a0)
# Allocates heap memory and return a pointer to it
# args:
#   a0 is the # of bytes to allocate heap memory for
# return:
#   a0 is the pointer to the allocated heap memory
#================================================================
malloc:
    # Call to sbrk
    mv a1 a0
    li a0 0x3CC
    addi a6 x0 1
    ecall
    #MALLOC_RETURN_HOOK
    jr ra


#================================================================
# void free(int a0)
# Frees heap memory referenced by pointer
# args:
#   a0 is the pointer to heap memory to free
# return:
#   void
#================================================================
free:
    mv a1 a0
    li a0 0x3CC
    addi a6 x0 4
    ecall
    jr ra

#================================================================
# void num_alloc_blocks(int a0)
# Returns the number of currently allocated blocks
# args:
#   void
# return:
#   a0 is the # of allocated blocks
#================================================================
num_alloc_blocks:
    li a0, 0x3CC
    li a6, 5
    ecall
    jr ra

print_num_alloc_blocks:
    addi sp, sp -4
    sw ra 0(sp)

    jal num_alloc_blocks
    mv a0 a0
    jal print_int

    li a0 '\n'
    jal print_char

    lw ra 0(sp)
    addi sp, sp 4
    jr ra

#================================================================
# void print_int_array(int* a0, int a1, int a2)
# Prints an integer array, with spaces between the elements
# args:
#   a0 is the pointer to the start of the array
#   a1 is the # of rows in the array
#   a2 is the # of columns in the array
# return:
#   void
#================================================================
print_int_array:
    # Prologue
    addi sp sp -24
    sw s0 0(sp)
    sw s1 4(sp)
    sw s2 8(sp)
    sw s3 12(sp)
    sw s4 16(sp)
    sw ra 20(sp)

    # Save arguments
    mv s0 a0
    mv s1 a1
    mv s2 a2

    # Set outer loop index
    li s3 0

outer_loop_start:
    # Check outer loop condition
    beq s3 s1 outer_loop_end

    # Set inner loop index
    li s4 0

inner_loop_start:
    # Check inner loop condition
    beq s4 s2 inner_loop_end

    # t0 = row index * len(row) + column index
    mul t0 s2 s3
    add t0 t0 s4
    slli t0 t0 2

    # Load matrix element
    add t0 t0 s0
    lw t1 0(t0)

    # Print matrix element
    mv a0 t1
    jal print_int

    # Print whitespace
    li a0 ' '
    jal print_char


    addi s4 s4 1
    j inner_loop_start

inner_loop_end:
    # Print newline
    li a0 '\n'
    jal print_char

    addi s3 s3 1
    j outer_loop_start

outer_loop_end:
    # Epilogue
    lw s0 0(sp)
    lw s1 4(sp)
    lw s2 8(sp)
    lw s3 12(sp)
    lw s4 16(sp)
    lw ra 20(sp)
    addi sp sp 24

    jr ra

#================================================================
# void compare_int_array(int a0, int* a0, int* a1, int a2)
# Prints an integer array, with spaces between the elements
# args:
#   a0 is the base exit code that will be used if an unequal element is found
#   a1 is the pointer to the expected data
#   a2 is the pointer to the actual data
#   a3 is the number of elements in each array
#   a4 is the error message
# return:
#   void
#================================================================
compare_int_array:
    # Prologue
    addi sp sp -24
    sw s0 0(sp)
    sw s1 4(sp)
    sw s2 8(sp)
    sw s3 12(sp)
    sw s4 16(sp)
    sw ra 20(sp)

    # save pointer to original array in s1
    mv s1, a2

    # t0: current element
    mv t0 zero

loop_start:
    # we are done once t0 >= a3
    bge t0, a3, end

    # t1 := *a1
    lw t1, 0(a1)
    # t2 := *a2
    lw t2, 0(a2)

    # if the values are different -> fail
    bne t1, t2, fail

    # go to next value
    addi t0, t0, 1
    addi a1, a1, 4
    addi a2, a2, 4
    j loop_start

fail:
    # exit code: a0
    mv s0, a0
    # remember length
    mv s2, a3

    # print user supplied error message
    mv a0, a4
    jal print_str

    # print actual data
    mv a0, s1
    li a1, 1
    mv a2, s2
    jal print_int_array

    # exit with user defined error code
    mv a0, s0
    jal exit

end:
    # Epilogue
    lw s0 0(sp)
    lw s1 4(sp)
    lw s2 8(sp)
    lw s3 12(sp)
    lw s4 16(sp)
    lw ra 20(sp)
    addi sp sp 24

    jr ra


# Shuffling registers to test calling convention

randomizeCallerSavedRegs:
    li t0 0x61C0061C
    li t1 0x61C1161C
    li t2 0x61C2261C
    li t3 0x61C3361C
    li t4 0x61C4461C
    li t5 0x61C5561C
    li t6 0x61C6661C

    li a0 0x61C8861C
    li a1 0x61C9961C
    li a2 0x61Caa61C
    li a3 0x61Cbb61C
    li a4 0x61Ccc61C
    li a5 0x61Cdd61C
    li a6 0x61Cee61C
    li a7 0x61Cff61C

    jr ra

randomizeCallerSavedRegsBesidesA0:
    li t0 0x61C0061C
    li t1 0x61C1161C
    li t2 0x61C2261C
    li t3 0x61C3361C
    li t4 0x61C4461C
    li t5 0x61C5561C
    li t6 0x61C6661C

    li a1 0x61C9961C
    li a2 0x61Caa61C
    li a3 0x61Cbb61C
    li a4 0x61Ccc61C
    li a5 0x61Cdd61C
    li a6 0x61Cee61C
    li a7 0x61Cff61C

    jr ra

randomizeCalleeSavedRegs:
    addi sp sp -48
    sw s0 0(sp)
    sw s1 4(sp)
    sw s2 8(sp)
    sw s3 12(sp)
    sw s4 16(sp)
    sw s5 20(sp)
    sw s6 24(sp)
    sw s7 28(sp)
    sw s8 32(sp)
    sw s9 36(sp)
    sw s10 40(sp)
    sw s11 44(sp)
    la t0 static_space_for_sp
    sw sp 0(t0)

    li s0, 0x61D0061D
    li s1, 0x61D1161D
    li s2, 0x61D2261D
    li s3, 0x61D3361D
    li s4, 0x61D4461D
    li s5, 0x61D5561D
    li s6, 0x61D6661D
    li s7, 0x61D7761D
    li s8, 0x61D8861D
    li s9, 0x61D9961D
    li s10, 0x61Daa61D
    li s11, 0x61Dbb61D

    jr ra

checkCalleeSavedRegs:
    li t0, 0x61D0061D
    bne s0, t0, savereg_error
    li t0, 0x61D1161D
    bne s1, t0, savereg_error
    li t0, 0x61D2261D
    bne s2, t0, savereg_error
    li t0, 0x61D3361D
    bne s3, t0, savereg_error
    li t0, 0x61D4461D
    bne s4, t0, savereg_error
    li t0, 0x61D5561D
    bne s5, t0, savereg_error
    li t0, 0x61D6661D
    bne s6, t0, savereg_error
    li t0, 0x61D7761D
    bne s7, t0, savereg_error
    li t0, 0x61D8861D
    bne s8, t0, savereg_error
    li t0, 0x61D9961D
    bne s9, t0, savereg_error
    li t0, 0x61Daa61D
    bne s10, t0, savereg_error
    li t0, 0x61Dbb61D
    bne s11, t0, savereg_error
    la t0 static_space_for_sp
    lw t0 0(t0)
    bne sp t0 savereg_error

    lw s0 0(sp)
    lw s1 4(sp)
    lw s2 8(sp)
    lw s3 12(sp)
    lw s4 16(sp)
    lw s5 20(sp)
    lw s6 24(sp)
    lw s7 28(sp)
    lw s8 32(sp)
    lw s9 36(sp)
    lw s10 40(sp)
    lw s11 44(sp)
    addi sp sp 48
    jr ra

randomizeCalleeSavedRegs2:
    addi sp sp -48
    sw s0 0(sp)
    sw s1 4(sp)
    sw s2 8(sp)
    sw s3 12(sp)
    sw s4 16(sp)
    sw s5 20(sp)
    sw s6 24(sp)
    sw s7 28(sp)
    sw s8 32(sp)
    sw s9 36(sp)
    sw s10 40(sp)
    sw s11 44(sp)
    la t0 static_space_for_sp2
    sw sp 0(t0)

    li s0, 0x61E0061E
    li s1, 0x61E1161E
    li s2, 0x61E2261E
    li s3, 0x61E3361E
    li s4, 0x61E4461E
    li s5, 0x61E5561E
    li s6, 0x61E6661E
    li s7, 0x61E7761E
    li s8, 0x61E8861E
    li s9, 0x61E9961E
    li s10, 0x61Eaa61E
    li s11, 0x61Ebb61E

    jr ra

checkCalleeSavedRegs2:
    li t0, 0x61D0061D
    bne s0, t0, savereg_error
    li t0, 0x61D1161D
    bne s1, t0, savereg_error
    li t0, 0x61D2261D
    bne s2, t0, savereg_error
    li t0, 0x61D3361D
    bne s3, t0, savereg_error
    li t0, 0x61D4461D
    bne s4, t0, savereg_error
    li t0, 0x61D5561D
    bne s5, t0, savereg_error
    li t0, 0x61D6661D
    bne s6, t0, savereg_error
    li t0, 0x61D7761D
    bne s7, t0, savereg_error
    li t0, 0x61D8861D
    bne s8, t0, savereg_error
    li t0, 0x61D9961D
    bne s9, t0, savereg_error
    li t0, 0x61Daa61D
    bne s10, t0, savereg_error
    li t0, 0x61Dbb61D
    bne s11, t0, savereg_error
    la t0 static_space_for_sp2
    lw t0 0(t0)
    bne sp t0 savereg_error

    lw s0 0(sp)
    lw s1 4(sp)
    lw s2 8(sp)
    lw s3 12(sp)
    lw s4 16(sp)
    lw s5 20(sp)
    lw s6 24(sp)
    lw s7 28(sp)
    lw s8 32(sp)
    lw s9 36(sp)
    lw s10 40(sp)
    lw s11 44(sp)
    addi sp sp 48
    jr ra

savereg_error:
    li a0 100
    jal exit
