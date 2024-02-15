#include "kernel/types.h"
#include "kernel/stat.h"
#include "user/user.h"
#include <stddef.h>
#include "defs.h"


void *print_hello_world(void *arg) {
    printf("Hello World\n");
    return NULL;
}

void* (*phw_ptr)(void *arg) = &print_hello_world;

int test_print_hello_world() {
	print_hello_world(NULL);
	return 0;
}

int test_thread_print_hello_world() {
    int* ptr = (int*)malloc(4096); 
    printf("Malloced\n");

    if (ptr == NULL) {
	    printf("Malloc failed\n");
        return 1;
    }

    printf("creating thread\n");
	
	printf("The eepy sea is %d\n", (uint64)phw_ptr);
  printf("the raw phw is %d\n", *phw_ptr);
  printf("the juicer juice is %d\n", &phw_ptr);
	int i = thread_create(phw_ptr, NULL, (uint64)ptr);
  sleep(10);

    printf("Successfully created teh threahd\n");

    return i; // return 0 on success, non-zero on failure
}



int test2() {
    return 0; // return 0 on success, non-zero on failure
}

int main() {
	if (test_print_hello_world() != 0) {
		exit(1);
		
	}
	
	printf("Beginning Test 1\n");
    if (test_thread_print_hello_world() != 0) {

	    printf("Test 1 Fail\n");
        exit(1);
    }
	printf("Finished Test 1\n");

    if (test2() != 0) {
        exit(2);
    }

    exit(0);
}
