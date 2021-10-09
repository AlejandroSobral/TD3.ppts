#include <pthread.h>

pthread_t t_hijoid[6];
pthread_mutex_t m=PTHREAD_MUTEX_INITIALIZER;
pthread_cond_t c1 = PTHREAD_COND_INITIALIZER;


void *hijo (void *args){
int a;

    a = *((int*)args)+1;
    sleep(1);
    while(1){
        printf("este es el hijo %d, thid=0x%x\n",a,pthread_self());
	printf("%d:Durmiendo %d segundos antes de sincronizarme....\n",a,a);
	sleep(a);
	printf("%d: alcanzo la barrera de sincronizacion\n",a);
        pthread_mutex_lock(&m);
	    pthread_cond_wait(&c1,&m);
        pthread_mutex_unlock(&m);
    }
}
/////////////////////////////////////////////////
//////////////////////////////////////////////////
int main(void){
int i;
int ars[6];


    for (i=0;i<6;i++){
        ars[i]=i;
        pthread_create(&t_hijoid[i],NULL,hijo,&ars[i]);
    }
    while(1){
	pthread_cond_init(&c1,NULL);
	sleep(1);
        printf("este es el padre...\n");
	printf("presione ENTER para sincronizar a los threads hijos\n");
        getchar();
	pthread_cond_broadcast(&c1);
    }
    return 0;
}

