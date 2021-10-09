/**
\details Programa readpipe. Efectuará la apertura de un  pipe. Si este no existe procederá a su creación.
Una vez creado irá a leerlo hasta que algún proceso escriba allí.
Compilar gcc -o readpipe readpipe.c -Wall
Ejecutarlo en otra terminal junto con writepipe y observar resultados. 
\author Alejandro Furfaro
\date 2011.09.01
*/

#include "pipe.h"

int main()
{
	int	fd;			//File descriptor
	char	buffer[MAX_CHARS];	//Buffer para guardar el mensaje
	int	cant;			//Variable para guardar la cantidad de caracteres escrios en el FIFO
	
// Abrimos el FIFO, y si no existe lo creamos: En cualquier caso fd se queda con el File descriptor

	if ((fd = open(FIFO_NAME, O_RDONLY)) == -1)
	  if ((fd = mkfifo (FIFO_NAME, 0666)) < 0)
	    fprintf (stderr, "Error creando la FIFO %s. Código de error %s\n",FIFO_NAME,strerror(errno));
	
//Vamos a leer en el FIFO
	
	if ((cant = read (fd, buffer, MAX_CHARS -1)) < 0)
	  fprintf (stderr, "Error leyendo la FIFO %s. Código de error %s\n",FIFO_NAME,strerror(errno));
	
	buffer[cant]='\0';
	printf ("%s\n", buffer);
	
	close (fd);
	
	return 0;
}