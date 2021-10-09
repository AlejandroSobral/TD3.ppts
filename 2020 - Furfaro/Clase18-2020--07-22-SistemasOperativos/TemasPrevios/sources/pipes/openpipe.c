/**
\details Programa openpipe. Solo efectuará la apertura de un  pipe. Si este no existe procederá a su creación
\author Alejandro Furfaro
\date 2011.09.01
*/

#include "pipe.h"

int main()
{
	int fd;


	if ((fd = open(FIFO_NAME, O_RDONLY)) == -1)
	   if ((fd=mkfifo (FIFO_NAME, 0666))<0)
	    fprintf (stderr, "Error creando la FIFO %s. Código de error %s\n",FIFO_NAME,strerror(errno));
	

	return 0;
 }
