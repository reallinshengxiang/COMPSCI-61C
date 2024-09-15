.import ex4_discrete_fn.s

.data
# asciiz is a directive used to store strings
# asciiz will automatically append a null terminator to the end of the string
neg3:   .asciiz "f(-3) should be 6, and it is: "
neg2:   .asciiz "f(-2) should be 61, and it is: "
neg1:   .asciiz "f(-1) should be 17, and it is: "
zero:   .asciiz "f(0) should be -38, and it is: "
pos1:   .asciiz "f(1) should be 19, and it is: "
pos2:   .asciiz "f(2) should be 42, and it is: "
pos3:   .asciiz "f(3) should be 5, and it is: "

output: .word   6, 61, 17, -38, 19, 42, 5

.text
main:
	######### evaluate f(-3), should be 6 #########
    # load the address of the string located at neg3 into a0
    # this will serve as the argument to print_str
    la a0, neg3
    # print out the string located at neg3
    jal print_str
    # load the first argument to f into a0
    li a0, -3
    # load the second argument of f into a1
    # `output` is a pointer to an array that contains the possible output values of f
    la a1, output
    # execute f(-3)
    jal f
    # f will return the output of f(-3) into register a0
    # to print out the return value, we will call print_int
    # print_int expects the value that it's printing out to be in register a0
    # the output of the function is already in a0, so we don't need to move it
    jal print_int
    # print a new line
    jal print_newline

	######### evaluate f(-2), should be 61 ########
    la a0, neg2
    jal print_str
    li a0, -2
    la a1, output
    jal f
    jal print_int
    jal print_newline

	######### evaluate f(-1), should be 17 ########
    la a0, neg1
    jal print_str
    li a0, -1
    la a1, output
    jal f
    jal print_int
    jal print_newline

	######### evaluate f(0), should be -38 ########
    la a0, zero
    jal print_str
    li a0, 0
    la a1, output
    jal f
    jal print_int
    jal print_newline

	######### evaluate f(1), should be 19 #########
    la a0, pos1
    jal print_str
    li a0, 1
    la a1, output
    jal f
    jal print_int
    jal print_newline

	######### evaluate f(2), should be 42 #########
    la a0, pos2
    jal print_str
    li a0, 2
    la a1, output
    jal f
    jal print_int
    jal print_newline

	######### evaluate f(3), should be 5 #########
    la a0, pos3
    jal print_str
    li a0, 3
    la a1, output
    jal f
    jal print_int
    jal print_newline

	# passing 10 to ecall will terminate the program
    li a0, 10
    ecall

# prints out one integer
# input values: a0: the integer to print
# does not return anything
print_int:
	# to print an integer, we need to make an ecall with a0 set to 1
    # the thing that will be printed is stored in register a1
    # this line copies the integer to be printed into a1
    mv a1, a0
    # set register a0 to 1 so that the ecall will print
    li a0, 1
    # print the integer
    ecall
    # return to the calling function
    jr    ra

# prints out a string
print_str:
    mv a1, a0
    li a0, 4 # tells ecall to print the string that a1 points to
    ecall
    jr    ra

print_newline:
    li a1, '\n'
    li a0, 11 # tells ecall to print the character in a1
    ecall
    jr    ra
