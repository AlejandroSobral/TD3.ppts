/**
 * \file ej-fork.c
 * \brief Ejemplo simple de uso de fork
 * \details 
 * Este ejemplo sencillo muestra el uso y funcionamiento de la primitiva fork (). Se crea desde el proceso principal una instancia child (hijo), la cual al igual que el padre ejecuta una demora de 180 segundo para desde otra terminal poderlos visualizar con htop o con ps -elf | grep ej-fork, y estudiar sus PIDs, PPIDs, y su estado (observar columna S en htop).
 * Una vez ejecutado comentar la línea anteúltima del programa que pone a dormir al padre de modo que éste proceso termine de inmediato. Repetir la observación en otra terminal con htop o ps y observar el estado del proceso hijo.
 * Finalmente descomentar la línea del proceso padre que contiene la función wait. Repetir la observación y ver el estado del padre mientras el hijo no termina. ¿Es wait () una función bloqueante?
 */
#include <stdio.h>
#include <stdlib.h>
#include <sys/types.h>
#include <sys/wait.h>
#include <unistd.h>
#include <string.h>

// cont = 0;

int main (void) 
{
  int pid, cont = 0; 
  pid = fork();                                // Creo un proceso hijo
  // Hijo
  if (!pid) 
  {
    printf ("[Hijo]: PID: %d. Mi variable pid vale: %d\n",getpid(),pid);
    cont += 20;
    sleep (5);
    printf ("[Hijo]: Mi variable cont vale: %d\n", cont);
    printf ("[Hijo]: Ha sido un placer. Adios!!\n");
    exit (0);
  }
 
  // Padre
  printf ("[Padre]: PID: %d. PID hijo: %d\n",getpid(), pid);
  printf ("[Padre]: Esperando que termine mi hijo cuyo PID es: %d\n", pid);
  wait (NULL);
  printf ("[Padre]: Termino el proceso hijo %d.  Excelente. Viste cuanto vale su variable cont?\n", pid);
  printf ("[Padre]: Dentro de 10 segundos te muestro cuanto vale mi variable cont!\n");
  sleep (10);
  printf ("[Padre]: Mi variable cont vale: %d\n", ++cont);
  return 0;
}

