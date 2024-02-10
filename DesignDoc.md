
# Custom Thread Library
## CUSTOM xv6 THREAD IMPLEMENTATION

[Project Page](https://www.rose-hulman.edu/class/csse/csse332/2324b/labs/project/)

### What is a thread:
- Needs to have its individual PC address
- Needs its own individual registers
- Needs its own individual stack space
- Needs to fit all of that stuff somewhere into an address space shared with other threads
- Needs to be able to run multiple at once

## xv6 Information

### Content of Page Table within a process in xv6
There is a page directory and a page table, each of length 1024 and storing 32 bits per row. The first (last?) 10 bits of the virtual address map to a row in the page directory. The first 20 bits of this number then point to a page table. The middle 10 bits of the virtual address then point to a row in this page table, where the PPN is the first 20 bits of the physical address and the final 12 bits of the virtual address are the rest of the physical address. The extra flag bits in the page directory and page table specify extra information about what can be done to the spots in memory.

### `trapframe` and `trampoline` mappings
The “trapframe” is used to save the state of a process when a system interrupt, otherwise known as a trap, is activated. It is mapped just below the trampoline.
The “trampoline” is used to switch between user and kernel space . It is mapped in both user and kernel page tables at the highest point, in the same address. It is mapped in the user page table so that traps can start executing in “supervisor mode”. It is mapped in kernel page table so that the trap handler can still run after switching to kernel page table. 

### Context Switching

Context switching is switching between two different processes. The xv6 scheduler uses a dedicated thread for each CPU. The process of switching involves saving the previous’ threads registers, and then restoring the new threads registers. This means the stack pointer and program counter are also restored, which allows the processes to share the same stack. 
The function swtch is called by a CPUs kernel and saves all of the current registers, and returns to the registers of the next process. The set of registers is called the context, and is its own struct. swtch takes two arguments, the old context and the new context. swtch stores the old context registers on the stack. The ra register stores the return address from where swtch was called. Then swtch restores the other registers from its provided context, including the old old ra which goes back to where the new context was saved.

Example usage:
```
void
scheduler(void)
{
  struct proc *p;
  struct cpu *c = mycpu();
  
  c->proc = 0;
  for(;;){
    // Avoid deadlock by ensuring that devices can interrupt.
    intr_on();

    for(p = proc; p < &proc[NPROC]; p++) {
      acquire(&p->lock);
      if(p->state == RUNNABLE) {
        // Switch to chosen process.  It is the process's job
        // to release its lock and then reacquire it
        // before jumping back to us.
        p->state = RUNNING;
        c->proc = p;
        swtch(&c->context, &p->context);

        // Process is done running for now.
        // It should have changed its p->state before coming back.
        c->proc = 0;
      }
      release(&p->lock);
    }
  }
}
```

Definition of swtch:

```
#   void swtch(struct context *old, struct context *new);
# 
# Save current registers in old. Load from new.	

.globl swtch
swtch:
        sd ra, 0(a0)
        sd sp, 8(a0)
        sd s0, 16(a0)
        sd s1, 24(a0)
        sd s2, 32(a0)
        sd s3, 40(a0)
        sd s4, 48(a0)
        sd s5, 56(a0)
        sd s6, 64(a0)
        sd s7, 72(a0)
        sd s8, 80(a0)
        sd s9, 88(a0)
        sd s10, 96(a0)
        sd s11, 104(a0)

        ld ra, 0(a1)
        ld sp, 8(a1)
        ld s0, 16(a1)
        ld s1, 24(a1)
        ld s2, 32(a1)
        ld s3, 40(a1)
        ld s4, 48(a1)
        ld s5, 56(a1)
        ld s6, 64(a1)
        ld s7, 72(a1)
        ld s8, 80(a1)
        ld s9, 88(a1)
        ld s10, 96(a1)
        ld s11, 104(a1)
        
        ret
```

### Main Challenge of Sharing Page Tables
If multiple processes shared the same page table, there is a risk of them changing the memory of each other. Trapframe and Trampoline shouldn't be shared because they help us bounce between each thread. Those shouldn't be shared between thread.

Each thread has own page table and it has its own trapframe and trampoline. Other spots are mapped to the same place. Use same thing we did in copy on write
creating thread -> copy on write minus write protection. Data changes are okay, adding a new entry to page table, whole gang gets updated. Do not start from, start from assumption threads don't use new pages, get to work with free allocated stuff.

Passing argument -> worry about passing pointer to a struct, but in kernel you can't access structure b/c kernal has it's own page table, two functions to ccopyin and copyout. 

Thread termination, make assumptions that make things easy. Have them call a function to exit -> undefined if not.


### Can we use uvmcopy?
No, as the threads need to share the same memory, which forking does not want to do. Threads want to have access to each other's memory spaces and can use locks to ensure that race conditions don't happen.

## Our threads

### Function API

`thread_create(thread_t thread,  void *(*start_routine)(void *), void *arg)`

`thread_join(thread_t thread)`

### Joining Threads

`thread_ join` would cause the current thread to wait for a signal from the passed in thread. All threads would send out a unique signal when finished, and listening threads would then have their flag set to allow for continued operation.

### Relationship between Threads

We will have an initial main thread that knows about all created threads. All threads will stem from the main thread.

### Thread Stack

The threads will only be able to occupy a limited size of the stack. The first thread will start at the top of the allotted space. The second stack will spawn halfway between the first thread and end of the allotted stack. We will keep cutting in half and chucking the thread data. Stack location will be stored in thread metadata.  If the stacks collide the behavior will be undefined. When a given thread terminates, its slot in the thread ladder will be marked as unoccupied, and occupying it will take priority over creating a new slot in the stack. This solution will probably be easier to implement if we limit the number of threads to like 16 or so.


### How to Pass Argument to Threads:

One pointer can be passed in, pointing to anywhere in the heap. We will use the first argument register.
Or idea: we just have the previous thread set up a new variable on the stack with whatever data the new thread needs and then have the new thread just use that. Not passing anything in, just setting it up in shared memory prior to the create call.

### Thread Termination

When a thread terminates, its shared pages are not freed. The pages aren’t affected by a single thread’s termination, only when the main thread terminates. The main thread also no longer keeps track of the thread. The thread’s space on the stack gets clear
