#include <stdio.h>
#include <stdlib.h>
#include <errno.h>
#include <sys/types.h>
#include <sys/ipc.h>
#include <sys/msg.h>
#include <string.h>

struct mensaje {
    long mtype;
    char mtext[100];
};

int main(int argc, char *argv[])
{
    struct mensaje buf;
    int msqid;
    key_t key;

    if ( argc < 3 ) 
    {
	printf(" \n\nError: El formato debe ser %s <MTYPE> <Texto>\n\n",argv[0]);
	exit(1);
    }


    if ( (key = ftok("EscribeConID.c", 'M')) == -1)
    {
	perror ("Error en ftok");
	exit (1);
    }

    if ( (msqid = msgget(key, 0644 | IPC_CREAT)) == -1)
    {
	perror ("Error en msgget");
	exit (1);
     }

     buf.mtype=atoi(argv[1]);
     strcpy (buf.mtext, argv[2]);
     if ( ( msgsnd ( msqid, ( struct msgbuf * ) &buf , sizeof ( buf ), 0 ) ) == -1)
     {
	perror ("Error en msgsnd");
	exit (1);
     }

     return 0;
}

