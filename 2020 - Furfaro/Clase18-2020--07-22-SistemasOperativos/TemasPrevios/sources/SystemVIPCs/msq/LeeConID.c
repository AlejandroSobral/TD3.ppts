#include <stdio.h>
#include <stdlib.h>
#include <errno.h>
#include <string.h>
#include <sys/types.h>
#include <sys/ipc.h>
#include <sys/msg.h>

struct mensaje {
    long mtype;
    char mtext[100];
};

int main(int argc, char *argv[])
{
    struct mensaje buf;
    int msqid;
    key_t key;
    long mtype;

    if ( argc < 2 ) 
    {
	printf(" \n\nError: El formato debe ser %s <MTYPE>\n\n",argv[0]);
	exit(1);
      
    }

    if ( (key = ftok("EscribeConID.c", 'M')) == -1)
    {
	perror ("Error en ftok");
	exit (1);
      
    }

    if ( (msqid = msgget(key, 0644)) == -1)
    {
	perror ("Error en msgget");
	exit (1);
    }

    printf ("\nCLIENTE\nDatos Recibidos:\n");
    do 
    {
        mtype=atoi(argv[1]);
	msgrcv(msqid, (struct msgbuf *)&buf, sizeof(buf), mtype, 0);
	printf ("%s\n",buf.mtext);
    }		
    while (1) ;
    
    return 0;
}

