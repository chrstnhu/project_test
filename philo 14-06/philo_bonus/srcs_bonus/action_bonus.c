/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   action_bonus.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: chrhu <chrhu@student.42.fr>                +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/06/01 17:20:13 by chrhu             #+#    #+#             */
/*   Updated: 2024/06/14 20:42:25 by chrhu            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../includes/philo_bonus.h"

void	exit_dead(t_data *data, t_philo *philo)
{
	sem_wait(data->dead_check);
	data->philo_dead = true;
	sem_post(data->dead_check);
	print_status(data, philo->philo, "died", 1);
	sem_post(data->stop_sem);
	close_semaphores(data);
	free_memory(data);
	exit(EXIT_FAILURE);
}

void	*check_dead(void *philosopher)
{
	t_philo		*philo;
	t_data		*data;
	long long	elapsed_time;

	philo = (t_philo *)philosopher;
	data = philo->data;
	while (!is_philo_dead(data))
	{
		sem_wait(data->lastmeal_check);
		elapsed_time = get_time() - philo->last_meal;
		sem_post(data->lastmeal_check);
		if (elapsed_time > data->time_to_die)
		{
			sem_wait(data->death_declared);
			if (!data->philo_dead)
				exit_dead(data, philo);
			sem_post(data->death_declared);
			break ;
		}
		usleep(1000);
	}
	return (NULL);
}

void	action_plus(t_data *data)
{
	if (is_philo_dead(data))
	{
		close_semaphores(data);
		free_memory(data);
		exit(EXIT_FAILURE);
	}
	free_memory(data);
}

void	*action(t_philo *philo)
{
	t_data	*data;

	data = philo->data;
	philo->last_meal = get_time();
	pthread_create(&philo->threads, NULL, check_dead, philo);
	if (philo->philo % 2)
		usleep(500 * data->time_to_eat);
	while (!is_philo_dead(data))
	{
		philo_eating(philo, data);
		if (is_philo_dead(data) || data->nb_philo == 1)
			break ;
		print_status(data, philo->philo, "is sleeping", 0);
		ft_usleep(data, data->time_to_sleep);
		if (is_philo_dead(data))
			break ;
		print_status(data, philo->philo, "is thinking", 0);
		if (data->nb_philo % 2 == 1 && data->time_to_think != -1)
			usleep(1000 * data->time_to_eat);
		if (is_philo_dead(data))
			break ;
	}
	pthread_join(philo->threads, NULL);
	exit(EXIT_SUCCESS);
}

int	init_process(t_data *data)
{
	t_philo	*philo;
	int		i;

	i = -1;
	philo = data->philo;
	data->start_time = get_time();
	sem_wait(data->stop_sem);
	while (++i < data->nb_philo)
	{
		philo[i].pid = fork();
		if (philo[i].pid == -1)
			return (1);
		if (philo[i].pid == 0)
		{
			action(&(philo[i]));
			exit(EXIT_SUCCESS);
		}
		usleep(100);
	}
	sem_post(data->stop_sem);
	wait_and_close(data);
	return (0);
}
