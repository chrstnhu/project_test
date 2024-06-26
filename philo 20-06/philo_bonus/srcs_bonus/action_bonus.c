/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   action_bonus.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: chrhu <chrhu@student.42.fr>                +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/06/01 17:20:13 by chrhu             #+#    #+#             */
/*   Updated: 2024/06/21 17:26:13 by chrhu            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../includes/philo_bonus.h"

// Check if philo dead with the lastmeal
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
		elapsed_time =  get_time() - philo->last_meal;
		sem_post(data->lastmeal_check);
		sem_wait(data->death_declared);
		if (elapsed_time > data->time_to_die)
		{
			sem_wait(data->dead_check);
			if (!data->philo_dead)
			{
				data->philo_dead = true;
				sem_post(data->dead_check);
				sem_wait(data->death_print);
				if (!data->death_printed)
				{
					data->death_printed = 1;
					sem_post(data->death_print);
					//print_status(data, philo->philo, "died", 1);
					return (NULL);
				}
				else
					sem_post(data->death_print);
				return (NULL);
			}
			sem_post(data->dead_check);
		}
		sem_post(data->death_declared);
		usleep(1000);
	}
	return (NULL);
}

// Action of the philo (eat, sleep, think)
// void	*action(t_philo *philo)
// {
// 	t_data	*data;

// 	data = philo->data;
// 	philo->last_meal = get_time();
// 	if (pthread_create(&philo->threads, NULL, check_dead, philo) != 0)
// 		exit(EXIT_FAILURE);
// 	if (philo->philo % 2)
// 		usleep(500 * data->time_to_eat);
// 	while (!is_philo_dead(data) && !is_printed(data))
// 	{
// 		philo_eating(philo, data);
// 		if (is_philo_dead(data) || data->nb_philo == 1 || is_printed(data))
// 			break ;
// 		print_status(data, philo->philo, "is sleeping", 0);
// 		ft_usleep(data, data->time_to_sleep);
// 		if (is_philo_dead(data) || is_printed(data))
// 			break ;
// 		print_status(data, philo->philo, "is thinking", 0);
// 		if (data->nb_philo % 2 == 1 && data->time_to_think != -1)
// 			usleep(1000 * data->time_to_eat);
// 	}
// 	pthread_join(philo->threads, NULL);
// 	if (is_philo_dead(data))
// 		exit_dead(data);
// 	exit(EXIT_SUCCESS);
// }

int	is_time_exceed(t_data *data, t_philo *philo, long long current_time, int status)
{
	long long	current_time_sleep;
	long long	current_time_think;

	sem_wait(data->lastmeal_check);
	current_time_sleep = current_time - philo->last_meal
		+ data->time_to_sleep;
	sem_post(data->lastmeal_check);
	if (status == 1 && current_time_sleep > data->time_to_die
		+ (0.1 * data->nb_philo))
		return (1);
	
	sem_wait(data->lastmeal_check);
	//if (data->nb_philo % 2 == 1)
		current_time_think = current_time - philo->last_meal
			+ data->time_to_think;
	//else
	//	current_time_think = current_time - philo->last_meal
	//	+ data->time_to_think;
	sem_post(data->lastmeal_check);

	if (status == 2 && data->time_to_think != -1
		&& current_time_think > data->time_to_die
			+ (0.2 * data->nb_philo))
		return (1);
	return (0);
}

void	*action(t_philo *philo)
{
	t_data	*data;
	long long current_time;

	data = philo->data;
	philo->last_meal = get_time();
	if (pthread_create(&philo->threads, NULL, check_dead, philo) != 0)
		exit(EXIT_FAILURE);
	if (philo->philo % 2)
		usleep(500 * data->time_to_eat);
	while (!is_philo_dead(data) && !is_printed(data))
	{
		philo_eating(philo, data);
		current_time = get_time();
		if (is_time_exceed(data, philo, current_time, 1) || is_philo_dead(data) || data->nb_philo == 1 
			|| is_printed(data))
			break ;
		print_status(data, philo->philo, "is sleeping", 0);
		ft_usleep(data, data->time_to_sleep);
		current_time = get_time();
		if (is_philo_dead(data) || is_printed(data)
			|| (is_time_exceed(data, philo, current_time,  2)))
			break ;
		print_status(data, philo->philo, "is thinking", 0);
		if (data->nb_philo % 2 == 1 && data->time_to_think != -1)
			usleep(1000 * data->time_to_eat);
	}
	pthread_join(philo->threads, NULL);
	if (is_philo_dead(data))
		exit_dead(data, philo);
	exit(EXIT_SUCCESS);
}

// Wait child process and clean semaphore and memory
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
	sem_post(data->sem_stop);
	sem_close_all(data);
	free_memory(data);
}

// Initialize child process, one per philo
int	init_process(t_data *data)
{
	t_philo	*philo;
	int		i;

	i = -1;
	philo = data->philo;
	data->start_time = get_time();
	sem_wait(data->sem_stop);
	while (++i < data->nb_philo)
	{
		philo[i].pid = fork();
		if (philo[i].pid == -1)
			return (1);
		if (philo[i].pid == 0)
			action(&(philo[i]));
		usleep(200);
	}
	wait_and_close(data);
	return (0);
}
