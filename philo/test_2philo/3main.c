#include "3philo.h"

void print_status(t_data *data, t_philo *philo, const char *status) 
{
    struct timeval current_time;
    gettimeofday(&current_time, NULL);
    printf("%ld ms %d is %s\n", ((current_time.tv_sec - data->start_time.tv_sec) * 1000) +
                                 ((current_time.tv_usec - data->start_time.tv_usec) / 1000),
           philo->id, status);
}

void *do_think(t_data *data, t_philo *philo) 
{
    print_status(data, philo, "thinking");
    data->thinking++;
    return (NULL);
}


void *do_eat(t_data *data, t_philo *philo) 
{
    print_status(data, philo, "eating");
    data->eating++;
    return (NULL);
}

void *do_sleep(t_data *data, t_philo *philo) 
{
    print_status(data, philo, "sleeping");
    data->sleeping++;
    return (NULL);
}
int check_time_to_die(t_data *data, t_philo *philo) 
{
    struct timeval current_time;
    gettimeofday(&current_time, NULL);
    long elapsed_time = ((current_time.tv_sec - philo->last_activity_time.tv_sec) * 1000000) +
                        (current_time.tv_usec - philo->last_activity_time.tv_usec);
    if (elapsed_time >= data->time_to_die) 
    {
        long elapsed_milliseconds = (elapsed_time % 1000000) / 1000; 
        printf("\nDebbug : %ld ms", elapsed_milliseconds);
        return (elapsed_time >= data->time_to_die);
    }
    return (0);
}

void *action(void *thread_id) 
{
    int i = 0;
	t_data *data = (t_data *)thread_id;
	t_philo *philo;
    gettimeofday(&data->start_time, NULL);
	data->philo1.thread_state = EATING;
	data->philo2.thread_state = SLEEPING;
    while (i < 6) 
	{
		pthread_mutex_lock(&data->eat_mutex);
        if (data->philo1.thread_state == EATING) {
            do_eat(data, &data->philo1);
            data->philo1.thread_state = SLEEPING;
        }
        pthread_mutex_unlock(&data->eat_mutex);

        pthread_mutex_lock(&data->sleep_mutex);
        if (data->philo1.thread_state == SLEEPING) {
            do_sleep(data, &data->philo1);
            if (data->time_to_think != 0)
                data->philo1.thread_state = THINKING;
            else
                data->philo1.thread_state = EATING;
        }
        pthread_mutex_unlock(&data->sleep_mutex);

        pthread_mutex_lock(&data->think_mutex);
        if (data->philo1.thread_state == THINKING) {
            do_think(data, &data->philo1);
            data->philo1.thread_state = EATING;
        }
        pthread_mutex_unlock(&data->think_mutex);


		pthread_mutex_lock(&data->eat_mutex);
        if (data->philo2.thread_state == EATING) {
            do_eat(data, &data->philo1);
            data->philo2.thread_state = SLEEPING;
        }
        pthread_mutex_unlock(&data->eat_mutex);

        pthread_mutex_lock(&data->sleep_mutex);
        if (data->philo2.thread_state == SLEEPING) {
            do_sleep(data, &data->philo2);
            if (data->time_to_think != 0)
                data->philo2.thread_state = THINKING;
            else
                data->philo2.thread_state = EATING;
        }
        pthread_mutex_unlock(&data->sleep_mutex);

        pthread_mutex_lock(&data->think_mutex);
        if (data->philo2.thread_state == THINKING) {
            do_think(data, &data->philo2);
            data->philo2.thread_state = EATING;
        }
        pthread_mutex_unlock(&data->think_mutex);


			if (check_time_to_die(data, &data->philo1) || check_time_to_die(data, &data->philo2))
			{
        	    printf("\nPhilosopher has died!\n");
				return NULL; // Sortie de la fonction si le philosophe est mort
			}
	        i++;
    }
    return NULL;
}



void init_philo(t_data *data, int argc, char **argv)
{
	(void)argc;
	(void)argv;
	data->time_to_eat =  150000;  // 150 ms
	data->time_to_sleep = 400000; // 400 ms
	data->time_to_die = 500000;
	if (data->time_to_eat < data->time_to_sleep)
		data->time_to_think = data->time_to_sleep - data->time_to_eat;
	else
		data->time_to_think = data->time_to_eat - data->time_to_sleep;
    data->eating = 0;
	data->sleeping = 0;
	data->thinking = 0;
	data->philo1.id = 1;
	data->philo2.id = 2;
}
int main(int argc, char **argv) 
{
	t_data data;
    pthread_t t1_philo;
    pthread_t t2_philo;

    printf("Before Thread\n");
    gettimeofday(&data.start_time, NULL);

	init_philo(&data, argc, argv);
	
	pthread_mutex_init(&data.activity_mutex, NULL);
	pthread_mutex_init(&data.eat_mutex, NULL);
	pthread_mutex_init(&data.sleep_mutex, NULL);
	pthread_mutex_init(&data.think_mutex, NULL);

    if (pthread_create(&t1_philo, NULL, &action, (void *)&data) != 0)
        return 1;

    if (pthread_create(&t2_philo, NULL, &action, (void *)&data) != 0)
        return 2;

    if (pthread_join(t1_philo, NULL))
        return 3;

    if (pthread_join(t2_philo, NULL))
        return 4;

	pthread_mutex_destroy(&data.activity_mutex);
	pthread_mutex_destroy(&data.eat_mutex);
	pthread_mutex_destroy(&data.sleep_mutex);
	pthread_mutex_destroy(&data.think_mutex);

    printf("Number of eating : %d\n", data.eating);
    printf("Number of sleeping : %d\n", data.sleeping);
    printf("Number of think : %d\n", data.thinking);

    return 0;
}
