#include "kernel/types.h"
#include "kernel/stat.h"
#include "user/user.h"
#include <stddef.h>
#include "defs.h"


/*
        _.-^^---....,,--       
     _--                  --_  
    <                        >)
    |                         | 
     \._                   _./  
        ```--. . , ; .--'''       
              | |   |             
           .-=||  | |=-.   
           `-=#$%&%$#=-'   
              | ;  :|     
     _____.,-#%&$@%$&#~,._____

*/


void *print_hello_world(void *arg) {
    printf("Hello World\n");
    exit(0);
    //return NULL;
	
}

void* (*phw_ptr)(void *arg) = &print_hello_world;

int test_thread_print_hello_world() {
    int* ptr = (int*)malloc(1024); 

    if (ptr == NULL) {
	    printf("Malloc failed\n");
        return 1;
    }

	int i = thread_create(phw_ptr, NULL, (uint64)ptr+1024);
	//int *j = &i;
	thread_wait(NULL, i);

    return 0; // return 0 on success, non-zero on failure
}



void *increment_and_print(void *arg) {
	printf("Incrementing and Printign\n");
    int *val = (int*) arg;
    (*val)++;
    printf("Incremented value: %d\n", *val);
    exit(0); // Threads should call thread_exit() in real scenarios if available
}

void* (*increment_ptr)(void *arg) = &increment_and_print;

int test_thread_increment() {
    int* ptr = (int*) malloc(1024); // Allocate stack for the thread
    if (ptr == NULL) {
        printf("Stack allocation failed\n");
        return 1;
    }

    int val = -1; // Example value to increment

    //printf("Creating thread to increment value\n");
    int tid = thread_create(increment_ptr, &val, (uint64)ptr+1024); // Adjusted stack pointer to simulate stack top
    if (tid < 0) {
        printf("Thread creation failed\n");
        free(ptr); // Clean up allocated stack
        return 1;
    }

    //int *retval = &tid;
    thread_wait(NULL, tid); // Wait for the thread to finish
    //printf("Thread finished with incremented value: %d\n", val);
    
    free(ptr); // Clean up allocated stack
    return val; // Success
}


int test_thread_print_hello_world_twice() {
    int* ptr = (int*)malloc(1024); 

    if (ptr == NULL) {
	    printf("Malloc failed\n");
        return 1;
    }

	int i = thread_create(phw_ptr, NULL, (uint64)ptr+1024);
	//int *j = &i;
	//wait(j);
	
	thread_wait(NULL, i);
	
	int* ptr2 = (int*)malloc(1024); 
	
	int k = thread_create(phw_ptr, NULL, (uint64)ptr2+1024);
	//int *l = &k;
	//wait(l);
	thread_wait(NULL, k);
    return 0; // return 0 on success, non-zero on failure
}

typedef struct {
    int a;
    int b;
    char msg[100];
} thread_arg_t;

void *modify_struct_and_print(void *arg) {
    thread_arg_t *thread_arg = (thread_arg_t *)arg;
    thread_arg->a += 5; // Modify field a
    thread_arg->b -= 3; // Modify field b
    printf("%s From thread: a = %d, b = %d\n", thread_arg->msg, thread_arg->a, thread_arg->b);
    exit(0); // Ideally, threads should use thread_exit if available
}

int test_thread_with_struct_arg() {
    int* ptr = (int*) malloc(1024); // Allocate stack for the thread
    if (ptr == NULL) {
        printf("Stack allocation failed\n");
        return 1;
    }

    // Initialize the structure
    thread_arg_t arg = {
        .a = 10,
        .b = 20,
        .msg = "Thread arg:"
    };

    // Create the thread, passing the structure as an argument
    printf("Creating thread to modify structure\n");
    int tid = thread_create(modify_struct_and_print, &arg, (uint64)ptr+1024); // Adjusted stack pointer to simulate stack top
    if (tid < 0) {
        printf("Thread creation failed\n");
        free(ptr); // Clean up allocated stack
        return 1;
    }
	
	thread_wait(NULL, tid);

    //int *retval = &tid;
    //wait(retval); // Wait for the thread to finish
    printf("Main thread: Modified values: a = %d, b = %d\n", arg.a, arg.b);
    
    free(ptr); // Clean up allocated stack
    return 0; // Success
}



int main() {
	
	printf("BEGINNING TEST BASIC\n");
    if (test_thread_print_hello_world() != 0) {

	    printf("Test 1 Fail\n");
        exit(1);
    }
	printf("TEST BASIC PASSED\n");
	
	
	printf("BEGINNING TEST 2ND THREAD\n");
    if (test_thread_print_hello_world() != 0) {

	    printf("Test 1 Fail\n");
        exit(1);
    }
	printf("TEST 2ND THREAD PASSED\n");
	
	
	printf("TEST THREAD TWICE\n");
	if (test_thread_print_hello_world_twice() != 0) {
		printf("Test failed\n");
        exit(1);
	}
	printf("TEST THREAD TWICE PASSED\n");

  

    printf("STARTING TEST FOR THREAD SINGLE ARGUMENT HANDLING\n");
    if (test_thread_increment() != 0) {
        printf("Test failed\n");
        exit(1);
    }
    printf("TEST SINGLE ARG PASSED\n");
	
	
	 printf("STARTING TEST FOR PASSING A STRUCTURE TO A THREAD\n");
    if (test_thread_with_struct_arg() != 0) {
        printf("Test failed\n");
        exit(1);
    }
    printf("TEST MULTI ARG PASSED\n");
	
	printf("百パーセント\n");


    exit(0);
}
