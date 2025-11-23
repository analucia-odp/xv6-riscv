#include "kernel/types.h"
#include "kernel/stat.h"
#include "user/user.h"
#include "kernel/pstat.h"

// ------ TESTING ------

#define TICKETS_A 30

void
workload(void)
{
  for(int i = 0; i < 10000000; i++) {
    asm volatile("nop");  // Operação vazia para consumir CPU
  }
}

int
main(int argc, char *argv[])
{

  int pid_a;
  struct pstat pst;
  
  printf("=== Lottery Scheduler Test - FORK ===\n");
  printf("Process A: %d tickets \n", TICKETS_A);
  printf("\n");
  
  // Criar Processo A com 30 tickets
  pid_a = fork();
  if(pid_a == 0) {
    settickets(TICKETS_A);

    int pid_child = fork();
    if(pid_child == 0) 
    {
        while(1){
        workload();
    }
    }

    printf("Process B PID: %d\n", pid_child);

    // Loop infinito fazendo trabalho
    while(1){
      workload();
    }
    kill(pid_child);
    wait(0);
  }

  printf("Process A PID: %d\n", pid_a);

  for(volatile int i = 0; i < 1000000; i++);  // Esperar um pouco antes de coletar dados

  if(getpinfo(&pst) < 0) {
      printf("getpinfo failed\n");
      exit(1);
  }

  for (int i = 0; i < NPROC; i++) {
      if(pst.inuse[i]) {
          printf("PID: %d, Tickets: %d, Ticks: %d\n", pst.pid[i], pst.tickets[i], pst.ticks[i]);
      }
  }
  
  // Mata os processos filhos
  kill(pid_a);
  
  wait(0);
  
  printf("\nTest completed!\n");
  exit(0);

}