/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   init_bonus.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: chrhu <chrhu@student.42.fr>                +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/06/01 17:20:15 by chrhu             #+#    #+#             */
/*   Updated: 2024/06/13 18:58:43 by chrhu            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../includes/philo_bonus.h"

// Initialize semaphore
int	init_semaphore(t_data *data)
{
	sem_unlink_all();
	data->forks = sem_open(SEM_FORKS, O_CREAT, 0644, data->nb_philo);
	if (data->forks == SEM_FAILED)
		return (1);
	data->dead_check = sem_open(SEM_DEAD, O_CREAT, 0644, 1);
	if (data->dead_check == SEM_FAILED)
		return (1);
	data->print = sem_open(SEM_PRINT, O_CREAT, 0644, 1);
	if (data->print == SEM_FAILED)
		return (1);
	data->lastmeal_check = sem_open(SEM_LAST_MEAL, O_CREAT, 0644, 1);
	if (data->lastmeal_check == SEM_FAILED)
		return (1);
	data->stop_sem = sem_open(SEM_STOP, O_CREAT, 0644, 1);
	if (data->stop_sem == SEM_FAILED)
		return (1);
	return (0);
}

// Initialize philo
int	init_philo(t_data *data)
{
	int	i;

	i = -1;
	while (++i < data->nb_philo)
	{
		data->philo[i].philo = i;
		data->philo[i].philo_ate = 0;
		data->philo[i].last_meal = 0;
		data->philo[i].data = data;
	}
	return (0);
}

// Initialize structure
int	initialize(t_data *data, char **argv)
{
	data->all_ate = false;
	data->philo_dead = false;
	data->nb_philo = ft_atoi(argv[1]);
	data->time_to_die = ft_atoi(argv[2]);
	data->time_to_eat = ft_atoi(argv[3]);
	data->time_to_sleep = ft_atoi(argv[4]);
	data->nb_meals = -1;
	data->time_to_think = -1;
	if (data->time_to_eat >= data->time_to_sleep)
		data->time_to_think = data->time_to_eat;
	if (argv[5])
		data->nb_meals = ft_atoi(argv[5]);
	if (data->nb_philo > 250 || data->nb_philo < 0 || data->time_to_die < 0
		|| data->time_to_eat < 0 || data->time_to_sleep < 0)
		return (1);
	init_philo(data);
	if (init_semaphore(data))
		return (1);
	return (0);
}
