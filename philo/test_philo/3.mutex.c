#include <stdlib.h>
#include <stdio.h>
#include <unistd.h>
#include <pthread.h>

/*
			|	t1   |   t2   |
read mails  |   29   |   23   |
--------------------------------
increment   |   29   |   30   |
--------------------------------
write mails |   30   |   24   |  (write 1 + read mails) --  issue

calculate t1

read the value inside t2

*/


int mails = 0;
int lock = 0;
pthread_mutex_t	mutex;


/*
pthread_mutex_lock(&mutex);
mails++;
pthread_mutex_unlock(&mutex);

➜  test_philo ./a.out     
Number of mails : 400000
-------------------------------------------
//pthread_mutex_lock(&mutex);
mails++;
//pthread_mutex_unlock(&mutex);

➜  test_philo ./a.out     
Number of mails : 137531

*/
void	*routine()
{
	int i = 0;

	while (i < 100000)
	{
/*
		(the second if finish)
		if (lock == 1)
		{
			//wait until the lock is 0
		}
		lock = 1；
		mails++;
		lock = 0；
*/		
		//Protect other threads executing at the same time between mutex
		pthread_mutex_lock(&mutex);
		mails++;
		pthread_mutex_unlock(&mutex);
		//read mails
		//increment 
		//write mails
		i++;
	}
	return (0);
}

int main (int argc, char **argv)
{
	pthread_t t1;
	pthread_t t2; //variable
	pthread_t t3;
	pthread_t t4;

	//Init mutex
	pthread_mutex_init(&mutex, NULL);

	if(pthread_create(&t1, NULL, &routine, NULL) != 0)
		return 1;
	if(pthread_create(&t2, NULL, &routine, NULL) != 0)
		return 2;
	if(pthread_create(&t3, NULL, &routine, NULL) != 0)
		return 3;
	if(pthread_create(&t4, NULL, &routine, NULL) != 0)
		return 4;
	if (pthread_join(t1, NULL))
		return 5;
	if (pthread_join(t2, NULL))
		return 6;
	if (pthread_join(t3, NULL))
		return 7;
	if (pthread_join(t4, NULL))
		return 8;
	
	//Destroy
	pthread_mutex_destroy(&mutex);
	printf("Number of mails : %d\n", mails);
	return (0);
}
