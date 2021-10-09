/**
 * \file shmdemo.c
 * \brief Ejemplo de uso de una shared memory
 * \details Crea un segmento de shared memory, y se conecta (attach). Si el programa se invoca sin argumentos, lee la shared memory y presenta su contenido. Si se desea escribir en la shared memory se debe pasar como argumento por línea de comando la string a escribir encerrada entre comillas dobles
 * 
 */
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/types.h>
#include <sys/ipc.h>
#include <sys/shm.h>

#define SHM_SIZE 20000  //Tamaño del segmento de shared memory a crear

int main(int argc, char *argv[])
{
  key_t key;
  int shmid;
  char *data;
  int mode;

  //Solo funciona sin argumentos o on solo un argumento. No acepta 
  //frases no entrecomilladas
  if (argc > 2) {
     fprintf(stderr, "Uso: shmdemo [datos_a_escribir]\n");
     exit(1);
  }
/*Crea la clave para identificar unívocamente al segmento de shared memory*/
  if ((key = ftok("shmdemo.c", 'R')) == -1) 
  {
     perror("ftok");
     exit(1);
  }
/*Si no existe el segmento de Shared Memory lo crea, y obtiene su identificador. */
  if ((shmid = shmget(key, SHM_SIZE, 0644 | IPC_CREAT)) == -1) 
  {
      perror("shmget");
      exit(1);
   }
/*Se conecta al segmento de shared memory y obtiene un puntero. */
  data = shmat(shmid, (void *)0, 0);
  if (data == (char *)(-1)) 
  {
    perror("shmat");
    exit(1);
  }
/*Cada vez que se ejecute el programa este puntero varía. Es una drección virtual. Una vez creado el segmento de shared memory estará siempre en la misma dirección física de memoria (mismas celdas de RAM) */
printf ("El valor del puntero a la shm es: %p\n", data);

  if (argc == 2) //Si hay argumento los escribe en la shared memory
  {
    printf("Escribiendo en el segmento de memoria: \"%s\"\n", argv[1]);
    strncpy(data, argv[1], SHM_SIZE);
  } 
  else //Si no hay argumentos muestra su contenido en stdout
     printf("El contendio del segmento de memoria es: \"%s\"\n", data);
//Se desconecta del segmento de shared memory
  if (shmdt(data) == -1) 
  {
      perror("shmdt");
      exit(1);
  }

  return 0;
 }
