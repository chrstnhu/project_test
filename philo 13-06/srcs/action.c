/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   action.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: chrhu <chrhu@student.42.fr>                +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/04/11 17:20:13 by chrhu             #+#    #+#             */
/*   Updated: 2024/06/13 19:38:18 by chrhu            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../includes/philo.h"

// Check if all philosopher have ate the number of times required
static void	check_philo_ate(t_data *data)
{
	int	i;

	i = 0;
	while (data->nb_meals > 0 && i < data->nb_philo
		&& number_of_meals(data, i) >= data->nb_meals)
		i++;
	if (data->nb_philo == i)
	{
		pthread_mutex_lock(&data->all_ate_check);
		data->all_ate = true;
		pthread_mutex_unlock(&data->all_ate_check);
	}
}

// Check if philo dead
long long	get_elapsed_time(int i, t_data *data, t_philo *philo)
{
	long long	elapsed_time;
	long long	current_time;

	current_time = get_time();
	pthread_mutex_lock(&data->lastmeal_check);
	elapsed_time = current_time - philo[i].last_meal;
	pthread_mutex_unlock(&data->lastmeal_check);
	return (elapsed_time);
}

// Check if philo dead
void	check_dead(t_data *data, t_philo *philo)
{
	int			i;

	while (!is_philo_dead(data) && !philo_all_ate(data))
	{
		i = -1;
		usleep(100);
		while (++i < data->nb_philo)
		{
			if (get_elapsed_time(i, data, philo) > data->time_to_die)
			{
				pthread_mutex_lock(&data->dead_check);
				data->philo_dead = true;
				pthread_mutex_unlock(&data->dead_check);
				print_status(data, philo[i].philo, "died", 1);
			}
			if (data->philo_dead)
				break ;
		}
		check_philo_ate(data);
	}
}

// Action of philosopher : eat, sleep, think
void	*action(void *philosopher)
{
	t_philo	*philo;
	t_data	*data;

	philo = (t_philo *)philosopher;
	data = philo->data;
	if (philo->philo % 2 == 0)
		usleep(500 * data->time_to_eat);
	while (!is_philo_dead(data) && !philo_all_ate(data))
	{
		philo_eating(philo, data);
		if (philo_all_ate(data) || data->nb_philo == 1 || is_philo_dead(data))
			break ;
		print_status(data, philo->philo, "is sleeping", 0);
		ft_usleep(data, data->time_to_sleep);
		if (philo_all_ate(data) || is_philo_dead(data))
			break ;
		print_status(data, philo->philo, "is thinking", 0);
		if (data->nb_philo % 2 == 1 && data->time_to_think != -1)
			usleep(1000 * data->time_to_eat);
	}
	return (0);
}

// Initialize thread
int	init_thread(t_data *data)
{
	t_philo	*philo;
	int		i;

	i = 0;
	philo = data->philo;
	data->start_time = get_time();
	while (i < data->nb_philo)
	{
		if ((pthread_create(&(philo[i].threads), NULL, action, &philo[i])) != 0)
			return (1);
		pthread_mutex_lock(&data->lastmeal_check);
		philo[i].last_meal = get_time();
		pthread_mutex_unlock(&data->lastmeal_check);
		i++;
	}
	check_dead(data, data->philo);
	thread_wait_destroy(data, philo);
	i = -1;
	while (data->nb_meals > 0 && ++i < data->nb_philo)
	{
		printf("Philosopher : %d, ", philo[i].philo + 1);
		printf("number of eat : %d\n", philo[i].philo_ate);
	}
	return (0);
}
