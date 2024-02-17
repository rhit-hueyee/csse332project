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
	thread_wait(NULL, i);

    return 0; // return 0 on success, non-zero on failure
}



void *increment_and_print(void *arg) {
	printf("Incrementing and Printign\n");
    int *val = (int*) arg;
    (*val)++;
    printf("Incremented value: %d\n", *val);
    exit(0);
}

void* (*increment_ptr)(void *arg) = &increment_and_print;

int test_thread_increment() {
    int* ptr = (int*) malloc(1024); 
    if (ptr == NULL) {
        printf("Stack allocation failed\n");
        return 1;
    }

    int val = -1;

    
    int tid = thread_create(increment_ptr, &val, (uint64)ptr+1024);
    if (tid < 0) {
        printf("Thread creation failed\n");
        free(ptr); // Clean up allocated stack
        return 1;
    }

    
    thread_wait(NULL, tid); 
    
    free(ptr);
    return val; // return 0 on success, non-zero on failure
}


int test_thread_print_hello_world_twice() {
    int* ptr = (int*)malloc(1024); 

    if (ptr == NULL) {
	    printf("Malloc failed\n");
        return 1;
    }

	int i = thread_create(phw_ptr, NULL, (uint64)ptr+1024);
	
	thread_wait(NULL, i);
	
	int* ptr2 = (int*)malloc(1024); 
	
	int k = thread_create(phw_ptr, NULL, (uint64)ptr2+1024);
	
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
    thread_arg->a += 5; 
    thread_arg->b -= 3; 
    printf("%s From thread: a = %d, b = %d\n", thread_arg->msg, thread_arg->a, thread_arg->b);
    exit(0); 
}

int test_thread_with_struct_arg() {
    int* ptr = (int*) malloc(1024); 
    if (ptr == NULL) {
        printf("Stack allocation failed\n");
        return 1;
    }

    
    thread_arg_t arg = {
        .a = 10,
        .b = 20,
        .msg = "Thread arg:"
    };
	
    int tid = thread_create(modify_struct_and_print, &arg, (uint64)ptr+1024); 
    if (tid < 0) {
        printf("Thread creation failed\n");
        free(ptr); // Clean up allocated stack
        return 1;
    }
	
	thread_wait(NULL, tid);
    printf("Main thread: Modified values: a = %d, b = %d\n", arg.a, arg.b);
    
    free(ptr);
	if (arg.a == 15 && arg.b == 17) {
		return 0; // Success
	} else {
		return 1; // Failure
	}
}

volatile int shared_value = 123; // Example shared variable


void *read_shared_value(void *arg) {
    int thread_id = *(int *)arg;
    printf("Thread %d: Reading shared_value = %d\n", thread_id, shared_value);
    exit(0); 
}

int test_shared_address_space() {
    int num_threads = 4;
	int thread_pids[num_threads];
    int* stacks[num_threads];

    for (int i = 0; i < num_threads; i++) {
        stacks[i] = (int*) malloc(4096); 
        if (!stacks[i]) {
            printf("Failed to allocate stack for thread %d\n", i);
            return 1;
        }
        thread_pids[i] = thread_create(read_shared_value, &i, (uint64)(stacks[i] + 1024));
		
		thread_wait(NULL, thread_pids[i]);
        free(stacks[i]); // Free allocated stack

    }

    return 0; // Success
}

volatile int shared_value2 = 0; 

void *modify_shared_value(void *arg) {
    int increment = *(int *)arg;
    shared_value2 += increment;
    printf("Thread modified shared_value to %d\n", shared_value);
    exit(0); // Use thread_exit in a real threading model, if available
}

void *read_shared_value2(void *arg) {
    sleep(10); // Delay to ensure write happens first; remove in a real scenario with proper synchronization
    printf("Thread reading shared_value = %d\n", shared_value2);
    exit(0); // Use thread_exit in a real threading model, if available
}

int test_shared_address_space_write() {
    const int num_threads = 4;
    int increment = 10;
	int thread_pids[num_threads];
	
    int* stack = (int*) malloc(1024);
    if (!stack) {
        printf("Failed to allocate stack\n");
        return 1;
    }

    if (thread_create(modify_shared_value, &increment, (uint64)(stack + 1024)) < 0) {
        printf("Failed to create write thread\n");
        return 1; // Handle failure
    }

    // Create threads to read the modified value
    for (int i = 0; i < num_threads - 1; i++) {
        int* read_stack = (int*) malloc(1024); // Allocate stack for each read thread
        if (!read_stack) {
            printf("Failed to allocate stack for read thread %d\n", i);
            return 1;
        }
        thread_pids[i] = thread_create(read_shared_value2, NULL, (uint64)(read_stack + 1024));
		thread_wait(NULL, thread_pids[i]);

    }

	if(shared_value2 == 10) {
		return 0;
	} else {
		return 1;
	}
}

//it is impossible to share stacks

//due to malloc stacks don't overlap

//




int main() {
	
	printf("STARTING TEST BASIC\n");
    if (test_thread_print_hello_world() != 0) {

	    printf("Test 1 Fail\n");
        exit(1);
    }
	printf("TEST BASIC PASSED\n");
	
	
	printf("STARTING TEST 2ND THREAD\n");
    if (test_thread_print_hello_world() != 0) {

	    printf("Test 1 Fail\n");
        exit(1);
    }
	printf("TEST 2ND THREAD PASSED\n");
	
	
	printf("STARTING TEST THREAD TWICE\n");
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
	
	printf("STARTING TEST READ SHARED ADDRESS SPACE\n");
    if (test_shared_address_space() != 0) {
        printf("Test failed\n");
        exit(1);
    }
    printf("TEST READ SHARED ADDRESS SPACE PASSED\n");
	
	printf("STARTING TESTING SHARED ADDRESS SPACE\n");
    if (test_shared_address_space_write() != 0) {
        printf("Test failed\n");
        exit(1);
    }
    printf("TEST SHARED ADDRESS SPACE PASSED\n");
	
	
	printf("百パーセント\n");


    exit(0);
}
