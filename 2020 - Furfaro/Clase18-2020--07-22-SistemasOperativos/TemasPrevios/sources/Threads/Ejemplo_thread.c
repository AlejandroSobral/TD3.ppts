#include <stdio.h>
#include <pthread.h>

void* nuevo_thread (void* unused){
  printf("Nuevo thread creado\n");
  printf ("Proceso Nº: %d\n",getpid());
  sleep(3);
  pthread_exit(0);
  //return(NULL);
}
int main (void){
  
  pthread_t thread;
  int contador;
  contador = 10;
  printf ("Hilo principal, Proceso Nº %d,\n",getpid());
  while(contador){
//	sleep(60);
    if( pthread_create(&thread,NULL,&nuevo_thread,(void*)NULL)){
      perror("pthread_create");
      exit(1);
    }
    contador --;
    pthread_detach(thread);
  }
  pthread_exit(NULL);
}
