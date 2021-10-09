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
/* La union se define al incluir <sys/sem.h> */
#else
/* Segn X/OPEN la tenemos que definir a mano */
	union semun {
	int	val;			/* valor para SETVAL */
	struct	semid_ds *buf;		/* buffer para IPC_STAT e IPC_SET */
	unsigned short int *array;	/* valor para GETALL y SETALL */
	struct seminfo *__buf;		/* buffer para IPC_INFO */
	};
#endif
        union semun arg;

        if ((key = ftok("semdemo.c", 'J')) == -1) {
            perror("ftok");
            exit(1);
        }

        /* crea el semáforo y lo setea en 1 */
        if ((semid = semget(key, 1, 0666 | IPC_CREAT)) == -1) {
            perror("semget");
            exit(1);
        }

        /* inicializa el semáforo #0 a 1: */
        arg.val = 1;
        if (semctl(semid, 0, SETVAL, arg) == -1) {
            perror("semctl");
            exit(1);
        }

        return 0;
}
