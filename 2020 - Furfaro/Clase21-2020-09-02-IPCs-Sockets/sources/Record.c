/**
***************************************************************
\details 
* "Departamento de Electrónica, FRBA, UTN"
* Programa ejemplo de manejo de audio en la PC sobre Linux (obvio).
* Objetivos: 
* 1.Aprender a programar el dispositivo de audio de la PC sobre Linux.
* 2.Acceder a su configuración. 
* Adicional: ver /usr/src/linux/include/linux/soundcard.h 
\author Alejandro Furfaro
\date 2011.08.08
\version 1.0.0
***************************************************************
*/

#include <unistd.h>
#include <fcntl.h>
#include <sys/types.h>
#include <sys/ioctl.h>
#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <linux/soundcard.h>

/* segundos de audio a grabar */
#define MSEG 10
/* sampling rate = velocidad de muestreo del audio a la entrada*/
#define RATE 48000
/* sample size = Tamaño de muestra. Típicamente 8 o 16 bits */
#define SIZE 16
/* 1 = mono 2 = stereo */
#define CHANNELS 2
/*Calcula dinámicamente el tamaño del buffer de audio*/
#define buffsize MSEG*RATE*SIZE*CHANNELS/8/1000

/**
*******************************************************************************
\fn void set_audio_params (int fd)
\details Se encarga de establecer los valores de velocidad de muestreo (Sampling Rate) Sample Size (tamaño de la muestra), y número de canales, en la placa de audio del sistema.
Utilza la syscall ioctl (), (input output control), para acceder al dispositivo mediante el formato establecido por la interfaz de sonido de LNUX, y el formato genérico de ioctl (file_descriptor, nombre del parámetro, puntero al valor a escribir);
\author Alejandro Furfaro
\date 2011.05.08
*/

void set_audio_params (int fd)
{
	int	arg;		/* argumento para ioctl */
	int	status;		/* salida de ioctl */

/* seteamos los parametros de muestreo  */
	arg = SIZE;	   /* arg = Tamaño de muestra */
	status = ioctl(fd, SOUND_PCM_WRITE_BITS, &arg); 
/* SOUND_PCM_WRITE_BITS es la macro que escribe en la placa de sonido (a través del driver por supuesto), el valor del tamaño de muestra en bits que seteamos en la variable arg, cuyo puntero se provee como tercer argumento a ioctl().
La interfaz genérica de audio manejada por streams traduce comandos generales a los comandos particulares para el hardware de modo transparente al programador */
	if (status == -1) 
		perror("Error con comando SOUND_PCM_WRITE_BITS");
/* El hecho que ioctl devuelva error no quiere decir que no se haya configurado el parámetro que se quiso programar.*/
	if (arg != SIZE)
/* Por lo general la variable pasada por referencia vuelve con el valor programado si el original no estaba entre los posibles valores del parámetro que se quiso configurar. Por ejemplo, Tamaño de muestra: si arg hubiese ido en un valor por ejemplo 13, o 15, seguramente no coicidiría con los valores posibles para configurar el conversor A/D de la placa de sonido. 
En tal caso el driver programa al valor mas cercano al argumento recibido,modifica la variable recibda como referencia al valor, para que el programa que lo envió pueda saber el valor con el que se configuró, y hace que la función devuelva -1, para que se pueda analizar el error y tomar nota del valor configurado finalmente. */
		fprintf (stderr,"Tamaño de muestras no soportado. Se programó %d\n",arg);

	arg = CHANNELS;  /* mono o stereo */
	status = ioctl(fd, SOUND_PCM_WRITE_CHANNELS, &arg);
	if (status == -1)
		perror("Error en comando SOUND_PCM_WRITE_CHANNELS");
	if (arg != CHANNELS)
		fprintf (stderr,"Cantidad de canales no soportado. Se programó %d\n",arg);

	arg = RATE;	   /* Velocidad de Muestreo */
	status = ioctl(fd, SOUND_PCM_WRITE_RATE, &arg);
	if (status == -1)
		perror("Error en comando SOUND_PCM_WRITE_RATE");
	if (arg != RATE)
		fprintf (stderr,"Velocidad de muestreo no soportada. Se programó %d\n",arg);

	return;
}

int main(int argc, char * argv[])
{
	int dspfd;	/* sound device file descriptor */
	int rawfd;	/* Archivo raw file descriptor*/
	int status;	/* return status de system calls */
	/* bufer para el audio */
 	unsigned char *buf = malloc(buffsize);
	
	/*Controlamos los argumentos*/
	if (argc !=2)
	{
	  printf ("Cantidad insuficiente de argumentos\nUso: %s [filename]\n",argv[0]);
	  exit (1);
	}
	/*Creamos el archivo de audio. Si no existe lo creamos. Y si existe lo truncamos*/
	if ((rawfd = open(argv[1], O_RDWR | O_CREAT | O_TRUNC ,0660)) < 0 )
	{
	  	fprintf(stderr,"Error en función open, Código de error: %s\n",strerror (rawfd)); 
		exit(1);
	}
	/* abrimos el dispositivo de audio*/
	if ((dspfd = open("/dev/dsp", O_RDWR))<0)
	{ 
		fprintf(stderr,"Error en función open, Código de error: %s\n",strerror (dspfd)); 
		exit(1);
	}
	/*Seteamos parámetros de audio*/
	set_audio_params (dspfd);

	while (1) 
	{

/*La siguiente función bloquea al proceso hasta que se llene el buffer.*/
		status = read(dspfd, buf, buffsize); /* lee */

		if (status != buffsize)
		{
			fprintf(stderr,"Error en función read, Código de error: %s\n",strerror (status));
			exit (1);
		}

/* Write generalmente no bloquea hasta wue termina. Sino que envía la información presente en el buffer al kernel.*/
		status = write(rawfd, buf, buffsize); /* reproduce */
		if (status != buffsize)
		{
			fprintf(stderr,"Error en función write, Código de error: %s\n",strerror (status)); 
		}

	}
	free (buf);
}
