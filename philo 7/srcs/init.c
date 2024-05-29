/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   init.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: chrhu <chrhu@student.42.fr>                +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/04/11 17:20:15 by chrhu             #+#    #+#             */
/*   Updated: 2024/05/29 13:00:25 by chrhu            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../includes/philo.h"

void	thread_wait_destroy(t_data *data, t_philo *philo)
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

int	init_mutex(t_data *data)
{
	int i;
	
	i = data->nb_philo;
	while (--i >= 0)
	{
		if (pthread_mutex_init(&(data->forks[i]), NULL))
			return (1);
	}
	if (pthread_mutex_init(&data->print, NULL))
		return (1);
	if (pthread_mutex_init(&data->meal_check, NULL))
		return (1);
	if (pthread_mutex_init(&data->dead_check, NULL))
		return (1);
	if (pthread_mutex_init(&data->lastmeal_check, NULL))
		return (1);
	if (pthread_mutex_init(&data->eat_check, NULL))
		return (1);
	return (0);
}

int	init_philo(t_data *data)
{
	int i;

	i = 0;
	while (i < data->nb_philo)
	{
		data->philo[i].philo = i;
		data->philo[i].philo_ate = 0;
		data->philo[i].left_fork = i;
		data->philo[i].right_fork = (i + 1) % data->nb_philo;
		data->philo[i].last_meal = 0;
		data->philo[i].data = data;
		i++;
	}
	return (0);
}

int	initialize(t_data *data, char **argv)
{
	data->all_ate = false;
	data->philo_dead = false;
    data->nb_philo = ft_atoi(argv[1]);
    data->time_to_die = ft_atoi(argv[2]);
    data->time_to_eat = ft_atoi(argv[3]);
    data->time_to_sleep = ft_atoi(argv[4]);
	data->time_to_think = -1;
	if (data->time_to_eat >= data->time_to_sleep)
		data->time_to_think = data->time_to_eat - data->time_to_sleep;
	// printf("time to eat: %d\n", data->time_to_eat);
	// printf("time to sleep : %d\n", data->time_to_sleep);
	// printf("time to think : %d\n", data->time_to_think);
	data->start_time = get_time();
	data->nb_eat = -1;
	if (argv[5])
		data->nb_eat = ft_atoi(argv[5]);
	// printf("nb eat : %d\n", data->nb_eat);
	if (data->nb_philo > 250 || data->nb_philo < 0 || data->time_to_die < 0
		|| data->time_to_eat < 0 || data->time_to_sleep < 0)
		return (1);
	init_philo(data);
	if (init_mutex(data))
			return (1);
	return (0);
}
