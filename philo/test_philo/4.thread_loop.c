#include <stdlib.h>
#include <stdio.h>
#include <unistd.h>
#include <pthread.h>

/*


*/

int received = 0;
int mails = 0;
int lock = 0;
pthread_mutex_t	mutex;

void	*routine()
{
	int i = 0;
	
	while (i < 100000)
	{
		if (i % 2 == 0)
		{
			pthread_mutex_lock(&mutex);
			mails++;
			pthread_mutex_unlock(&mutex);
		}
		else
		{
			pthread_mutex_lock(&mutex);
			received++;
			pthread_mutex_unlock(&mutex);
		}
		i++;
	}
	return (0);
}

int main (int argc, char **argv)
{
	//pthread array
	pthread_t th[4];
	int i = 0;

	//Init mutex
	pthread_mutex_init(&mutex, NULL);

	// while loop, not at the same time
/*
	while (i < 4)
	{
						// or &th[i]
		if (pthread_create(th + i, NULL, &routine, NULL) != 0)
		{
			perror("Failed to create thread");
			return 1;
		}
		printf("Thread %d has started\n", i);
		if (pthread_join(th[i], NULL) != 0)
			return 5;
		printf("Thread %d has finished execution\n", i);
		i++;
	}
*/

	// execute at the same time
	while (i < 4)
	{
						// or &th[i]
		if (pthread_create(th + i, NULL, &routine, NULL) != 0)
		{
			perror("Failed to create thread");
			return 1;
		}
		printf("Thread %d has started\n", i);
		i++;
	}
	i = 0;
	while (i < 4)
	{
		if (pthread_join(th[i], NULL) != 0)
			return 5;
		printf("Thread %d has finished execution\n", i);
		i++;
	}
	
	//Destroy
	pthread_mutex_destroy(&mutex);
	printf("\nTotal = %d\n", i*100000);
	printf("Number of mails : %d\n", mails);
	printf("Number received : %d\n", received);
	return (0);
}
