#include "proc.h"
#include "param.h"
#include "spinlock.h"
#include "memlayout.h"
#include "spinlock.h"

struct thread*
thread_create(void (*fn)(void), void *arg){
struct proc *np;
struct proc *p = myproc();

if((np = allocproc()) == 0){
  return -1;
}

np->sz = p->sz//same size
np->paent_process = p;//setting parent
np->is_thread = 1;//we have to add this to attribute to proc.c

//np->trapframe->sp = alloc_stack(np->pagetable, np->sz);
//
//we dont know how to get this unique pointer for stack space

if(np->trapframe->sp == 0){
//this is for handling error when creating new sp
freeproc(np);
return -1
}

//code below is trying to set up contetx for the function provided in the thread call
//memset(&np->context, 0, sizeof(np->context));

//do we need to set this stuff in both trap frame and context?

//np->trapframe->
//how do we set up trap frame?

release(&np->lock);
