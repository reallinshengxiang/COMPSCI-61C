.import ../src/utils.s
.import ../src/relu.s

.data

.globl main_test
.text
# main_test function for testing
main_test:

    # load -1 into a1
    li a1 -1

    # call relu function
    jal ra relu
    # we expect relu to exit early with code 36

    # exit normally
    li a0 0
    jal exit
