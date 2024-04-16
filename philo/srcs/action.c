#include "../includes/philo.h"

void	philo_eating(t_data *data)
{
	int i = 0;
	
	while (!data->died)
	{
		if (i % 2 == 0)
		{
			pthread_mutex_lock(&data->meal_check);
			data->philo_eat++;
			pthread_mutex_unlock(&data->meal_check);
		}
		i++;
	}
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



void	check_dead(t_data *data, t_philo *philo)
{
	int i;

	while (!(data->all_ate))
	{
		i = 0;
		while (!(data->philo_died) && i < data->nb_philo)
		{
			pthread_mutex_lock(&(data->meal_check));
			if ((philo[i].last_meal - get_time()) > data->time_to_die)
			{
				action_print(data, i, "died");
				data->philo_died = 1;
			}
			pthread_mutex_unlock(&(data->meal_check));
			// usleep(100);
			i++;
		}
		if (data->philo_died == 1)
			break ;
		i = 0;
		while (data->nb_eat != -1 && i < data->nb_philo && philo[i].philo_ate >= data->nb_eat)
			i++;
		if (data->nb_philo == i)
			data->all_ate = 1;
	}
}


void *init_thread(t_data *data)
{
    int i;
	t_philo *philo;

	i = 0;
	philo = data->philo;
	data->current_time = get_time();

    while (i < data->nb_philo)
    {
        if ((pthread_create(&(philo[i].nb_threads), NULL, action, &philo[i])) != 0)
            return NULL;
		philo->last_meal = get_time();
        printf("Thread eating %d has started\n", i);
		i++;
	}
	check_dead(data, data->philo);
    return NULL;
}

void *action(t_data *data)
{
    pthread_t threads[NB_MAX];
    int i;

    init_thread(data);
    for (i = 0; i < data->philo; i++)
    {
        if (pthread_join(threads[i], NULL) != 0)
            return (void *)5;
        printf("Thread %d has finished execution\n", i);
    }
    for (i = 0; i < data->philo; i++)
    {
        pthread_mutex_destroy(&data->forks[i]);
    }
    pthread_mutex_destroy(&data->meal_check);
    return NULL;
}