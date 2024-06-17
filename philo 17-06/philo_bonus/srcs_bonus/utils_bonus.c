/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   utils_bonus.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: chrhu <chrhu@student.42.fr>                +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/06/01 17:20:21 by chrhu             #+#    #+#             */
/*   Updated: 2024/06/17 20:11:53 by chrhu            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../includes/philo_bonus.h"

// Free memory
void	free_memory(t_data *data)
{
	if (data->philo)
	{
		free(data->philo);
		data->philo = NULL;
	}
}

// Wait child process and clean semaphore er memory
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
	sem_close_all(data);
	free_memory(data);
}

// Close semaphore and clean memory
void	sem_close_all(t_data *data)
{
	int	error;

	error = 0;
	if (sem_close(data->forks) == -1)
		error = 1;
	if (sem_close(data->dead_check) == -1)
		error = 1;
	if (sem_close(data->print) == -1)
		error = 1;
	if (sem_close(data->lastmeal_check) == -1)
		error = 1;
	if (sem_close(data->stop_sem) == -1)
		error = 1;
	if (sem_close(data->death_declared) == -1)
		error = 1;
	if (error)
		exit(EXIT_FAILURE);
}

// Check overflow and negative number
int	check_number(int argc, char **argv)
{
	int	i;

	i = 1;
	while (i < argc)
	{
		if (ft_atoi(argv[i]) > 2147483647 || ft_atoi(argv[i]) < 1)
			return (1);
		i++;
	}
	if (ft_atoi(argv[1]) > 0)
		return (0);
	return (0);
}

// Convert string into number
int	ft_atoi(const char *nptr)
{
	int	i;
	int	result;

	i = 0;
	result = 0;
	while ((nptr[i] > 8 && nptr[i] < 14) || nptr[i] == ' ')
		i++;
	if (nptr[i] == '-')
		return (-1);
	if (nptr[i] == '+')
		i++;
	while (nptr[i] >= '0' && nptr[i] <= '9')
	{
		result = result * 10 + (nptr[i] - '0');
		i++;
	}
	return (result);
}
