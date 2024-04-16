#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <pthread.h>
#include <sys/time.h>

#define EATING 0
#define SLEEPING 1
#define THINKING 2
#define FORK_AVAILABLE 3
#define FORK_TAKEN 4

#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <pthread.h>
#include <sys/time.h>

pthread_mutex_t forks_mutex;
pthread_mutex_t print_mutex;

typedef struct philosopher {
    int id;
    int state;
    int left_fork;
    int right_fork;
    int times_to_eat;
    struct timeval last_meal_time;
    pthread_t thread;
} philosopher_t;

philosopher_t *philosophers;
int number_of_philosophers;
int time_to_die;
int time_to_eat;
int time_to_sleep;
int number_of_times_each_philosopher_must_eat = -1;

// Function to get current timestamp in milliseconds
long long current_timestamp() {
    struct timeval te;
    gettimeofday(&te, NULL); // get current time
    return te.tv_sec * 1000LL + te.tv_usec / 1000; // calculate milliseconds
}

// Function to simulate eating
void eat(philosopher_t *philosopher) {
    pthread_mutex_lock(&print_mutex);
    printf("%lld %d is eating\n", current_timestamp(), philosopher->id);
    pthread_mutex_unlock(&print_mutex);
    usleep(time_to_eat * 1000);
    pthread_mutex_lock(&forks_mutex);
    philosopher->state = SLEEPING;
    pthread_mutex_unlock(&forks_mutex);
}

// Function to simulate sleeping
void ok_sleep(philosopher_t *philosopher) {
    pthread_mutex_lock(&print_mutex);
    printf("%lld %d is sleeping\n", current_timestamp(), philosopher->id);
    pthread_mutex_unlock(&print_mutex);
    usleep(time_to_sleep * 1000);
    pthread_mutex_lock(&forks_mutex);
    philosopher->state = THINKING;
    pthread_mutex_unlock(&forks_mutex);
}

// Function to simulate thinking
void think(philosopher_t *philosopher) {
    pthread_mutex_lock(&print_mutex);
    printf("%lld %d is thinking\n", current_timestamp(), philosopher->id);
    pthread_mutex_unlock(&print_mutex);
}

// Function to check if a philosopher should die
void check_death(philosopher_t *philosopher) {
    long long current_time = current_timestamp();
    long long time_since_last_meal = current_time - philosopher->last_meal_time.tv_sec * 1000LL - philosopher->last_meal_time.tv_usec / 1000;
    if (time_since_last_meal >= time_to_die) {
        pthread_mutex_lock(&print_mutex);
        printf("%lld %d died\n", current_timestamp(), philosopher->id);
        pthread_mutex_unlock(&print_mutex);
        exit(1);
    }
}

// Function to perform philosopher actions
void *philosopher_action(void *arg) {
    philosopher_t *philosopher = (philosopher_t *)arg;
    while (1) {
        pthread_mutex_lock(&forks_mutex);
        if (philosopher->state == THINKING) {
            think(philosopher);
            philosopher->state = FORK_AVAILABLE;
            pthread_mutex_unlock(&forks_mutex);
        } else if (philosopher->state == FORK_AVAILABLE) {
            if (pthread_mutex_trylock(&forks_mutex) == 0) {
                printf("%lld %d has taken a fork\n", current_timestamp(), philosopher->id);
                if (pthread_mutex_trylock(&forks_mutex) == 0) {
                    printf("%lld %d has taken a fork\n", current_timestamp(), philosopher->id);
                    eat(philosopher);
                    philosopher->times_to_eat--;
                    gettimeofday(&philosopher->last_meal_time, NULL);
                    check_death(philosopher);
                    if (number_of_times_each_philosopher_must_eat != -1 && philosopher->times_to_eat == 0)
                        break;
                }
                pthread_mutex_unlock(&forks_mutex);
                pthread_mutex_unlock(&forks_mutex);
            }
        } else if (philosopher->state == SLEEPING) {
            ok_sleep(philosopher);
        }
    }
    return NULL;
}

int main(int argc, char *argv[]) {
    if (argc < 5 || argc > 6) {
        printf("Usage: %s number_of_philosophers time_to_die time_to_eat time_to_sleep [number_of_times_each_philosopher_must_eat]\n", argv[0]);
        return 1;
    }

    number_of_philosophers = atoi(argv[1]);
    time_to_die = atoi(argv[2]);
    time_to_eat = atoi(argv[3]);
    time_to_sleep = atoi(argv[4]);

    if (argc == 6)
        number_of_times_each_philosopher_must_eat = atoi(argv[5]);

    pthread_mutex_init(&forks_mutex, NULL);
    pthread_mutex_init(&print_mutex, NULL);

    philosophers = (philosopher_t *)malloc(number_of_philosophers * sizeof(philosopher_t));

    for (int i = 0; i < number_of_philosophers; i++) {
        philosophers[i].id = i + 1;
        philosophers[i].state = THINKING;
        philosophers[i].left_fork = i;
        philosophers[i].right_fork = (i + 1) % number_of_philosophers;
        philosophers[i].times_to_eat = number_of_times_each_philosopher_must_eat;
        gettimeofday(&philosophers[i].last_meal_time, NULL);

        pthread_create(&philosophers[i].thread, NULL, philosopher_action, (void *)&philosophers[i]);
    }

    for (int i = 0; i < number_of_philosophers; i++) {
        pthread_join(philosophers[i].thread, NULL);
    }

    pthread_mutex_destroy(&forks_mutex);
    pthread_mutex_destroy(&print_mutex);

    free(philosophers);

    return 0;
}
