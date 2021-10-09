#include <fcntl.h>
#include <unistd.h>
#include <stdio.h>
#include <stdlib.h>
#include <errno.h>
#include <string.h>
#include <sys/types.h>
#include <sys/wait.h>

#include <pthread.h>


pthread_t t_hijoid[6];
pthread_mutex_t m=PTHREAD_MUTEX_INITIALIZER;

//////////////////////////////////////////////////
void *hijo (void *args){
unsigned int a;
char archname[20];
char pbuf[256];
int fiffd;				//para el pipe

	a=*((unsigned int *)args)+1;
	sprintf(&archname[0],"archtest.dat");
	if (a>3) sprintf(&archname[0],"fifotest");
	fiffd = open(archname, O_RDWR);

	while(1){
		sprintf(pbuf,"este es el hijo %d, thid=0x%x\n",a,pthread_self(),a);
		//printf("%s",pbuf);
		printf("%d:adquiriendo lock para fifo..\n",a);
		sleep(4);
		pthread_mutex_lock(&m);
			printf("%d:lo tengo!\n",a);
			write(fiffd,pbuf,strlen(pbuf));
			printf("%d:soltando lock para fifo..\n",a);

		pthread_mutex_unlock(&m);

		sleep(a);
	}

}

//////////////////////////////////////////////////
int main(void){
	int i;
	unsigned int ars[6];

	for (i=0;i<6;i++){
		ars[i]=i;
		pthread_create(&t_hijoid[i],NULL,hijo,&ars[i]);
	}
	while(1){
		printf("este es el padre...\n");
		sleep(5);
	}
}
