#include <stdio.h>
#include <unistd.h>

int main (){

	int pid;
	int cant;

	pid = fork();
	
	if(pid > 0){
		printf("Soy el proceso padre,ahora muero\n");
		exit(0);
	}
	if(pid < 0){
		perror("fork");
		exit(1);
	}
	
	/* Proceso hijo, futuro demonio */
	setsid();
	cant = 10;
	/*No hace nada solo pierde tiempo */
	while(cant){
		
		sleep(30);
		cant --;
	}
	exit(0);
	
}
