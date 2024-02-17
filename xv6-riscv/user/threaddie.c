#include "kernel/types.h"
#include "kernel/stat.h"
#include "user/user.h"
#include <stddef.h>
#include "defs.h"

void *printRepeater(void *arg){
while(1){
printf("im running bruh \n");
sleep(7);
}
exit(0);
}

void* (*prp)(void *arg) = &printRepeater;

void testThreadDeath(void *arg){
int* ptr = (int*)malloc(1024);
if(ptr == NULL){
exit(1);
}
int i = thread_create(prp, &x, (uint64)ptr+1024);
i = i + 1;//do this so i is used
exit(0);
}

int main(){
int forker = fork();
if(forker == 0){
testThreadDeath(NULL);
}
else{
sleep(50);
kill(forker);
printf("test complete blud\n");
return 0;
}
return 0;
}
