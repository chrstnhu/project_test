#include "../includes/philo.h"

// check if I keep the using the while loop
int ft_usleep_one(t_data *data, long long time)
{
	long long start;
	long long current;

	start = get_time();
	current = get_time();

	while (!(data->philo_dead))
	{
		current = get_time();
		if ((current - start) > time)
			break;
		usleep(100);
	}
	return (0);
}

void	philo_eating_one(t_data *data, long long time_last_meal)
{
	print_status(data, data->philo[0].philo, "has taken a fork", 0);
	print_status(data, data->philo[0].philo, "has taken a fork", 0);
	print_status(data, data->philo[0].philo, "is eating", 0);
	time_last_meal = get_time();
	data->philo[0].philo_ate++;
	ft_usleep_one(data, data->time_to_eat);
}

void philo_sleeping_one(t_data *data, long long time_last_meal)
{
	long long start_sleep = get_time();

	print_status(data, data->philo[0].philo, "is sleeping", 0);
    while (get_time() - start_sleep < data->time_to_sleep) 
	{
        if (get_time() - time_last_meal >= data->time_to_die) 
		{
            data->philo_dead = true;
		    printf("%lld %d died\n", get_time() - data->start_time, data->philo[0].philo + 1);
    	    break;
        }
        usleep(100);
    }
}

void handle_one_philo(t_data *data)
{
	long long current_time = get_time();
	long long time_last_meal;
	int i;

	i = 0;
	time_last_meal = get_time();
	if (pthread_mutex_init(&data->print, NULL))
		return ;
	while (!data->philo_dead)
	{
		current_time = get_time();
		if (i == 0)
		{
			philo_eating_one(data, time_last_meal);
			i = 1;
		}
		if (i == 1)
		{
			philo_sleeping_one(data, time_last_meal);
            if (data->philo_dead)
                break;
			i = 0;
		}
	}
	pthread_mutex_destroy(&data->print);
}
