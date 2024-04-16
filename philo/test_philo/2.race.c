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

void	*routine()
{
	int i = 0;

	while (i < 100000)
	{
		mails++;
		i++;
	}
	return (0);
}

int main (int argc, char **argv)
{
	pthread_t t1;
	pthread_t t2; //variable

	if(pthread_create(&t1, NULL, &routine, NULL) != 0)
		return 1;
	if(pthread_create(&t2, NULL, &routine, NULL) != 0)
		return 2;
	if (pthread_join(t1, NULL))
		return 3;
	if (pthread_join(t2, NULL))
		return 4;
	printf("Number of mails : %d\n", mails);
	return (0);
}
