#include <stdio.h>
#include <stdlib.h>
#include <errno.h>
#include <sys/types.h>
#include <sys/ipc.h>
#include <sys/msg.h>
#include <sys/wait.h>

struct mensaje {
    long mtype;
    char my_nic[15];
    char mtext[100];
};

int main(int argc, char *argv[])
{
    struct mensaje buf;
    int msqid;
    key_t key;
    long mtype;
    int a;


    if ( (key = ftok("chat.c", 'M')) == -1)
	{
	perror ("Error en ftok");
	exit (1);
	}

    if ( (msqid = msgget(key, 0644 | IPC_CREAT)) == -1)
	{
	perror ("Error en msgget");
	exit (1);
	}
 

    printf ("\nCONSOLA DE CHAT\n");
   do 
	{
	msgrcv(msqid, (struct msgbuf *)&buf, sizeof(buf), 1, 0);
	printf ("%s: %s\n",buf.my_nic, buf.mtext);
// 	wait (100);
	}		
    while (1) ;
    
    return 0;
}

