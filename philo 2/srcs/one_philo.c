#include "../includes/philo.h"

// check if I keep the using the while loop
int ft_usleep_one(t_data *data, long long time) 
{
    struct timeval start;
    get_current_time(&start);

    while (!(data->philo_dead)) {
        struct timeval current;
        get_current_time(&current);
        long long elapsed_time = get_time_ms(current, start);

        if (elapsed_time > time)
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
    time_last_meal = data->elapsed_time;
	data->philo[0].philo_ate++;
	ft_usleep_one(data, data->time_to_eat);
}
void philo_sleeping_one(t_data *data, long long time_last_meal) 
{
    struct timeval start_sleep;
    get_current_time(&start_sleep);

    print_status(data, data->philo[0].philo, "is sleeping", 0);
    while (get_time_ms(data->current, start_sleep) < data->time_to_sleep) 
	{
		struct timeval last_meal_tv = ll_to_timeval(time_last_meal);
        if (get_time_ms(data->current, last_meal_tv) >= data->time_to_die) 
		{
            data->philo_dead = true;
            printf("%ld %d died\n", get_time_ms(data->current, data->start), data->philo[0].philo + 1);
            break;
        }
        usleep(100);
    }
}

void handle_one_philo(t_data *data) 
{
    long long time_last_meal = data->elapsed_time;
    int i = 0;

    if (pthread_mutex_init(&(data->print), NULL))
        return;
    while (!data->philo_dead) {
        if (i == 0) {
            philo_eating_one(data, time_last_meal);
            i = 1;
        }
        if (i == 1) {
            philo_sleeping_one(data, time_last_meal);
            if (data->philo_dead)
                break;
            i = 0;
        }
    }
    pthread_mutex_destroy(&(data->print));
}
