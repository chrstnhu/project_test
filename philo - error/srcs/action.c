#include "../includes/philo.h"

void	philo_eating(t_philo *philo)
{	
	t_data *data;

	data = philo->data;

	pthread_mutex_lock(&data->forks[philo->left_fork]);
	print_status(data, philo->philo, "has taken a fork", 0);
	pthread_mutex_lock(&data->forks[philo->right_fork]);
	print_status(data, philo->philo, "has taken a fork", 0);
	print_status(data, philo->philo, "is eating", 0);

	pthread_mutex_lock(&data->meal_check);
	pthread_mutex_lock(&data->lastmeal_check);
	philo->last_meal = get_time();
	pthread_mutex_unlock(&data->lastmeal_check);
	
	
	philo->philo_ate++;
	ft_usleep(data, data->time_to_eat);
	pthread_mutex_unlock(&data->meal_check);

	pthread_mutex_unlock(&data->forks[philo->left_fork]);
	pthread_mutex_unlock(&data->forks[philo->right_fork]);
}

// void	philo_sleeping(t_data *data, t_philo *philo)
// {
	// print_status(data, philo->philo, "is sleeping", 0);
	// ft_usleep(data, data->time_to_sleep);

// }

void *action(void *philosopher)
{
    t_philo *philo;
    t_data *data;

    philo = (t_philo *)philosopher;
    data = philo->data;

	if (philo->philo % 2)
		usleep(500 * data->time_to_eat);

    while (!(data->philo_dead))
    {
        philo_eating(philo);
        if (data->all_ate)
            break ;
        // philo_sleeping(data, philo);
		print_status(data, philo->philo, "is sleeping", 0);
		ft_usleep(data, data->time_to_sleep);
		if (data->time_to_think <= 0)
		{
			print_status(data, philo->philo, "is thinking", 0);
			ft_usleep(data, data->time_to_think);
		}
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
            pthread_mutex_lock(&data->dead_check);
            if ((get_time() - philo[i].last_meal) > data->time_to_die)
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
		if (data->nb_philo == i)
		{
			pthread_mutex_lock(&data->eat_check);
            data->all_ate = true;
			pthread_mutex_unlock(&data->eat_check);
		}
	}
}

int init_thread(t_data *data)
{
	t_philo *philo;
    int i;

	i = 0;
	philo = data->philo;
	while (i < data->nb_philo)
	{
		if ((pthread_create(&(philo[i].threads), NULL, action, &philo[i])) != 0)
			return (1);
		philo[i].last_meal = get_time();
		i++;
	}
	check_dead(data, data->philo);
	thread_wait_destroy(data, philo);
    return (0);
}

