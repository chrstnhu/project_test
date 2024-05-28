/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   actions_utils.c                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: chrhu <chrhu@student.42.fr>                +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/05/28 17:23:40 by chrhu             #+#    #+#             */
/*   Updated: 2024/05/28 17:31:33 by chrhu            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../includes/philo.h"

void philo_eating(t_philo *philo)
{	
    t_data *data;

    data = philo->data;

	int first_fork = (philo->left_fork < philo->right_fork) ? philo->left_fork : philo->right_fork;
    int second_fork = (philo->left_fork < philo->right_fork) ? philo->right_fork : philo->left_fork;
    pthread_mutex_lock(&data->forks[first_fork]);
	print_status(data, philo->philo, "has taken a fork", 0);
    pthread_mutex_lock(&data->forks[second_fork]);
    print_status(data, philo->philo, "has taken a fork", 0);
    print_status(data, philo->philo, "is eating", 0);
    pthread_mutex_lock(&data->lastmeal_check);
    philo->last_meal = get_time();
    pthread_mutex_unlock(&data->lastmeal_check);
    pthread_mutex_lock(&data->meal_check);
    philo->philo_ate++;
    pthread_mutex_unlock(&data->meal_check);
    ft_usleep(data, data->time_to_eat);
    pthread_mutex_unlock(&data->forks[first_fork]);
    pthread_mutex_unlock(&data->forks[second_fork]);
}

void print_status(t_data *data, int philo, const char *status, int dead) 
{
    long long current_time;
	
	current_time = get_time();
	
	pthread_mutex_lock(&(data->print));
	if (!philo_no_dead(data))
	{
    	printf("%lld %d ", current_time - data->start_time, philo + 1);
		printf("%s\n", status);
	}
	if (dead == 1)
	{
		printf("%lld %d ", current_time - data->start_time, philo + 1);
		printf("%s\n", status);
	}
	pthread_mutex_unlock(&(data->print));

	return ;
}

int ft_usleep(t_data *data, long long time)
{
	long long start;

	start = get_time();
	//  Continue the while loop when ./philo 2 800 400 400
	//	while (!(data->philo_dead)  && (current - start) < time)
	while (!philo_no_dead(data))
	{
		if ((get_time() - start) > time)
			break;
		usleep(1000);
	}
	return (0);
}

//Convert seconds and microseconds to millisecondes
long long get_time(void) 
{
    struct timeval tv;

    if (gettimeofday(&tv, NULL) < 0)
		printf("%sError function : gettimeofday()%s\n", RED, DEFAULT);
    return (((long long)tv.tv_sec) * 1000) + ((long long)tv.tv_usec / 1000);
}