#include <stdio.h>
#include <stdlib.h>
#include <errno.h>
#include <sys/types.h>
#include <sys/ipc.h>
#include <sys/sem.h>

int main(void)
{
  key_t key;
  int semid;
  #if defined (__GNU_LIBRARY__) && !defined (_SEM_SEMUN_UNDEFINED)
  /* la union semun se define al incluir <sys/sem.h> */
  #else
  /* segun X/POSIX tenemos que definirla nosotros mismos */
  union semun 
  {
    int	val;			/* valor para SETVAL */
    struct semid_ds *buf;	/* buffer para IPC_STAT, IPC_SET */
    unsigned short int *array;	/* array pata GETALL SETALL */
    struct seminfo *__buf;	/* buffer para IPC_INFO */
  };
#endif
  union semun arg;
	
  if ((key = ftok("semdemo.c", 'J')) == -1)
  {
    perror("ftok");
    exit(1);
  }

  //Obtenemos semáforo
  if ((semid = semget(key, 1, 0)) == -1) 
  {
    perror("semget");
    exit(1);
  }
  //Lo borramos
  if (semctl(semid, 0, IPC_RMID, arg) == -1) 
  {
    perror("semctl");
    exit(1);
  }
  return 0;
}