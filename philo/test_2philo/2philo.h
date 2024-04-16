#ifndef PHILO_H

#define PHILO_H

#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <pthread.h>
#include <sys/time.h>

# define DEFAULT "\033[0;39m"
# define RED "\033[0;31m"
# define GREEN "\033[0;32m"

# define MAX 220
# define EATING 1
# define SLEEPING 2
# define THINKING 3

int eating = 0;
int sleeping = 0;
int thinking = 0;
int current_thread = 1;
int thread1_state = 1; // 1 pour manger, 0 pour dormir
int thread2_state = 1; // 1 pour manger, 0 pour dormir

struct timeval eat_start_time;
struct timeval last_meal_time;
// struct timeval start_time;
long long start_time;
long long last_detection_time;

pthread_mutex_t activity_mutex;
// pthread_mutex_t fork[MAX];
pthread_mutex_t eat_mutex;
pthread_mutex_t sleep_mutex;
pthread_mutex_t think_mutex;


int time_to_eat = 150000;  // 150 ms
int time_to_sleep = 400000; // 400 ms
int time_to_think = 100000;
int time_to_die = 500000;

void print_status(int philosopher_num, const char *status) ;
void *do_think(int philosopher_num) ;
void *do_eat(int philosopher_num) ;
void *do_sleep(int philosopher_num); 
void *action(void *thread_id);
// int check_time_to_die(int philosopher_num, struct timeval last_activity_time);
int check_time_to_die(int philosopher_num, long long last_meal_start) ;



#endif 