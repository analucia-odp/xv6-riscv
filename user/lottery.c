#include "kernel/types.h"
#include "kernel/stat.h"
#include "user/user.h"
#include "kernel/pstat.h"

// ------ TESTING ------

// #define TICKETS_A 30
// #define TICKETS_B 20
// #define TICKETS_C 10

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
  if (argc < 4){
        printf("[lottery] Error - provided number process tickets\n");
        exit(1);
  }

  int tickets_a = atoi(argv[1]);
  int tickets_b = atoi(argv[2]); 
  int tickets_c = atoi(argv[3]);
  if (tickets_a <= 0 || tickets_b <= 0 || tickets_c <= 0){
        printf("[lottery] Error - number of tickets must be greater than 0\n");
        exit(1);
  }

  int pid_a, pid_b, pid_c;
  struct pstat pst;
  
  printf("=== Lottery Scheduler Test ===\n");
  printf("Process A: %d tickets \n", tickets_a);
  printf("Process B: %d tickets \n", tickets_b);
  printf("Process C: %d tickets \n", tickets_c);
  printf("Total tickets: %d\n", tickets_a + tickets_b + tickets_c);
  printf("\n");
  
  // Criar Processo A com 30 tickets
  pid_a = fork();
  if(pid_a == 0) {
    settickets(tickets_a);
    // Loop infinito fazendo trabalho
    while(1){
      workload();
    }
  }

  printf("Process A PID: %d\n", pid_a);

  // Criar Processo B com 20 tickets
  pid_b = fork();
  if(pid_b == 0) {
    settickets(tickets_b);
    while(1) {
      workload();
    }
  }

  printf("Process B PID: %d\n", pid_b);

  // Criar Processo C com 10 tickets
  pid_c = fork();
  if(pid_c == 0) {
    settickets(tickets_c);
    while(1) {
      workload();
    }
  }

  printf("Process C PID: %d\n", pid_c);

  for(volatile int i = 0; i < 500000000; i++);  // Esperar um pouco antes de coletar dados

  if(getpinfo(&pst) < 0) {
      printf("getpinfo failed\n");
      exit(1);
  }

  int total_ticks = 0;
  int ticks_a = 0, ticks_b = 0, ticks_c = 0;

  for (int i = 0; i < NPROC; i++) {
      if(pst.inuse[i]) {
          printf("PID: %d, Tickets: %d, Ticks: %d\n", pst.pid[i], pst.tickets[i], pst.ticks[i]);
          if (pst.pid[i] == pid_a) {
              ticks_a = pst.ticks[i];
          } else if (pst.pid[i] == pid_b) {
              ticks_b = pst.ticks[i];
          } else if (pst.pid[i] == pid_c) {
              ticks_c = pst.ticks[i];
          }
      }
  }

  total_ticks = ticks_a + ticks_b + ticks_c;

  printf("Total ticks across all processes A, B and C: %d\n", total_ticks);

  printf("Process A: %d ticks\n", ticks_a);
  
  printf("Process B: %d ticks\n", ticks_b);
  
  printf("Process C: %d ticks\n", ticks_c);
  
  // Mata os processos filhos
  kill(pid_a);
  kill(pid_b);
  kill(pid_c);
  
  wait(0);
  wait(0);
  wait(0);
  
  printf("\nTest completed!\n");
  exit(0);

}