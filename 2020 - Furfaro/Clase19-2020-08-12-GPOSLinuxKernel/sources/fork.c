#include <sys/types.h>
#include <unistd.h>
#include <stdio.h>
#include <stdlib.h>

int varg=123;

void relacion(void);

int main(int argc, char *argv[])
{
	int varm=234;
	pid_t pid;
	pid=fork();
	if(pid<0)
        	exit(1);
    	if(!(pid=fork()))
	{
        	//printf("PID=%d del hijo y PID=%d del padre\n", getpid(), getppid());
        	printf("%d es hijo de %d\n",getpid(),getppid());
        	relacion();
        	varg++;varm++;
        	printf("(%d) Variable local:%p -> %d\n",getpid(),&varm,varm);
        	printf("(%d) Variable global:%p -> %d\n",getpid(),&varg,varg);
	}
	else
	{
		//printf("PID=%d del padre y PID=%d del abuelo\n", getpid(), getppid());
		printf("%d es hijo de %d\n",getpid(),getppid());
		relacion();
		varg--;varm--;
		printf("%d Variable local:%p -> %d\n",getpid(),&varm,varm);
        	printf("%d Variable global:%p -> %d\n",getpid(),&varg,varg);
	}
	return 0;
}
void relacion(void)
{
        int varf=345;
	printf("%d Variable función:%p -> %d\n",getpid(),&varf,varf);
}
