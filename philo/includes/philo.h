#ifndef PHILO_H
#define PHILO_H

# include <sys/time.h>
# include <stdint.h>
# include <unistd.h>
# include <stdio.h>
# include <stdlib.h>
# include <string.h>
# include <pthread.h>

# define DEFAULT "\033[0;39m"
# define RED "\033[0;31m"
# define GREEN "\033[0;32m"

# define NB_MAX      220

typedef struct s_philo 
{
	int philo;
	int philo_ate;
	int left_fork;
	int right_fork;
	long long last_meal;
	
	pthread_mutex_t	dead_check;
	pthread_mutex_t meal_check;
	pthread_mutex_t writing;
	pthread_mutex_t forks[NB_MAX];

	pthread_t nb_threads;
}	t_philo;

typedef struct s_data
{
	int nb_philo;
	int nb_eat;
	int all_ate;

	int time_to_die;
	int time_to_eat;
	int time_to_sleep;
	
	int philo_eat;
	int philo_think;
	int philo_sleep;
	int philo_died;
	long long current_time;

	pthread_mutex_t	dead_check;
	pthread_mutex_t meal_check;
	pthread_mutex_t writing;
	t_philo		philo[NB_MAX];
}	t_data;

//Initialise
int	ft_atoi(const char *nptr);
int	init_philo(t_philo *data, char **argv);
void *init_thread(t_philo *philo);
void *action(t_data *data);
int init_mutex(t_philo *data);
long long get_time(void);


#endif