#include "../includes/philo.h"

int check_number(int argc, char **argv)
{
	int i;

	i = 1;
	while (i < argc)
	{
		if (ft_atoi(argv[i]) > 2147483647 || ft_atoi(argv[i]) < 1)
			return (1);
		i++;
	}
	i = 1;
	while (i < argc)
	{
		if (ft_atoi(argv[i]) < 1)
			return (1);
		i++;
	}
	if (ft_atoi(argv[1]) > 0)
		return (0);
	return (0);
}

int	ft_atoi(const char *nptr)
{
	int	i;
	int	result;

	i = 0;
	result = 0;
	while ((nptr[i] > 8 && nptr[i] < 14) || nptr[i] == ' ')
		i++;
	if (nptr[i] == '-')
		return (-1);
	if (nptr[i] == '+')
		i++;
	while (nptr[i] >= '0' && nptr[i] <= '9')
	{
		result = result * 10 + (nptr[i] - '0');
		i++;
	}
	return (result);
}

void print_status(t_data *data, int philo, const char *status, int dead) 
{
    struct timeval current_time;
    gettimeofday(&current_time, NULL);
    long long current_time_ms = get_ts_in_ms(current_time, data->start_time);

    pthread_mutex_lock(&(data->print));
    if (!(data->philo_dead))
    {
        printf("%lld %d ", current_time_ms, philo + 1);
        printf("%s\n", status);
    }
    else if (dead == 1)
    {
        printf("%lld %d ", current_time_ms, philo + 1);
        printf("%s\n", status);
    }
    pthread_mutex_unlock(&(data->print));
    return;
}

int ft_usleep(t_data *data, long long time)
{
    struct timeval start_time, current_time;
    gettimeofday(&start_time, NULL);
    long long start_time_ms = get_ts_in_ms(start_time, data->start_time);

    gettimeofday(&current_time, NULL);
    long long current_time_ms = get_ts_in_ms(current_time, data->start_time);

    while (!(data->philo_dead))
    {
        gettimeofday(&current_time, NULL);
        current_time_ms = get_ts_in_ms(current_time, data->start_time);
        
        if ((current_time_ms - start_time_ms) > time)
            break;
        
        usleep(100);
    }
    return 0;
}

void thread_wait_destroy(t_data *data, t_philo *philo)
{
    int i;

	i = data->nb_philo;
    while (--i >= 0)
    {
        if (pthread_join(philo[i].threads, NULL) != 0)
            return ;
    }
	i = data->nb_philo;
    while (--i >= 0)
        pthread_mutex_destroy(&data->forks[i]);
    pthread_mutex_destroy(&data->print);
	pthread_mutex_destroy(&data->dead_check);
	pthread_mutex_destroy(&data->meal_check);
	//add
	pthread_mutex_destroy(&data->eat_check);
	pthread_mutex_destroy(&data->lastmeal_check);
}
