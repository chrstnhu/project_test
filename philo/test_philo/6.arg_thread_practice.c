#include <stdlib.h>
#include <stdio.h>
#include <pthread.h>
#include <time.h>
#include <unistd.h>

/*
sum 5 number from an array
*/

#include <stdio.h>
#include <stdlib.h>
#include <pthread.h>
#include <unistd.h>

int primes[10] = { 2, 3, 5, 7, 11, 13, 17, 19, 23, 29 };

void* routine(void* arg) 
{
    int index = *(int*)arg;
    int sum = 0;
	int j = 0;

	while(j < 5) 
	{
        sum += primes[index + j];
	j++;
	}
    printf("Local sum: %d\n", sum);
    *(int*)arg = sum;
    return arg;
}

int main(int argc, char **argv) 
{
    pthread_t th[2];
    int i;
    int *a;
	
	i = 0;
	while (i < 2) 
	{
        a = malloc(sizeof(int));
        *a = i * 5;
        if (pthread_create(&th[i], NULL, &routine, a) != 0)
            perror("Failed to create thread");
		i++;
    }
    int globalSum = 0;
	i = 0;
    while (i < 2) 
	{
        int* r;
        if (pthread_join(th[i], (void**) &r) != 0)
            perror("Failed to join thread");
        globalSum += *r;
        free(r);
		i++;
    }
    printf("Global sum: %d\n", globalSum);
    return 0;
}