/**
\details Programa writepipe. Efectuará la apertura de un  pipe. Si este no existe procederá a su creación.
Una vez creado o abierto, solicitará al usuario el ingreso de una línea de texto por stdin, y una vez recibida la escribirá en el pipe.
Compilar gcc -o writepipe writepipe.c -Wall
Ejecutarlo y observar resultados. 
Luego ejecutar en el directorio de trabajo "cat info1" desde otra terminal.
¿Que pasó?. ¿Porque?.
\author Alejandro Furfaro
\date 2011.09.01
*/

#include "pipe.h"

int main()
{
	int	fd;	//File descriptor
	char	buffer[MAX_CHARS];	//Buffer para guardar el mensaje
	int	cant;	//Variable para guardar la cantidad de caracteres escritos en el FIFO
	
// Abrimos el FIFO, y si no existe lo creamos: En cualquier caso fd se queda con el File descriptor

	if ((fd = open(FIFO_NAME, O_WRONLY)) == -1)
	  if ((fd = mkfifo (FIFO_NAME, 0666)) < 0)
	    fprintf (stderr, "Error creando la FIFO %s. Código de error %s\n",FIFO_NAME,strerror(errno));

// Vamos a esperar una línea desde el teclado
	printf ("Por favor ingrese un mensaje: \n");

	if (!fgets(buffer, MAX_CHARS, stdin))
	    fprintf (stderr, "Error leyendo standard input. Código de error %s\n", strerror(errno));
	
//Vamos a escribir en el FIFO
	
	cant = write (fd, buffer, strlen (buffer));
	
	close (fd);
	
	return 0;
}