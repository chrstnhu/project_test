/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   philo_bonus.h                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: chrhu <chrhu@student.42.fr>                +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/04/11 17:04:04 by chrhu             #+#    #+#             */
/*   Updated: 2024/06/18 13:38:29 by chrhu            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef PHILO_BONUS_H
# define PHILO_BONUS_H

# include <unistd.h>
# include <stdio.h>
# include <stdlib.h>
# include <pthread.h>
# include <stdbool.h>
# include <semaphore.h>
# include <fcntl.h>
# include <sys/time.h>
# include <sys/stat.h>
# include <sys/wait.h>
# include <sys/types.h>
# include <signal.h>

# define DEFAULT "\033[0;39m"
# define RED "\033[0;31m"
# define GREEN "\033[0;32m"

# define SEM_FORKS "/forks"
# define SEM_DEAD "/dead"
# define SEM_PRINT "/print"
# define SEM_LAST_MEAL "/last_meal"
# define SEM_STOP "/stop"
# define SEM_DEATH_DECLARED "/death_declared"

struct	s_data;

typedef struct s_philo
{
	int				philo;
	long long		last_meal;
	struct s_data	*data;
	pthread_t		threads;
	pid_t			pid;
}	t_philo;

typedef struct s_data
{
	int				nb_philo;
	int				time_to_die;
	int				time_to_eat;
	int				time_to_sleep;
	int				time_to_think;
	bool			philo_dead;
	long long		start_time;
	sem_t			*forks;
	sem_t			*print;
	sem_t			*dead_check;
	sem_t			*lastmeal_check;
	sem_t			*sem_stop;
	sem_t			*death_declared;
	t_philo			*philo;
}	t_data;

// Number
int			check_number(int argc, char **argv);
int			ft_atoi(const char *nptr);

// Initialize
int			init_process(t_data *data);
int			init_semaphore(t_data *data);
int			init_philo(t_data *data);
int			initialize(t_data *data, char **argv);

// Execution
void		*action(t_philo *philo);
void		philo_eating(t_philo *philo, t_data *data);
void		wait_and_close(t_data *data);
void		print_status(t_data *data, int philo, const char *status, int dead);

// Calcul time
void		ft_usleep(t_data *data, long long time);
long long	get_time(void);

// Check
void		*check_dead(void *philosopher);
int			is_philo_dead(t_data *data);
int			is_time_exceed(t_data *data, t_philo *philo, int status);

// Clean
void		exit_dead(t_data *data);
void		sem_unlink_all(void);
void		sem_close_all(t_data *data);
void		free_memory(t_data *data);

#endif
