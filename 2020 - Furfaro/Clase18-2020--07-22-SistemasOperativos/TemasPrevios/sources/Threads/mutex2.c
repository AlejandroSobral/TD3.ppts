/*
 * Programa de ejemplo de threads.
 *
 * Dos hilos acceden simultáneamente a un buffer de 1000 enteros, uno para leer
 * y otro para escribir.
 * Si al compilar esta definido MUTEX, los hilos accederán sincronizadamente
 * al buffer. Si no está definido, lo harán de cualquier manera.
 */
#include <pthread.h>
#include <stdlib.h>
#include <stdio.h>

/* Prototipo de la función que va a ejecutar el thread hijo */
void *funcionThread (void *parametro);

/* Buffer de datos al que accederán ambos hilos a la vez */
#define TAMANO_BUFFER 1000
int buffer[TAMANO_BUFFER];

#ifdef MUTEX
/* mutex para sincronizar el acceso a buffer */
pthread_mutex_t mutexBuffer;
#endif


/*
 * Principal
 * Lanza un thread para la función funcionThread.
 * Después de comprobar el posible error, se mete en un bucle infinito
 * haciendo que todos los elementos de buffer vayan tomando valores iguales,
 * pero incrementando en uno en cada pasada.
 */
int main ()
{
	/* identificador del thread hijo */
	pthread_t idHilo;

	/* error devuelto por la función de creación del thread */
	int error;

	/* Valor que se meterá en buffer */
	int contador = 0;

	/* Para el bucle para recorrer los ítems de buffer */
	int i;

#ifdef MUTEX
	/* Se inicializa el mutex */
	pthread_mutex_init (&mutexBuffer, NULL);
#endif

	/* Creamos el thread */
	error = pthread_create (&idHilo, NULL, funcionThread, NULL);

	/* Comprobamos el error al arrancar el thread */
	if (error != 0)
	{
		perror ("No puedo crear thread");
		exit (-1);
	}

	/* Bucle infinito */
	while (1)
	{

#ifdef MUTEX
		/* Se espera y se bloquea el mutex */
		pthread_mutex_lock (&mutexBuffer);
#endif

		/* Se cambian todos los valores de buffer */
		for (i=0; i<TAMANO_BUFFER; i++)
			buffer[i] = contador;

#ifdef MUTEX
		/* Se desbloquea el mutex */
		pthread_mutex_unlock (&mutexBuffer);
#endif

		/* Se incrementa el valor que se meterá en buffer en la siguiente
		 * iteración */
		contador++;
	}
	exit(0);
}





/*
 * Función que se ejecuta en el thread.
 * Comprueba que todos los ítems de buffer sean iguales y marca error si
 * hay alguno distinto.
 */
void *funcionThread (void *parametro)
{
	int i;		/* Para bucle */
	int elementoDistinto = 0;	/* Para marcar si hay algún elemento distinto */

	while (1)
	{

#ifdef MUTEX
		/* Esperamos y bloqueamos el mutex */
		pthread_mutex_lock (&mutexBuffer);
#endif

		/* Bucle para comparar si todos los elementos del buffer son iguales.
		 * Si hay alguno distinto, se marca en elementoDistinto y se sale. */
		for (i=1; i<TAMANO_BUFFER; i++)
		{
			if (buffer[0] != buffer[i])
			{
				elementoDistinto = 1;
				break;
			}
		}

		/* Se escribe si se ha encontrado algún elemento distinto */
		if (elementoDistinto)
			printf ("Hijo  : Error. Elementos de buffer distintos\n");
		else
			printf ("Hijo  : Correcto\n");

		/* Se deja la variable de elementoDistinto a 0 */
		elementoDistinto = 0;

#ifdef MUTEX
		/* Desbloqueemos el mutex */
		pthread_mutex_unlock (&mutexBuffer);
#endif
	}
}
