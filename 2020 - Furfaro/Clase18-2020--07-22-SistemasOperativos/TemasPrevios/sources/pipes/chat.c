/**
\details Programa chat. 
Efectuará la apertura de un  pipe. Si este no existe procederá a su creación.
Una vez creado o abierto, solicitará al usuario el ingreso de una línea de texto por stdin.
A partir de este momento esperará por dos eventos en forma simultánea:
1. Una string recibida desde stdin como resultado de lo que escriba el usuario. Ni bien la reciba la escribirá en el pipe para transmitirla a un usuario que ejecutará este mismo proceso pero en otra terminal.
2. Una string escrita en el pipe por dicho usuario desde otra terminal. Esa string debe ser presentada en pantalla de la forma:
[remoto]: string_recibida
Compilar gcc -o chat chat.c -Wall
El programa realizará esta atividad hasta que el usuario presione CTRL+C.
EL PROBLEMA: ¿Como hacemos para esperar dos eventos en forma simultánea?
Pista: man select
\author Alejandro Furfaro
\date 2011.09.01
*/

#include "chat.h"

int main()
{
  /**Se definen los File Descriptors para FIFOs de RX y TX*/
  int	fdrx,fdtx;
  
  /**Abrimos / creamos los FIFO's de Tx y Rx.*/
  abrir (&fdrx,&fdtx);
  
  /**A chatear!...*/
  chat (&fdrx,&fdtx);
  
  return 0;
}


/**
**************************************************************
\fn void abrir (int *fdrx, int *fdtx)
\details Función para obtener los file descriptors para los pipes de Transmisión y Recepción.
Intenta la apertura del named pipe /tmp/canal1 y /tmp/canal2.
Si no existen open devuelve -1 y se setea la variable errno (definida en errno.h) en 2 (ENOENT).
Detecta esa condición y si la falla de open setea errno en ENOENT, entonces mediante mkfifo crea los dos named pipes.
\arg Recibe por referencia dos enteros para almacenar los file descriptors respectivos.
\author Alejandro Furfaro
\date 2011.05.08
*/

void abrir (int *fdrx, int *fdtx)
{
   
/**Abrimos el FIFO de Transmisión, y si no existe lo creamos: En cualquier caso fd se queda con el File descriptor.*/
  if ((*fdtx = open(TX_FIFO_NAME, O_WRONLY)) < 0)
    if (errno == ENOENT)
      if ((*fdtx = mkfifo (TX_FIFO_NAME, 0666)) < 0)
      {
	fprintf (stderr, "Error creando la FIFO %s. Código de error %s\n",TX_FIFO_NAME,strerror(errno));
	exit (1);
      }
  printf ("Abrí archivo para transmisión %s, file descriptor= %d.\n",TX_FIFO_NAME, *fdtx);
  
/**Abrimos el FIFO de Recepción, y si no existe lo creamos: En cualquier caso fd se queda con el File descriptor.*/
  if ((*fdrx = open(RX_FIFO_NAME, O_RDONLY)) < 0)
    if (errno == ENOENT)
      if ((*fdrx = mkfifo (RX_FIFO_NAME, 0666)) < 0)
      {
	fprintf (stderr, "Error creando la FIFO %s. Código de error %s\n",RX_FIFO_NAME,strerror(errno));
	exit (1);
      }
  printf ("Abrí archivo para recepción %s, file descriptor= %d.\n",RX_FIFO_NAME, *fdrx);
}

/**
**************************************************************
\fn void chat (int *fdrx, int *fdtx)
\details Función que implementa un loop infinito en el que se espera en forma simultánea el ingreso de datos por el teclado o el ingreso de datos por el pipe de recepción.
En el primer caso los datos recibidos se envían por el pipe de transmisión.
En el segundo caso los datos se presentan en pantalla.
\arg Recibe por referencia los dos enteros que almacenan los file descriptors de los named pipes creados con la función abrir.
\author Alejandro Furfaro
\date 2011.05.08
*/

void chat (int *fdrx, int *fdtx)
{
  char	rx_buffer[MAX_CHARS];	/**Buffer para recibir */
  char	tx_buffer[MAX_CHARS];	/**Buffer para transmitir*/

  int	cant;	/**Variable para guardar la cantidad de caracteres escrios en el FIFO.*/
  
  /**Definimos array de descriptores de lectura a esperar.*/
  fd_set rfds;
  /**Lo Inicializamos (zero)*/
  FD_ZERO(&rfds);

  printf ("Bienvenido a chat. Ya hay alguien escuchando desde la otra punta de la conversación. Por favor ingrese sus mensajes en el índice.\n\n\n");
  
  /**Loop Infinito....*/
  while (1)
  {
    FD_SET (0, &rfds);
    /**Incluimos stdin (0) en el array de file descriptors de lectura*/
    FD_SET (*fdrx, &rfds);
    /**Incluimos el FIFO de RX (*fdrx) en el array de file descriptors de lectura*/ 
    printf ("[Vos]:");/**Pesenta el prompt local.*/
    fflush (stdout);/**Vaciamos el contenido de stdout.*/
    
    /**Aqui es donde hay que ir a esperar el teclado y al pipe simultáneamente:*/
    if (select (*fdrx+1, &rfds, NULL, NULL, NULL) < 0)
    {
      if (errno == EINTR)
      {
	fprintf (stderr,"Signal received!!!\n");
// 	continue;
      }
    }
    else  if (FD_ISSET (0, &rfds)) /**Si se ingresó string por teclado.*/
    {
      /**Leemos la string*/
      if (!fgets(tx_buffer, MAX_CHARS, stdin))
	fprintf (stderr, "Error leyendo standard input. Código de error: %s.\n",strerror(errno));
      /**Vamos a escribir en el FIFO de transmisión.*/
      cant = write (*fdtx, tx_buffer, strlen (tx_buffer));
    }
    else if (FD_ISSET (*fdrx, &rfds)) /**Si llegó algo por el pipe de recepción...*/
    {
      if ((cant = read(*fdrx, rx_buffer, MAX_CHARS)) < 0)
	fprintf (stderr, "Error leyendo FIFO de recepción. Código de error: %s\n", strerror(errno));
      rx_buffer[cant]='\0';
      printf("[remoto]: %s\n",rx_buffer);
    }
  }
}