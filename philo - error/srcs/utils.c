#include "../includes/philo.h"

int check_number(int argc, char **argv)
{
	int i;

	i = 1;
	while (i < argc)
	{
		if (ft_atoi(argv[1]) == 1)
			return (0);
		if (ft_atoi(argv[i]) < 1 || ft_atoi(argv[i]) > 2147483647)
			return (1);
		if (ft_atoi(argv[i]) == 2)
			return (1);
		i++;
	}
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
		return (2);
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
    long long current_time;
	
	current_time = get_time();
	
	
	if (!(data->philo_dead))
	{
		pthread_mutex_lock(&(data->print));
    	printf("%lld %d ", current_time - data->start_time, philo + 1);
		printf("%s\n", status);
		pthread_mutex_unlock(&(data->print));
	}
	if (dead == 1)
	{
		pthread_mutex_lock(&(data->print));
		printf("%lld %d ", current_time - data->start_time, philo + 1);
		printf("%s\n", status);
		pthread_mutex_unlock(&(data->print));
	}
	
	return ;
}

int ft_usleep(t_data *data, long long time)
{
	long long start;

	start = get_time();
	//  Continue the while loop when ./philo 2 800 400 400
	//	while (!(data->philo_dead)  && (current - start) < time)
	while (!(data->philo_dead))
	{
		if ((get_time() - start) > time)
			break;
		usleep(1000);
	}
	return (0);
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
	pthread_mutex_destroy(&data->meal_check);
	pthread_mutex_destroy(&data->dead_check);
	pthread_mutex_destroy(&data->eat_check);
	pthread_mutex_destroy(&data->lastmeal_check);
}