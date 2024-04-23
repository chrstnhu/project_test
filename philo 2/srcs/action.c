#include "../includes/philo.h"

void philo_eating(t_data *data, t_philo *philo) {
    pthread_mutex_lock(&(data->forks[philo->left_fork]));
    print_status(data, philo->philo, "has taken a fork", 0);
    pthread_mutex_lock(&(data->forks[philo->right_fork]));
    print_status(data, philo->philo, "has taken a fork", 0);
    pthread_mutex_lock(&(data->meal_check));
    print_status(data, philo->philo, "is eating", 0);
    pthread_mutex_unlock(&(data->lastmeal_check));
    philo->last_meal = data->elapsed_time;
    pthread_mutex_unlock(&(data->lastmeal_check));

    pthread_mutex_unlock(&(data->meal_check));
    philo->philo_ate++;
    ft_usleep(data, data->time_to_eat);
    pthread_mutex_unlock(&(data->forks[philo->left_fork]));
    pthread_mutex_unlock(&(data->forks[philo->right_fork]));
}

void philo_sleeping(t_data *data, t_philo *philo) {
    print_status(data, philo->philo, "is sleeping", 0);
    ft_usleep(data, data->time_to_sleep);
    print_status(data, philo->philo, "is thinking", 0);
}

void *action(void *philosopher) {
    t_philo *philo = (t_philo *)philosopher;
    t_data *data = philo->data;

    if (philo->philo % 2)
        ft_usleep(data, 10);

    while (!(data->philo_dead)) {
        philo_eating(data, philo);
        if (data->all_ate)
            break;
        philo_sleeping(data, philo);
    }
    return (NULL);
}

struct timeval ll_to_timeval(long long ll_time) {
    struct timeval tv;
    tv.tv_sec = ll_time / 1000;
    tv.tv_usec = (ll_time % 1000) * 1000;
    return tv;
}

// Utilisation de ll_to_timeval dans la fonction check_dead
void check_dead(t_data *data, t_philo *philo) {
    while (!(data->all_ate)) {
        int i = -1;
        while (!(data->philo_dead) && ++i < data->nb_philo) {
            usleep(100);
            pthread_mutex_lock(&(data->dead_check));
            // Utilisation de ll_to_timeval pour convertir last_meal en struct timeval
            struct timeval last_meal_tv = ll_to_timeval(philo[i].last_meal);
            if ((get_time_ms(data->current, last_meal_tv)) > data->time_to_die) {
                data->philo_dead = true;
                print_status(data, philo[i].philo, "died", 1);
                pthread_mutex_unlock(&(data->dead_check));
                break;
            }
            pthread_mutex_unlock(&(data->dead_check));
        }
        if (data->philo_dead)
            break;
        i = 0;
        while (data->nb_eat != -1 && i < data->nb_philo && philo[i].philo_ate >= data->nb_eat)
            i++;
        pthread_mutex_lock(&(data->eat_check));
        if (data->nb_philo == i)
            data->all_ate = true;
        pthread_mutex_unlock(&(data->eat_check));
    }
}


int init_thread(t_data *data) {
    t_philo *philo = data->philo;
    int i = 0;

    while (i < data->nb_philo) {
        if ((pthread_create(&(philo[i].threads), NULL, action, &philo[i])) != 0)
            return (1);
        philo[i].last_meal = data->start_time;
        i++;
    }
    check_dead(data, data->philo);
    // thread_wait_destroy(data, philo);
    return (0);
}
