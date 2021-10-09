#include <pthread.h>
#include <stdio.h>
#include <unistd.h>
#include <stdlib.h>

void*  execute ( )
{
	//execlp ("ls","ls","-las",NULL);
        system ("ls -las");
	return (void *) NULL;
}

int main (){
	pthread_t thread;

	pthread_create(&thread,NULL,&execute,0);
	pthread_join(thread,0);
        printf ("El thread concluyó\n");
        printf ("Este proceso se autodestruirá en 5 segundos\n");
        sleep (5);
	printf("booom!!\n");

	return 0;
}
