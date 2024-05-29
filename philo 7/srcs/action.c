/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   action.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: chrhu <chrhu@student.42.fr>                +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/04/11 17:20:13 by chrhu             #+#    #+#             */
/*   Updated: 2024/05/29 13:06:21 by chrhu            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../includes/philo.h"

static int check_nb_philo(t_philo *philo)
{
	t_data *data;
	
	data = philo->data;
	if (data->nb_philo == 1)
	{
		print_status(data, philo->philo, "has taken a fork", 0);
		return (1);
	}
	
	if (philo->philo % 2)
		usleep(500 * philo->data->time_to_eat);
	
	return (0);
}

void *action(void *philosopher)
{
    t_philo *philo;

    philo = (t_philo *)philosopher;
	if (check_nb_philo(philo) == 1)
		return (NULL);
    while (!philo_no_dead(philo->data))
    {
		
        philo_eating(philo);
		pthread_mutex_lock(&philo->data->eat_check);
        if (philo->data->all_ate)
		{
			pthread_mutex_unlock(&philo->data->eat_check);
            break ;
		}
		pthread_mutex_unlock(&philo->data->eat_check);
		print_status(philo->data, philo->philo, "is sleeping", 0);
		ft_usleep(philo->data, philo->data->time_to_sleep);
		if (philo->data->time_to_think <= 0)
		{
			print_status(philo->data, philo->philo, "is thinking", 0);
			ft_usleep(philo->data, philo->data->time_to_think);
		}
    }
    return (NULL);
}

static void	check_philo_ate(t_data *data)
{
	int i;

	i = 0;
    while (data->nb_eat != -1 && i < data->nb_philo && philo_all_ate(data, i) >= data->nb_eat)
	{
        i++;
	}
	if (data->nb_philo == i)
	{
		pthread_mutex_lock(&data->eat_check);
        data->all_ate = true;
		pthread_mutex_unlock(&data->eat_check);
	}
}
void check_dead(t_data *data, t_philo *philo)
{
    int i;
	long long elapsed_time;

    while (!(data->all_ate))
    {
        i = -1;
        while (!philo_no_dead(data) && ++i < data->nb_philo)
        {
			usleep(100);
			pthread_mutex_lock(&data->lastmeal_check);
			elapsed_time = get_time() - philo[i].last_meal;
			pthread_mutex_unlock(&data->lastmeal_check);
            if (elapsed_time > data->time_to_die)
            {
            	pthread_mutex_lock(&data->dead_check);
                data->philo_dead = true;
				pthread_mutex_unlock(&data->dead_check);
                print_status(data, philo[i].philo, "died", 1);
                break;
            }
        }
        if (philo_no_dead(data))
            break;
        check_philo_ate(data);
	}
}

int init_thread(t_data *data)
{
	t_philo *philo;
    int i;

	i = 0;
	philo = data->philo;
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
    return (0);
}
//./philo 199 800 300 100