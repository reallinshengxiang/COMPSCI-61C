.import ../src/utils.s
.import ../src/classify.s
.import ../src/argmax.s
.import ../src/dot.s
.import ../src/matmul.s
.import ../src/read_matrix.s
.import ../src/relu.s
.import ../src/write_matrix.s

.data

.globl main_test
.text
# main_test function for testing
main_test:

    # load 1 into a2
    li a2 1

    # call classify function
    jal ra classify
    # we expect classify to exit early with code 31

    # exit normally
    li a0 0
    jal exit
