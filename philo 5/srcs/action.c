#include "../includes/philo.h"

void philo_eating(t_data *data, t_philo *philo)
{
    struct timeval start_time;
    gettimeofday(&start_time, NULL);

    printf("[%lld] Philosopher %d starts eating.\n", start_time, philo->philo);

    pthread_mutex_lock(&data->forks[philo->left_fork]);
    print_status(data, philo->philo, "has taken a fork", 0);
    pthread_mutex_lock(&data->forks[philo->right_fork]);
    print_status(data, philo->philo, "has taken a fork", 0);
    pthread_mutex_lock(&data->meal_check);
    print_status(data, philo->philo, "is eating", 0);
    philo->last_meal = get_ts_in_ms(start_time, data->start_time); // Met à jour le temps du dernier repas
    pthread_mutex_unlock(&data->lastmeal_check);
    pthread_mutex_unlock(&data->meal_check);
    philo->philo_ate++;
    ft_usleep(data, data->time_to_eat);
    pthread_mutex_unlock(&data->forks[philo->left_fork]);
    pthread_mutex_unlock(&data->forks[philo->right_fork]);
}


void	philo_sleeping(t_data *data, t_philo *philo)
{
	print_status(data, philo->philo, "is sleeping", 0);
	ft_usleep(data, data->time_to_sleep);
	// if (data->time_to_think != 0)
	// {
		print_status(data, philo->philo, "is thinking", 0);
	// 	ft_usleep(data, data->time_to_think);
	// }
}

void *action(void *philosopher)
{
    t_philo *philo;
    t_data *data;

    philo = (t_philo *)philosopher;
    data = philo->data;

    printf("[%lld] Philosopher %d starts thinking.\n", get_ts_in_ms(struct timeval current_time, data->start_time), philo->philo);

    if (philo->philo % 2)
        ft_usleep(data, 10);

    while (!(data->philo_dead))
    {
        philo_eating(data, philo);
        if (data->all_ate)
            break ;

        philo_sleeping(data, philo);
    }
    return (NULL);
}



void check_dead(t_data *data, t_philo *philo)
{
    int i;

    while (!(data->all_ate))
    {
        i = -1;
        while (!(data->philo_dead) && ++i < data->nb_philo)
        {
            usleep(100);
            struct timeval current_time;
            gettimeofday(&current_time, NULL);
            long long current_time_ms = get_ts_in_ms(current_time, data->start_time);

            pthread_mutex_lock(&data->dead_check);
            if ((current_time_ms - philo[i].last_meal) > data->time_to_die)
            {
                data->philo_dead = true;
                print_status(data, philo[i].philo, "died", 1);
                pthread_mutex_unlock(&data->dead_check);
                break;
            }
            pthread_mutex_unlock(&data->dead_check);
        }
        if (data->philo_dead)
            break;
        i = 0;
        while (data->nb_eat != -1 && i < data->nb_philo && philo[i].philo_ate >= data->nb_eat)
            i++;
        pthread_mutex_lock(&data->eat_check);
        if (data->nb_philo == i)
            data->all_ate = true;
        pthread_mutex_unlock(&data->eat_check);
    }
}


int init_thread(t_data *data)
{
    t_philo *philo;
    int i;
	struct timeval start_time;

    i = 0;
    philo = data->philo;
    while (i < data->nb_philo)
    {
        if ((pthread_create(&(philo[i].threads), NULL, action, &philo[i])) != 0)
            return (1);
        gettimeofday(&start_time, NULL);
        philo[i].last_meal = get_ts_in_ms(start_time, data->start_time);
        i++;
    }
    check_dead(data, data->philo);
    thread_wait_destroy(data, philo);
    return (0);
}