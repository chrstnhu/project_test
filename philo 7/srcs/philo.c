/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   philo.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: chrhu <chrhu@student.42.fr>                +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/04/11 17:20:19 by chrhu             #+#    #+#             */
/*   Updated: 2024/05/29 13:06:44 by chrhu            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../includes/philo.h"

int main(int argc, char *argv[]) 
{
	t_data data;

	if (argc == 5 || argc == 6)
	{
		if ((check_number(argc,argv) == 1) || (initialize(&data, argv) == 1))
		{
			printf("%sError init philo%s", RED, DEFAULT);
			return (1);
		}
		if (data.nb_philo == 1)
			handle_one_philo(&data);
		else
			init_thread(&data);
	}
	else
	{
        printf("%sPlease enter 5/6arguments :", RED);
		printf("./philo nb_of_philo time_to_die");
		printf("time_to_eat time_to_sleep number_of_eat%s\n", DEFAULT);
        return (1);
    }
    return (0);
}
