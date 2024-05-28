/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   one_philo.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: chrhu <chrhu@student.42.fr>                +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/04/11 17:20:17 by chrhu             #+#    #+#             */
/*   Updated: 2024/05/28 16:22:42 by chrhu            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../includes/philo.h"

void	print_status_one(t_data *data, int philo, const char *status, int dead) 
{
    long long current_time;
	
	current_time = get_time();
	
	if (!(data->philo_dead))
	{
    	printf("%lld %d ", current_time - data->start_time, philo + 1);
		printf("%s\n", status);
	}
	if (dead == 1)
	{
		printf("%lld %d ", current_time - data->start_time, philo + 1);
		printf("%s\n", status);
	}
}

// check if I keep the using the while loop
int	ft_usleep_one(t_data *data, long long time)
{
	long long start;
	long long current;

	start = get_time();
	current = get_time();

	while (!(data->philo_dead))
	{
		current = get_time();
		if ((current - start) > time)
			break;
		usleep(100);
	}
	return (0);
}

void	philo_eating_one(t_data *data, long long time_last_meal)
{
	print_status_one(data, data->philo[0].philo, "has taken a fork", 0);
	print_status_one(data, data->philo[0].philo, "has taken a fork", 0);
	print_status_one(data, data->philo[0].philo, "is eating", 0);
	time_last_meal = get_time();
	data->philo[0].philo_ate++;
	ft_usleep_one(data, data->time_to_eat);
}

void philo_sleeping_one(t_data *data, long long time_last_meal)
{
	long long start_sleep;
	
	start_sleep = get_time();
	print_status_one(data, data->philo[0].philo, "is sleeping", 0);
    while (get_time() - start_sleep < data->time_to_sleep) 
	{
		usleep(100);
        if (get_time() - time_last_meal >= data->time_to_die) 
		{
            data->philo_dead = true;
		    printf("%lld %d died\n", get_time() - data->start_time, data->philo[0].philo + 1);
    	    break;
        }
    }
}

void handle_one_philo(t_data *data)
{
	long long time_last_meal;
	int i;

	i = 0;
	time_last_meal = get_time();
	while (!data->philo_dead)
	{
		if (i == 0)
		{
			philo_eating_one(data, time_last_meal);
			i = 1;
		}
		if (i == 1)
		{
			philo_sleeping_one(data, time_last_meal);
            if (data->philo_dead)
                break;
			i = 0;
		}
	}
}
