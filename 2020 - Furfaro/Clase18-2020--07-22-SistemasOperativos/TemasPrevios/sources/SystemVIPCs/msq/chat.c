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

    if ( (msqid = msgget(key, 0644 )) == -1)
	{
	printf ("La consola de CHAT no esta lista. Vuela a intentarlo en unos minutos\n");
	exit (1);
	}
 

    printf ("\nBienvenido al cliente de CHAT\n");
    printf ("\nEscriba su nombre:\n");
    fflush(stdin);
    buf.mtype = 1;
    gets(buf.my_nic);
    printf ("\nEscriba su mensajes seguidos por Enter:\n");
   do 
	{
	gets(buf.mtext);
	if ( msgsnd ( msqid, ( struct msgbuf * ) &buf , sizeof ( buf ), 0 )== -1)
		printf("Ocurrio un error al tratar de enviar el mensaje, reintente en unos minutos\n");
// 	wait (100);
	}		
    while (1) ;
    
    return 0;
}

