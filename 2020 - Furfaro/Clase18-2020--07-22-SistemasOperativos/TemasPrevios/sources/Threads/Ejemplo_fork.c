#include<stdio.h>
#include<signal.h>
#include<unistd.h>
#include<string.h>
#include<stdlib.h>
#include<errno.h>

//hacer el handler para que los hijos no queden defunct!!

int main (void){
  int contador = 1000;
    
  while(contador){
    if( fork() == 0){
      printf ("Proceso hijo\n");
      sleep(60);
      exit(1);
    }
    else{
      printf ("Siguo creando hijos\n");
	sleep(30);
    }
    contador--;
  }
}
