/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main_bonus.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: chrhu <chrhu@student.42.fr>                +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/06/01 17:20:19 by chrhu             #+#    #+#             */
/*   Updated: 2024/06/13 19:04:13 by chrhu            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../includes/philo_bonus.h"

int	main(int argc, char **argv)
{
	t_data	data;

	if (argc == 5 || argc == 6)
	{
		if ((check_number(argc, argv) == 1) || (initialize(&data, argv) == 1))
		{
			printf("%sError init philo%s", RED, DEFAULT);
			return (1);
		}
		init_process(&data);
	}
	else
	{
		printf("%sPlease enter 5/6 arguments :", RED);
		printf("./philo number_of_philosopher time_to_die");
		printf("time_to_eat time_to_sleep number_of_meals%s\n", DEFAULT);
		return (1);
	}
	return (0);
}
