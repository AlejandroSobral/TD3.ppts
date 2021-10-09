/**
 * \file waitchld.c
 * \brief Captura de SIGCHLD para evitar dejar hijos defunct sin bloquear permanentemente al proceso padre
 * \details Este programa utiliza la funición signal para reemplazar el hander de la señal SIGCHLD enviada por cada hijo cuando ejecuta la syscall exit(). El objetivo es solucionar la generación de procesos hijos que quedan defunct en servidores de loop infinito, sin bloquear mediante wait () al proceso padre.
 */

#include <stdio.h>
#include <stdlib.h>
#include <errno.h>
#include <signal.h>
#include <fcntl.h>
#include <unistd.h>
#include <string.h>
#include <sys/types.h>
#include <sys/wait.h>

/* handler de SIGCHLD*/
void sigchld_hand(int senial)
{
 	printf("Estamos en el handler de SIGCHLD del proceso padre pid=%d....\n",getpid());
 	printf("Antes del wait sobre el hijo, dormimos al proceso padre 10 seg. para ver que el hijo queda zombie durante ese tiempo....\n");
 	sleep(20);
 	printf("Ahora hacemos wait sobre el hijo...\n");
	wait(NULL);
 	printf("Saliendo de handler.\nRevisar lista de procesos y ver que hijo terminó....\n");
	return;
}

//************************************************************/
int main(void){
  pid_t  pidhijo,pidpadre;
  pidpadre = getpid();
  /*instalo signal handler*/
  if (signal(SIGCHLD, sigchld_hand) == SIG_ERR )
  {
    perror ("signal");
    exit (1);
  }

  if (!fork())
  {
    pidhijo=getpid();
    printf("Proc Hijo, pid=%d arrancando\n",pidhijo);
    sleep(20);/* Tenemos 20 segundos, para visualizar con ps en otra termial que esta durmiendo */
    printf("Proc Hijo pid=%d terminando\n",pidhijo);
  }
  else
  {
    printf("Proc Padre, pid=%d arrancando y a dormir\n",pidpadre);
    while (1)
      sleep(1);
  }
  return(0);
}

