#include <stdio.h>
#include <stdlib.h>
#include <errno.h>
#include <string.h>
#include <fcntl.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <unistd.h>
#include <sys/select.h>
#include <sys/time.h>

#define RX_FIFO_NAME "/tmp/canal1"
#define TX_FIFO_NAME "/tmp/canal2"
#define MAX_CHARS 128

extern	int	errno;
void abrir (int *fdrx, int *fdtx);
void chat (int *fdrx, int *fdtx);