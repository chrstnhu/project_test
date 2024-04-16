#include <stdlib.h>
#include <stdio.h>
#include <pthread.h>
#include <time.h>
#include <unistd.h>

/*
we want to print 1 of the array the primes, just one time
*/

int primes[10] = {2, 3, 5, 7, 11, 13, 17, 19, 23, 29};

void* routine(void* arg) 
{
    int index = *(int*)arg;
    printf("%d ", primes[index]);
    free(arg);
	return (0);
}
/* MULTITHREAD ISSUS -> 0
int main(int argc, char **argv) 
{
	pthread_t th[10];
	int i = 0;
	int j = 0;
	while (i < 10)
	{
		if (pthread_create(&th[i], NULL, &routine, &i) != 0)
			perror("FAiled to create thread");
		i++;
	}
	j = 0;
	while (j < 10)
	{
		if (pthread_join(th[j], NULL) != 0)
			perror("FAiled to join thread");
		j++;
	}
}
*/

/*
int main(int argc, char **argv) 
{
	pthread_t th[10];
	int i = 0;
	while (i < 10)
	{
		if (pthread_create(&th[i], NULL, &routine, &i) != 0)
			perror("FAiled to create thread");
		i++;
	}
	i = 0;
	while (i < 10)
	{
		if (pthread_join(th[i], NULL) != 0)
			perror("FAiled to join thread");
		i++;
	}
}
*/

int main(int argc, char **argv) 
{
	int *a;
	pthread_t th[10];
	int i = 0;
	while (i < 10)
	{
		a = malloc(sizeof(int));
		*a = i;
		if (pthread_create(&th[i], NULL, &routine, a) != 0)
			perror("Failed to create thread");
		i++;
	}
	i = 0;
	while (i < 10)
	{
		if (pthread_join(th[i], NULL) != 0)
			perror("FAiled to join thread");
		i++;
	}
}