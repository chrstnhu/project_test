#ifndef PHILO_H
#define PHILO_H

# include <sys/time.h>
# include <stdint.h>
# include <unistd.h>
# include <stdio.h>
# include <stdlib.h>
# include <string.h>
# include <pthread.h>
# include <stdbool.h>

# define DEFAULT "\033[0;39m"
# define RED "\033[0;31m"
# define GREEN "\033[0;32m"

# define NB_MAX      220

struct s_data;

typedef struct s_philo 
{
	int philo;
	int philo_ate;
	int left_fork;
	int right_fork;
	long long last_meal;
	struct s_data *data;
	
	// pthread_mutex_t	dead_check;
	// pthread_mutex_t meal_check;
	// pthread_mutex_t print;

	pthread_t threads;
}	t_philo;

typedef struct s_data
{
	int nb_philo;
	int nb_eat;

	int time_to_die;
	int time_to_eat;
	int time_to_sleep;
	int time_to_think;
	
	int philo_eat;
	int philo_think;
	int philo_sleep;

	bool philo_dead;
	bool all_ate;
	struct timeval start_time;
	bool program_should_end;

	pthread_mutex_t forks[NB_MAX];
	pthread_mutex_t	dead_check;
	pthread_mutex_t meal_check;
	//add
	pthread_mutex_t eat_check;
	pthread_mutex_t lastmeal_check;
	// pthread_mutex_t sleep_check;
	// pthread_mutex_t think_check;
	pthread_mutex_t print;

	t_philo		philo[NB_MAX];
}	t_data;

//Initialise
int	ft_atoi(const char *nptr);
int init_thread(t_data *data);
void *action(void *philosopher);
int init_mutex(t_data *data);
int init_philo(t_data *data);
int	initialize(t_data *data, char **argv);

long long get_ts_in_ms(struct timeval current, struct timeval start);

void print_status(t_data *data, int philo, const char *status, int dead);
void	philo_sleeping(t_data *data, t_philo *philo);
void check_dead(t_data *data, t_philo *philo);
int ft_usleep(t_data *data, long long time);
void handle_one_philo(t_data *data);
void thread_wait_destroy(t_data *data, t_philo *philo);
int check_number(int argc, char **argv);

#endif
