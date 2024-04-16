#ifndef PHILO_H

#define PHILO_H

#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <pthread.h>
#include <sys/time.h>

#define EATING 1
#define SLEEPING 2
#define THINKING 3

typedef struct s_philo
{	
	int id;
	int thread_state;
	struct timeval last_activity_time;
}	t_philo;

typedef struct s_data
{
	t_philo philo1;
	t_philo philo2;
	pthread_mutex_t activity_mutex;

	pthread_mutex_t left_fork;
	pthread_mutex_t right_fork;

	pthread_mutex_t eat_mutex;
	pthread_mutex_t sleep_mutex;
	pthread_mutex_t think_mutex;
	int time_to_eat;
	int time_to_sleep;
	int time_to_think;
	int time_to_die;
	struct timeval start_time;
	int eating;
	int sleeping;
	int thinking;
}	t_data;

void print_status(t_data *data, t_philo *philo, const char *status) ;
void *do_think(t_data *data, t_philo *philo) ;
void *do_eat(t_data *data, t_philo *philo) ;
void *do_sleep(t_data *data, t_philo *philo); 
int check_time_to_die(t_data *data, t_philo *philo) ;
void *action(void *thread_id) ;
void init_philo(t_data *data, int argc, char **argv);


#endif 