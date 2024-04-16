#include <stdlib.h>
#include <stdio.h>
#include <unistd.h>
#include <pthread.h>


/*
Process id 750640
Process id 750640

1 Process can have multiple thread 
2 Threads can access all variables (common adress)
----------------------
Value of x : 3
Value of x : 3


*/

int x = 2;

void	*routine()
{
	// printf("Process id %d\n", getpid());
	x++;
	sleep(2);
	// printf("Ending threads\n");
	printf("Value of x : %d\n", x);

	return (0);
}

void	*routine2()
{
	// printf("Process id %d\n", getpid());
	sleep(2);
	// printf("Ending threads\n");
	printf("Value of x : %d\n", x);

	return (0);
}

int main (int argc, char **argv)
{
	pthread_t t1;
	pthread_t t2; //variable

	if(pthread_create(&t1, NULL, &routine, NULL) != 0)
		return 1;
	if(pthread_create(&t2, NULL, &routine2, NULL) != 0)
		return 2;
	if (pthread_join(t1, NULL))
		return 3;
	if (pthread_join(t2, NULL))
		return 4;
	return (0);
}

/*
not the sam
Process id 750640
Process id 750000
--------------------
1. Duplicate all the variable in process

-----------------
Value of x: 2
Value of x: 3

int main  (int argc, char **argv)
{
	int x = 2;
	int pid = fork();
	if (pid == -1)
		return 1;

	// printf("Process id %d\n", getpid());

	if (pid == 0)
	{
		x++;
	}
	sleep(2);
	printf("Process id %d\n", getpid());
	printf("Value of x : %d\n", x);
	if (pid != 0)
		wait(NULL);
	return 0;
}
*/