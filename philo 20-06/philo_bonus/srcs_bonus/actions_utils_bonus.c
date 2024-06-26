/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   actions_utils_bonus.c                              :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: chrhu <chrhu@student.42.fr>                +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/06/01 17:23:40 by chrhu             #+#    #+#             */
/*   Updated: 2024/06/21 16:27:50 by chrhu            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../includes/philo_bonus.h"

// Philo take two forks and eat
void	philo_eating(t_philo *philo, t_data *data)
{
	if (data->nb_philo == 1)
	{
		sem_wait(data->print);
		printf("%lld 1 ", data->start_time - data->start_time);
		printf("has taken a fork\n");
		sem_post(data->print);
		return ;
	}
	sem_wait(data->forks);
	sem_wait(data->forks);
	print_status(data, philo->philo, "has taken a fork", 0);
	print_status(data, philo->philo, "has taken a fork", 0);
	print_status(data, philo->philo, "is eating", 0);
	sem_wait(data->lastmeal_check);
	philo->last_meal = get_time();
	sem_post(data->lastmeal_check);
	ft_usleep(data, data->time_to_eat);
	sem_post(data->forks);
	sem_post(data->forks);
}

// Return if philo live or dead
int	is_philo_dead(t_data *data)
{
	int	philo_status;

	sem_wait(data->dead_check);
	philo_status = data->philo_dead;
	sem_post(data->dead_check);
	return (philo_status);
}

int	is_printed(t_data *data)
{
	int	print_status;

	sem_wait(data->death_print);
	print_status = data->death_printed;
	sem_post(data->death_print);
	return (print_status);
}

// Print the status of the philo
void	print_status(t_data *data, int philo, const char *status, int dead)
{
	long long	current_time;

	if (dead == 0)
		sem_wait(data->print);
	current_time = get_time();
	if (dead == 1 && is_philo_dead(data))
	{
		printf("%lld %d %s\n", current_time - data->start_time,
			philo + 1, status);
	}
	else if (dead == 0 && !is_philo_dead(data) && !is_printed(data))
	{
		printf("%lld %d %s\n", current_time - data->start_time,
			philo + 1, status);
	}
	if (dead == 0)
		sem_post(data->print);
}


// Usleep time (eat, sleep)
void	ft_usleep(t_data *data, long long time)
{
	long long	start;

	start = get_time();
	while (!is_philo_dead(data) && !is_printed(data))
	{
		if (get_time() - start > time)
			break ;
		usleep(1000);
	}
}

// Convert seconds and microseconds to millisecondes
long long	get_time(void)
{
	struct timeval	tv;

	if (gettimeofday(&tv, NULL) < 0)
		printf("%sError function : gettimeofday()%s\n", RED, DEFAULT);
	return ((tv.tv_sec * 1000) + (tv.tv_usec / 1000));
}
