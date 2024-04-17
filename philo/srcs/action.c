#include "../includes/philo.h"

void	philo_eating(t_data *data, t_philo *philo)
{	
	data = philo->data;

	pthread_mutex_lock(&data->forks[philo->left_fork]);
	pthread_mutex_lock(&data->forks[philo->right_fork]);

	print_status(data, philo->philo, "has taken a fork");

	pthread_mutex_lock(&data->meal_check);

	print_status(data, philo->philo, "is eating");

	philo->last_meal = get_time();
	
	pthread_mutex_unlock(&data->meal_check);
	
	usleep(data->time_to_eat * 1000);
	philo->philo_ate++;
	
	
	pthread_mutex_unlock(&data->forks[philo->right_fork]);
	pthread_mutex_unlock(&data->forks[philo->left_fork]);
}

void	philo_sleeping(t_data *data, t_philo *philo)
{
	long long start_time;
	long long current_time;

	start_time = get_time();
	// pthread_mutex_lock(&data->sleep_check);
	
	while (data->philo_dead == false)
	{
		current_time = get_time();
		if ((current_time - start_time) >= data->time_to_sleep)
			break ;
		print_status(data, philo->philo, "is sleeping");
		usleep(data->time_to_sleep * 1000);
	}
	
	// pthread_mutex_unlock(&data->sleep_check);

}

void	philo_thinking(t_data *data, t_philo *philo)
{
	long long start_time;
	long long current_time;

	start_time = get_time();
	// pthread_mutex_lock(&data->think_check);
	
	while (data->philo_dead == false)
	{
		current_time = get_time();
		if ((current_time - start_time) >= data->time_to_think)
			break ;
		print_status(data, philo->philo, "is thinking");
		usleep(data->time_to_think * 1000);
	}
	
	// pthread_mutex_unlock(&data->think_check);
}

void *action(void *philosopher)
{
    t_philo *philo;
    t_data *data;

    philo = (t_philo *)philosopher;
    data = philo->data;

	if (philo->philo % 2)
		usleep(10000);

    while (!(data->philo_dead))
    {
        philo_eating(data, philo);
        if (data->all_ate)
            break ;
        philo_sleeping(data, philo);
        if (data->time_to_think != 0)
            philo_thinking(data, philo);
    }
    return (NULL);
}


/*
void	philo_thinking(t_philo *data)
{
	t_philo *data;
	int i = 0;
	
	while (!data->died)
	{
		if (i % 2 == 0)
		{
			pthread_mutex_lock(&data->meal_check);
			data->philo_think++;
			pthread_mutex_unlock(&data->meal_check);
		}
		i++;
	}
	return (0);
}

void	philo_sleeping(t_philo *data)
{
	t_philo *data;
	int i = 0;
	
	while (!data->died)
	{
		if (i % 2 == 0)
		{
			pthread_mutex_lock(&data->meal_check);
			data->philo_sleep++;
			pthread_mutex_unlock(&data->meal_check);
		}
		i++;
	}
	return (0);
}
*/


void thread_wait_destroy(t_data *data, t_philo *philo)
{
    int i;

	i = 0;
    while (i < data->nb_philo)
    {
        if (pthread_join(philo[i].threads, NULL) != 0)
            return ;
        // printf("\nThread %d has finished execution\n", i);
		i++;
    }
	i = 0;
	while (i < data->nb_philo)
    {
        pthread_mutex_destroy(&data->forks[i]);
		i++;
    }
    pthread_mutex_destroy(&data->print);
	pthread_mutex_destroy(&data->dead_check);
	pthread_mutex_destroy(&data->meal_check);
	// pthread_mutex_destroy(&data->sleep_check);
	// pthread_mutex_destroy(&data->think_check);
    
}

void check_dead(t_data *data, t_philo *philo)
{
    int i;

    while (!(data->all_ate))
    {
        i = 0;
        while (!(data->philo_dead) && i < data->nb_philo)
        {
            pthread_mutex_lock(&data->dead_check);
            if ((get_time() - philo[i].last_meal) > data->time_to_die)
            {
                data->philo_dead = true;
                print_status(data, philo[i].philo, "died\n");
				pthread_mutex_unlock(&data->dead_check);
				if (data->philo_dead == true)
					printf("true, one philo dead\n");
                break;
				printf("After break\n");
            }
            pthread_mutex_unlock(&data->dead_check);
            usleep(100);
            i++;
        }
        if (data->philo_dead)
            break;
        i = 0;
        while (data->nb_eat != -1 && i < data->nb_philo && philo[i].philo_ate >= data->nb_eat)
            i++;
        if (data->nb_philo == i)
            data->all_ate = true;
    }
}


int init_thread(t_data *data)
{
	t_philo *philo;
    int i;

	i = 0;
	philo = data->philo;
	data->start_time = get_time();
    while (i < data->nb_philo)
    {
        if ((pthread_create(&(philo[i].threads), NULL, action, &philo[i])) != 0)
            return (1);
		philo[i].last_meal = get_time();
        // printf("Thread eating %d has started\n", i);
		i++;
	}
	check_dead(data, data->philo);
	thread_wait_destroy(data, philo);
    return (0);
}

