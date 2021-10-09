#include <stdio.h>
#include <stdlib.h>
#include <errno.h>
#include <signal.h>
#include <fcntl.h>
#include <unistd.h>
#include <string.h>

pid_t pidpadre, pidhijo;
int despertate;

void sigusr1_hand(int senial)
{
	printf ("\n\nhandler de signal sigusr1\n");
	if	(pidpadre==getpid()){
		//padre recibe señal
		despertate = 1;
		printf("padre recibe señal, despertando....\n");
	} else {
		//hijo recibe señal, do nothing
		printf("hijo recibe señal, no hago nada....\n");
	}
	return;
}


//************************************************************
int main(void){

pidpadre = getpid();

	if (signal(SIGUSR1, sigusr1_hand) == SIG_ERR ) { //instalo mi signal handler
		perror ("signal");
		exit (1);
	}

  if (!fork()){
		pidhijo=getpid();
      printf("Proc Hijo, pid=%d arrancando\n", pidhijo);
      sleep(1000);
      printf("Proc Hijo terminando\n");
   }else{
      printf("Proc Padre, pid=%d arrancando\n", pidpadre);
      while (1)
      {
	printf("Proc Padre durmiendo\n");
	while (!despertate)
	  sleep(1);
	despertate = 0;
	sleep(4);
      }
   }
   return(0);
}