#include "kernel/types.h"
#include "kernel/stat.h"
#include "user/user.h"
#include <stddef.h>
#include "defs.h"


void *print_hello_world(void *arg) {
    printf("Hello World\n");
    return NULL;
}

int test_print_hello_world() {
    int* ptr = (int*)malloc(4096); 
    printf("Malloced\n");

    if (ptr == NULL) {
	    printf("Malloc failed\n");
        return 1;
    }

    printf("creating thread\n");

    thread_create(print_hello_world, NULL, (uint64)ptr);

    return 0; // return 0 on success, non-zero on failure
}



int test2() {
    return 0; // return 0 on success, non-zero on failure
}

int main() {
	printf("Beginning Test 1\n");
    if (test_print_hello_world() != 0) {
	    printf("Test 1 Fail\n");
        exit(1);
    }

    if (test2() != 0) {
        exit(2);
    }

    exit(0);
}
