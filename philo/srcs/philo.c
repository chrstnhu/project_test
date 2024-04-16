#include "../includes/philo.h"

//Convert seconds and microseconds to millisecondes
long long get_time(void) 
{
    struct timeval tv;

    if (gettimeofday(&tv, NULL) < 0)
		printf("%sError function : gettimeofday()%s\n", RED, DEFAULT);
    return (((long long)tv.tv_sec) * 1000) + ((long long)tv.tv_usec / 1000);
}

int main(int argc, char *argv[]) 
{
	t_data data;

    if (argc != 5) 
	{
        printf("%sPlease enter 5 arguments :", RED);
		printf("./philo nb_of_philo time_to_die time_to_eat time_to_sleep%s\n", DEFAULT);
        return (1);
    }
	if (initialize(&data, argv) == 1)
		printf("%sError init philo%s", RED, DEFAULT);
	init_thread(&data);

    // printf("Starting simulation for %d philosophers...\n", data.nb_philo);

    // Your philosopher simulation logic here...

    // Example of displaying the timestamp along with the message
    // printf("%lld 0 1 has taken a fork\n", get_time() / 1000);
    
    return 0;
}