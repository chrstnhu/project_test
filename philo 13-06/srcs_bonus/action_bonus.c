/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   action_bonus.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: chrhu <chrhu@student.42.fr>                +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/06/01 17:20:13 by chrhu             #+#    #+#             */
/*   Updated: 2024/06/13 18:55:42 by chrhu            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../includes/philo_bonus.h"

long long	get_elapsed_time(t_data *data, t_philo *philo)
{
	long long	elapsed_time;
	long long	current_time;

	current_time = get_time();
	sem_wait(data->lastmeal_check);
	elapsed_time = get_time() - philo->last_meal;
	sem_post(data->lastmeal_check);
	return (elapsed_time);
}

void	*check_dead(void *philosopher)
{
	t_philo	*philo;
	t_data	*data;

	philo = (t_philo *)philosopher;
	data = philo->data;
	while (!is_philo_dead(data))
	{
		if (get_elapsed_time(data, philo) > data->time_to_die)
		{
			sem_wait(data->dead_check);
			data->philo_dead = true;
			sem_post(data->dead_check);
			print_status(data, philo->philo, "died", 1);
			sem_wait(data->stop_sem);
			cleanup_semaphores(data);
			exit(EXIT_FAILURE);
		}	
		usleep(1000);
	}
	return (NULL);
}

void	*action(t_philo *philo)
{
	t_data	*data;

	data = philo->data;
	philo->last_meal = get_time();
	pthread_create(&philo->threads, NULL, check_dead, philo);
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
	}
	pthread_join(philo->threads, NULL);
	if (is_philo_dead(data))
	{
		cleanup_semaphores(data);
		exit(EXIT_FAILURE);
	}
	exit(EXIT_SUCCESS);
}

void	wait_and_close(t_data *data)
{
	int	i;
	int	status;

	i = 0;
	while (i < data->nb_philo)
	{
		waitpid(-1, &status, 0);
		if (status != 0)
		{
			i = -1;
			while (++i < data->nb_philo)
				kill(data->philo[i].pid, SIGKILL);
			break ;
		}
		i++;
	}
	sem_post(data->stop_sem);
	cleanup_semaphores(data);
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
			action(&(philo[i]));
		usleep(100);
	}
	sem_post(data->stop_sem);
	wait_and_close(data);
	return (0);
}
