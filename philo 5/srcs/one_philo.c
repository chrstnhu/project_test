#include "../includes/philo.h"

// check if I keep the using the while loop
int ft_usleep_one(t_data *data, long long time)
{
    struct timeval start_time, current_time;
    long long start_ms, current_ms;

    gettimeofday(&start_time, NULL);
    start_ms = get_ts_in_ms(start_time, start_time);
    
    gettimeofday(&current_time, NULL);
    current_ms = get_ts_in_ms(current_time, start_time);

    while (!(data->philo_dead))
    {
        gettimeofday(&current_time, NULL);
        current_ms = get_ts_in_ms(current_time, start_time);
        
        if ((current_ms - start_ms) > time)
            break;

        usleep(100);
    }
    return 0;
}

void philo_eating_one(t_data *data, long long time_last_meal)
{
    print_status(data, data->philo[0].philo, "has taken a fork", 0);
    print_status(data, data->philo[0].philo, "has taken a fork", 0);
    print_status(data, data->philo[0].philo, "is eating", 0);
    struct timeval current_time;
    gettimeofday(&current_time, NULL);
    time_last_meal = get_ts_in_ms(current_time, data->start_time);
    data->philo[0].philo_ate++;
    ft_usleep_one(data, data->time_to_eat);
}


void philo_sleeping_one(t_data *data, long long time_last_meal)
{
    struct timeval start_sleep;
    gettimeofday(&start_sleep, NULL);
    long long start_sleep_ms = get_ts_in_ms(start_sleep, start_sleep);

    print_status(data, data->philo[0].philo, "is sleeping", 0);
    while (true) 
    {
        struct timeval current_time;
        gettimeofday(&current_time, NULL);
        long long current_time_ms = get_ts_in_ms(current_time, start_sleep);
        
        if (current_time_ms - start_sleep_ms >= data->time_to_sleep) 
        {
            break;
        }
        
        if (current_time_ms - time_last_meal >= data->time_to_die) 
        {
            data->philo_dead = true;
            // Convertir data->start_time en millisecondes également
            long long start_time_ms = get_ts_in_ms(data->start_time, data->start_time);
            printf("%lld %d died\n", current_time_ms - start_time_ms, data->philo[0].philo + 1);
            break;
        }
        usleep(100);
    }
}

void handle_one_philo(t_data *data)
{
    if (pthread_mutex_init(&data->print, NULL) != 0)
        return;

    struct timeval start_time;
    gettimeofday(&start_time, NULL);
    long long time_last_meal = get_ts_in_ms(start_time, start_time);

    while (!data->philo_dead)
    {
        if (data->philo[0].philo)
            philo_eating_one(data, time_last_meal); // Ne pas passer l'adresse de time_last_meal
        else
            philo_sleeping_one(data, time_last_meal);
    }
    pthread_mutex_destroy(&data->print);
}
