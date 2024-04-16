#include "2philo.h"
/*

void print_status(int philosopher_num, const char *status) 
{
    struct timeval current_time;
    gettimeofday(&current_time, NULL);
    printf("%ld ms %d is %s\n", ((current_time.tv_sec - start_time.tv_sec) * 1000) +
                                 ((current_time.tv_usec - start_time.tv_usec) / 1000),
           philosopher_num, status);
}

void *do_action(int philosopher_num, char *action) 
{
    print_status(philosopher_num, action);
	if (action[0] == 'e') eating++;
    else if (action[0] == 's') sleeping++;
    else thinking++;
    return NULL;
}


void *do_think(int philosopher_num) 
{
    print_status(philosopher_num, "thinking");
    think++;

    return NULL;
}


void *do_eat(int philosopher_num) 
{
    print_status(philosopher_num, "eating");
    eating++;
    return NULL;
}

void *do_sleep(int philosopher_num) 
{
    print_status(philosopher_num, "sleeping");
    sleeping++;
    return NULL;
}


int check_time_to_die(int philosopher_num, struct timeval last_activity_time) 
{
    struct timeval current_time;
    gettimeofday(&current_time, NULL);
    long elapsed_time = ((current_time.tv_sec - last_activity_time.tv_sec) * 1000000) +
                        (current_time.tv_usec - last_activity_time.tv_usec);
    if (elapsed_time >= time_to_die) 
    {
        long elapsed_milliseconds = (elapsed_time % 1000000) / 1000; 
        printf("\nDebbug : %ld ms", elapsed_milliseconds);
        return(1);
    }
    return (0);
}


void *action(void *thread_id) 
{
    int i = 0;
    int id = *((intptr_t *)thread_id);
    int philosopher_num = id;
    struct timeval last_activity_time;
    gettimeofday(&last_activity_time, NULL);

    while (i < 6) 
	{
		struct timeval current_time;
        gettimeofday(&current_time, NULL);
		long elapsed_time = (current_time.tv_sec - last_activity_time.tv_sec) * 1000 +
                            (current_time.tv_usec - last_activity_time.tv_usec) / 1000;

		if (check_time_to_die(philosopher_num, last_activity_time))
		{
			pthread_mutex_lock(&activity_mutex);
			printf("Debbug : %ld ms Philosopher %d has died!\n", elapsed_time, philosopher_num);
			pthread_mutex_unlock(&activity_mutex);
			return NULL; // Sortie de la fonction si le philosophe est mort
		}
			if (id == 1) 
			{
				if (thread1_state == EATING) 
				{
					pthread_mutex_lock(&eat_mutex);
					gettimeofday(&last_activity_time, NULL); // Update last activity time
					do_action(philosopher_num, "eating");
					usleep(time_to_eat);
					pthread_mutex_unlock(&eat_mutex);
					thread1_state = SLEEPING;
				} 
				else if (thread1_state == SLEEPING)
				{
					pthread_mutex_lock(&sleep_mutex);
					do_action(philosopher_num, "sleeping");
					usleep(time_to_sleep);
					pthread_mutex_unlock(&sleep_mutex);
					if (time_to_think != 0) 
						thread1_state = THINKING;
					else
						thread1_state = EATING;
				}
				else if (thread1_state == THINKING)
				{
					pthread_mutex_lock(&think_mutex);
					do_action(philosopher_num, "thinking");
					usleep(time_to_think);
					pthread_mutex_unlock(&think_mutex);
					thread1_state = EATING;
				}
			}

			if (id == 2) 
			{
				if (thread2_state == SLEEPING) 
				{
					pthread_mutex_lock(&sleep_mutex);
					do_action(philosopher_num, "sleeping");
					usleep(time_to_sleep);
					pthread_mutex_unlock(&sleep_mutex);
					if (time_to_think != 0) 
						thread2_state = THINKING;
					else
						thread2_state = EATING;
				} 
				else if (thread2_state == EATING)
				{
					pthread_mutex_lock(&eat_mutex);
					gettimeofday(&last_activity_time, NULL); // Update last activity time
					do_action(philosopher_num, "eating");
					usleep(time_to_eat);
					pthread_mutex_unlock(&eat_mutex);
					
					thread2_state = SLEEPING;
				}
				else if (thread2_state == THINKING)
				{
					pthread_mutex_lock(&think_mutex);
					do_action(philosopher_num, "thinking");
					usleep(time_to_think);
					pthread_mutex_unlock(&think_mutex);
					thread2_state = EATING;
				}
			}
        current_thread = (current_thread % 2) + 1;
        i++;
    }
    return NULL;
}



int main() 
{
    pthread_t t1_philo;
    pthread_t t2_philo;
    intptr_t thread1_id = 1;
    intptr_t thread2_id = 2;

    printf("Before Thread\n");
    gettimeofday(&start_time, NULL);

    pthread_mutex_init(&activity_mutex, NULL);

    if (pthread_create(&t1_philo, NULL, &action, (void *)&thread1_id) != 0)
        return 1;

    if (pthread_create(&t2_philo, NULL, &action, (void *)&thread2_id) != 0)
        return 2;

    if (pthread_join(t1_philo, NULL))
        return 3;

    if (pthread_join(t2_philo, NULL))
        return 4;

    pthread_mutex_destroy(&activity_mutex);

    printf("Number of eating : %d\n", eating);
    printf("Number of sleeping : %d\n", sleeping);
    printf("Number of think : %d\n", thinking);

    return 0;
}
*/


#include "2philo.h"

long long start_time;

long long get_time(void) 
{
    struct timeval tv;

    if (gettimeofday(&tv, NULL) < 0)
		printf("%sError function : gettimeofday()%s\n", RED, DEFAULT);
    return (((long long)tv.tv_sec) * 1000) + ((long long)tv.tv_usec / 1000);
}

void print_status(int philosopher_num, const char *status) 
{
    long long current_time = get_time();
    printf("%lld ms %d is %s\n", current_time - start_time, philosopher_num, status);
}

void *do_action(int philosopher_num, char *action) 
{
    print_status(philosopher_num, action);
	if (action[0] == 'e') eating++;
    else if (action[0] == 's') sleeping++;
    else thinking++;
    return NULL;
}

int check_time_to_die(int philosopher_num, long long last_meal_start) 
{
    long long current_time = get_time();
    long long elapsed_time = current_time - last_meal_start;
    if (elapsed_time >= time_to_die) 
    {
		printf("\nEnter elapsed_time\n");
        long elapsed_milliseconds = elapsed_time % 1000;
        printf("\nDebbug : %ld ms Philosopher %d has died!\n", elapsed_milliseconds, philosopher_num);
        return(1);
    }
    return (0);
}

void *action(void *thread_id) 
{
    int i = 0;
    int id = *((intptr_t *)thread_id);
    int philosopher_num = id;
    long long last_activity_time = get_time() - start_time;
    long long last_meal_start = start_time;
	long long time_since_last_meal = 0;

    while (i < 6) 
	{
		if (id == 1) 
		{
			if (thread1_state == EATING) 
			{
				last_meal_start = get_time();
				pthread_mutex_lock(&eat_mutex);
				last_activity_time = get_time() - start_time; 
				do_action(philosopher_num, "eating");
				usleep(time_to_eat);
				pthread_mutex_unlock(&eat_mutex);
				thread1_state = SLEEPING;
			} 
			else if (thread1_state == SLEEPING)
			{
				pthread_mutex_lock(&sleep_mutex);
				last_activity_time = get_time() - start_time; 
				do_action(philosopher_num, "sleeping");
				usleep(time_to_sleep);
				pthread_mutex_unlock(&sleep_mutex);
				if (time_to_think != 0) 
					thread1_state = THINKING;
				else
					thread1_state = EATING;
			}
			else if (thread1_state == THINKING)
			{
				pthread_mutex_lock(&think_mutex);
				last_activity_time = get_time() - start_time;
				do_action(philosopher_num, "thinking");
				usleep(time_to_think);
				pthread_mutex_unlock(&think_mutex);
				thread1_state = EATING;
			}
		}

		if (id == 2) 
		{
			if (thread2_state == SLEEPING) 
			{
				pthread_mutex_lock(&sleep_mutex);
				last_activity_time = get_time() - start_time;
				do_action(philosopher_num, "sleeping");
				usleep(time_to_sleep);
				pthread_mutex_unlock(&sleep_mutex);
				if (time_to_think != 0) 
					thread2_state = THINKING;
				else
					thread2_state = EATING;
			} 
			else if (thread2_state == EATING)
			{
				last_meal_start = get_time();
				pthread_mutex_lock(&eat_mutex);
				last_activity_time = get_time() - start_time; 
				do_action(philosopher_num, "eating");
				usleep(time_to_eat);
				pthread_mutex_unlock(&eat_mutex);
				
				thread2_state = SLEEPING;
			}
			else if (thread2_state == THINKING)
			{
				pthread_mutex_lock(&think_mutex);
				last_activity_time = get_time() - start_time;
				do_action(philosopher_num, "thinking");
				usleep(time_to_think);
				pthread_mutex_unlock(&think_mutex);
				thread2_state = EATING;
			}
		}

		if (check_time_to_die(philosopher_num, last_meal_start))
		{
			pthread_mutex_lock(&activity_mutex);
			printf("Debbug : Philosopher %d has died!\n", philosopher_num);
			pthread_mutex_unlock(&activity_mutex);
			return NULL;
		}
		long long current_time_since_last_meal = get_time() - last_meal_start;
		printf("Debug: Time since last meal for Philosopher %d: %lld ms\n", philosopher_num, current_time_since_last_meal);

        i++;
    }
    return NULL;
}


int main() 
{
    pthread_t t1_philo;
    pthread_t t2_philo;
    intptr_t thread1_id = 1;
    intptr_t thread2_id = 2;

    printf("Before Thread\n");
    start_time = get_time();

    pthread_mutex_init(&activity_mutex, NULL);

    if (pthread_create(&t1_philo, NULL, &action, (void *)&thread1_id) != 0)
        return 1;

    if (pthread_create(&t2_philo, NULL, &action, (void *)&thread2_id) != 0)
        return 2;

    if (pthread_join(t1_philo, NULL))
        return 3;

    if (pthread_join(t2_philo, NULL))
        return 4;

    pthread_mutex_destroy(&activity_mutex);

    printf("Number of eating : %d\n", eating);
    printf("Number of sleeping : %d\n", sleeping);
    printf("Number of think : %d\n", thinking);

    return 0;
}
