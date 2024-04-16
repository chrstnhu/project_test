# include <sys/time.h>
# include <unistd.h>
# include <stdio.h>
# include <stdlib.h>
# include <string.h>
# include <pthread.h>


// struct timeval 
// {
//    time_t      tv_sec;     // seconds
//    suseconds_t tv_usec;    // microseconds
// }
// ;

long long gettime(void)
{
	struct timeval tv;

	gettimeofday(&tv, NULL);
	return((((long long)tv.tv_sec) * 1000) + (((long long)tv.tv_usec) / 1000));
}

int main()
{
	printf("milliseconde : %lli\n", gettime());
	
	return (0);
}