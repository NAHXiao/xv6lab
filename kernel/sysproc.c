#include "types.h"
#include "riscv.h"
#include "defs.h"
#include "date.h"
#include "param.h"
#include "memlayout.h"
#include "spinlock.h"
#include "proc.h"

// #include "syscall.h"
// #define trace_syscall(SYSCALL) (((myproc()->trace_mask>>SYSCALL) & 1))
uint64
sys_exit(void)
{
  int n;
  if(argint(0, &n) < 0)
    return -1;
  exit(n);
  return 0;  // not reached
}

uint64
sys_getpid(void)
{
  return myproc()->pid;
}

uint64
sys_fork(void)
{
  return fork();
}

uint64
sys_wait(void)
{
  uint64 p;
  if(argaddr(0, &p) < 0)
    return -1;
  return wait(p);
}

uint64
sys_sbrk(void)
{
  int addr;
  int n;

  if(argint(0, &n) < 0)
    return -1;
  addr = myproc()->sz;
  if(growproc(n) < 0)
    return -1;
  return addr;
}

uint64
sys_sleep(void)
{
  int n;
  uint ticks0;

  if(argint(0, &n) < 0)
    return -1;
  acquire(&tickslock);
  ticks0 = ticks;
  while(ticks - ticks0 < n){
    if(myproc()->killed){
      release(&tickslock);
      return -1;
    }
    sleep(&ticks, &tickslock);
  }
  release(&tickslock);
  return 0;
}

uint64
sys_kill(void)
{
  int pid;

  if(argint(0, &pid) < 0)
    return -1;
  return kill(pid);
}

// return how many clock tick interrupts have occurred
// since start.
uint64
sys_uptime(void)
{
  uint xticks;

  acquire(&tickslock);
  xticks = ticks;
  release(&tickslock);
  return xticks;
}


uint64 sys_trace(void){
  //
int trace_mask;
  if(argint(0, &trace_mask) < 0){
    return -1;
  }
  // myproc()->trace_mask|=trace_mask;
  myproc()->trace_mask=trace_mask;
  return 0;
}

extern uint64 get_used_procnum();
extern uint64 get_kmemfreespc();
#include "sysinfo.h"
uint64 sys_sysinfo(void){
  uint64 addr=0;
  //  struct sysinfo *si=0;
  //  if(argaddr(0,(uint64*)&si)<0){
   if(argaddr(0,&addr)<0){
      return -1;
   }
   if(addr==0){
    return -1;
   }
   struct sysinfo si = {
      4096*get_kmemfreespc(),
      get_used_procnum()
   };
  //  printf("freeprocnum:%d,kmemfree:%d\n",si.nproc,si.freemem);
 if (copyout(myproc()->pagetable,addr , (char *)&si, sizeof(si)) < 0){
    return -1;
 }
  return 0;
}