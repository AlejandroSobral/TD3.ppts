#include <stdio.h>
#include <unistd.h>
#include <pthread.h>

FILE *fd,*fd_log;
pthread_mutex_t mutex;

void logger (char *str){
	int bytes;
        /* esta funcion copia en un archivo lo que seria los printfs*/

        bytes = fprintf(fd_log,"%s",str);	
	
	fprintf(fd_log,"Cant de bytes = %d\n",bytes);

        //exit(0);
}

void *escribe (void *data){
        char str[30];

        strcpy(str,(char*)data);
        logger("Cadena a escribir por mi que soy un thread:");
	logger(str);
	logger("\n");

        /* se lock el mutex para que nadie mas escriba en el archivo */
        pthread_mutex_lock(&mutex);
        logger("Mutex locked\n");
        sleep(3);
        logger("Escribo en el archivo\n");
        if(fprintf(fd,"%s",str) < 0 ){
                perror("fwrite");
                pthread_mutex_unlock(&mutex);
                pthread_exit(0);
        }
        /* se unlock el mutex para que otro thread pueda usar el archivo*/
        pthread_mutex_unlock(&mutex);
        logger("Mutex unlocked\n");
        
        pthread_exit(0);
}

int main (){

        int pid;
        int cant;
	int count;
        pthread_t id;
        char buf[30];

	
	printf("Abro archivo para logs\n");
	if ( (fd_log = fopen("logs.txt","w+")) == NULL ){
		perror("fopen");
                exit(1);
	}

	printf("Abro archivo donde escribiran los threads\n");
	if ( ((fd = fopen("archivo.txt","w+")) == NULL) ){
                perror("fopen");
		exit(1);
	}

	pid = fork();

        if(pid > 0){
                printf("Soy el proceso padre,ahora muero\n");
                exit(0);
        }
        if(pid < 0){
                perror("fork");
                exit(1);
        }

        /* Proceso hijo, futuro demonio */
        setsid();

        logger("Inicializo el mutex\n");
        pthread_mutex_init(&mutex,NULL);
	logger("Mutex inicializado\n");

        count = 20;
        cant = 1;

        while(count){
                
		sprintf(buf,"Soy el thread numero %d\n",cant);
		//strcpy(buf,"Soy el thread numero");
		logger("Lo que le paso al thread es:");
                logger(buf);
		logger("\n");

                /*creacion del thread*/
                if(pthread_create(&id,NULL,&escribe,buf)!= 0 ){
                        logger("Error en la funcion pthread_create");
                        exit(1);
                }
                cant++;
                count--;
                sleep(2);
        }
	fclose(fd_log);
	fclose(fd);
	exit(0);

}

