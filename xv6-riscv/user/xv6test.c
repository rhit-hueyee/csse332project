#include "kernel/types.h"
#include "kernel/stat.h"
#include "user/user.h"
#include <stddef.h>
#include "defs.h"


void *print_hello_world(void *arg) {
    printf("Hello World\n");
    return NULL; // Since we're not really returning anything meaningful
}

int test_print_hello_world() {
    // perform test
    int* ptr = (int*)malloc(4096); // It's a good practice to cast the return value of malloc in C
    printf("Malloced\n");

    if (ptr == NULL) {
	    printf("Malloc failed\n");
        return 1; // indicate memory allocation failure
    }

    printf("creating thread\n");

    thread_create(print_hello_world, NULL, (uint64)ptr);

    return 0; // return 0 on success, non-zero on failure
}



int test2() {
    // perform another test
    return 0; // return 0 on success, non-zero on failure
}

int main() {
	printf("Beginning Test 1\n");
    if (test_print_hello_world() != 0) {
	    printf("Test 1 Fail\n");
        exit(1); // indicate test1 failed
    }

    if (test2() != 0) {
        exit(2); // indicate test2 failed
    }

    // If we reached here, all tests passed
    exit(0);
}
