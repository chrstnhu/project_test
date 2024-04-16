#include "../includes/philo.h"

int init_mutex(t_data *data)
{
	int i = 0;

	while (i < data->philo)
	{
		if (pthread_mutex_init(&data->forks[i], NULL))
			return (1);
		i++;
	}
	if (pthread_mutex_init(&data->writing, NULL))
		return (1);
	if (pthread_mutex_init(&data->meal_check, NULL))
		return (1);
	return (0);
}

void *init_thread(void *data) 
{
    t_data *d = (t_data *)data;
    int i;
    pthread_t threads[NB_MAX];

    i = 0;
    while (i < d->nb_philo) {
        if (pthread_create(&threads[i], NULL, philo_eating, (void *)&d->philosopher[i]) != 0)
            return (void *)1;
        printf("Thread %d has started\n", i);
        i++;
    }
    i = 0;
    while (i < d->nb_philo) {
        if (pthread_join(threads[i], NULL) != 0)
            return (void *)5;
        printf("Thread %d has finished execution\n", i);
        i++;
    }
    i = 0;
    while (i < d->nb_philo) {
        pthread_mutex_destroy(&d->forks[i]);
        i++;
    }
    pthread_mutex_destroy(&d->meal_check);
    return NULL;
}

int	init_philo(t_data *data, char **argv)
{
	// *data = (t_philo){0};
	
    data->nb_philo = ft_atoi(argv[1]);
    data->time_to_die = ft_atoi(argv[2]);
    data->time_to_eat = ft_atoi(argv[3]);
    data->time_to_sleep = ft_atoi(argv[4]);

	data->all_ate = 0;
	// Pour le 5e arg
	if (argv[5] != '\0')
		data->nb_eat = ft_atoi(argv[5]);
	else
		data->nb_eat = -1;

	if (data->nb_philo > 250 || data->nb_philo < 0 || data->time_to_die < 0
		|| data->time_to_eat < 0 || data->time_to_sleep < 0)
		return (1);
	return (0);
}