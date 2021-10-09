#include <pthread.h>
#include <stdio.h>
#include <string.h>
#include <unistd.h>
#include <stdlib.h>

#define CANT 20
FILE *fd;
pthread_mutex_t mutex;

void *escribe (void *data){
  char *str = malloc (30);
/* lockea el mutex para que nadie mas escriba en el archivo */
  pthread_mutex_lock(&mutex);
  printf("Mutex locked\n");
  sprintf(str,"Soy el thread numero %ld\n",(unsigned long)pthread_self());
  printf("El contenido del buffer es: %s\n",str);
  printf("Escribo en el archivo\n");
  if(fprintf(fd,"%s",str) < 0 ){
    perror("fwrite");
    pthread_mutex_unlock(&mutex);
    pthread_exit(0);
  }
  free (str);
/* se unlock el mutex para que otro thread pueda usar el archivo*/
  pthread_mutex_unlock(&mutex);
  printf("Mutex unlocked\n");
  pthread_exit(0);
}

int main(){
  int count,cant;
  pthread_t id[CANT];
  printf("Abro archivo al que escribiran los threads\n");
  if ( ((fd = fopen("archivo.txt","w+")) == NULL) ){
    perror("fopen");
    exit (1);
  }
  printf("Inicializo el mutex\n");
  pthread_mutex_init(&mutex,NULL);
  for(count = CANT, cant = 1; count ; cant++, count--){
/*creacion del thread*/
    if(pthread_create(&id[cant-1],NULL,&escribe,&cant)!= 0 ){
      perror("pthread_create");
      exit(1);
    }
  }
  while (--cant)
    pthread_join(id[cant-1],NULL);
  printf("Cerré archivo\n");
  fclose(fd);
  exit(0);
}
