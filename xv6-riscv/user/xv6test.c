#include "kernel/types.h"
#include "kernel/stat.h"
#include "user/user.h"

int test1() {
    // perform test
    return 0; // return 0 on success, non-zero on failure
}

int test2() {
    // perform another test
    return 0; // return 0 on success, non-zero on failure
}

int main() {
    if (test1() != 0) {
        exit(1); // indicate test1 failed
    }

    if (test2() != 0) {
        exit(2); // indicate test2 failed
    }

    // If we reached here, all tests passed
    exit(0);
}
