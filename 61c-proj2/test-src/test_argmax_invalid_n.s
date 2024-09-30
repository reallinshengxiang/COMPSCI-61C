.import ../src/utils.s
.import ../src/argmax.s

.data

.globl main_test
.text
# main_test function for testing
main_test:

    # load 0 into a1
    li a1 0

    # call argmax function
    jal ra argmax
    # we expect argmax to exit early with code 36

    # exit normally
    li a0 0
    jal exit
