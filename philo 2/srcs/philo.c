#include "../includes/philo.h"


void get_current_time(struct timeval *current_time) {
    gettimeofday(current_time, NULL);
}

void get_start_time(t_data *data) {
    gettimeofday(&(data->start), NULL);
}

/*
//Convert seconds and microseconds to millisecondes
long long get_time(void) 
{
    struct timeval tv;

    if (gettimeofday(&tv, NULL) < 0)
		printf("%sError function : gettimeofday()%s\n", RED, DEFAULT);
    return (((long long)tv.tv_sec) * 1000) + ((long long)tv.tv_usec / 1000);
}*/

long get_time_ms(struct timeval current, struct timeval start)
{
	long long time_sec;
	long long time_usec;

	if (current.tv_usec < start.tv_usec)
		time_sec = current.tv_sec - start.tv_sec - 1;
	else
		time_sec = current.tv_sec - start.tv_sec;
	if (current.tv_usec < start.tv_usec)
		time_usec = current.tv_usec - start.tv_usec + 1000000;
	else
		time_usec = current.tv_usec - start.tv_usec;
	return ((time_sec * 1000) + (time_usec / 1000));
}

int main(int argc, char *argv[]) 
{
	t_data data;

	if (argc == 5 || argc == 6)
	{
		if ((check_number(argc,argv) == 1) || (initialize(&data, argv) == 1))
		{
			printf("%sError init philo%s", RED, DEFAULT);
			return (1);
		}
		// if (data.nb_philo == 1)
		// 	handle_one_philo(&data);
		// else
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