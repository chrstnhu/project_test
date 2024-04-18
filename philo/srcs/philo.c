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

	if (argc == 5 || argc == 6)
	{
		if (initialize(&data, argv) == 1)
			printf("%sError init philo%s", RED, DEFAULT);
		init_thread(&data);
	}
	else
	{
        printf("%sPlease enter 5/6arguments :", RED);
		printf("./philo nb_of_philo time_to_die");
		printf("time_to_eat time_to_sleep number_of_eat%s\n", DEFAULT);
        return (1);
    }
    return 0;
}