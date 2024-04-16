#include <stdio.h>
#include <stdlib.h>
#include <pthread.h>
#include <unistd.h>

#define N 5

typedef enum 
{
    THINKING = 0,
    HUNGRY = 1,
    EATING = 2
} State;

State state[N];
pthread_mutex_t critical_region_mtx = PTHREAD_MUTEX_INITIALIZER;
pthread_mutex_t output_mtx = PTHREAD_MUTEX_INITIALIZER;
pthread_mutex_t forks[N];
pthread_t philosophers[N];

size_t left(size_t i) 
{
    return (i + N - 1) % N;
}

size_t right(size_t i) 
{
    return (i + 1) % N;
}

size_t my_rand(size_t min, size_t max) 
{
    return rand() % (max - min + 1) + min;
}

void test(size_t i)
{
    if (state[i] == HUNGRY && state[left(i)] != EATING && state[right(i)] != EATING) 
	{
        state[i] = EATING;
        pthread_mutex_unlock(&forks[i]);
    }
}

void think(size_t i) 
{
    size_t duration = my_rand(400, 800);

    pthread_mutex_lock(&output_mtx);
    printf("%zu is thinking %zums\n", i, duration);
    pthread_mutex_unlock(&output_mtx);

    usleep(duration * 1000);
}

void take_forks(size_t i) 
{
    pthread_mutex_lock(&critical_region_mtx);

    state[i] = HUNGRY;
    pthread_mutex_lock(&output_mtx);
    printf("\t\t%zu is HUNGRY\n", i);
    pthread_mutex_unlock(&output_mtx);
    test(i);
    pthread_mutex_unlock(&critical_region_mtx);

    pthread_mutex_lock(&forks[i]);
}

void eat(size_t i) 
{
    size_t duration = my_rand(400, 800);

    pthread_mutex_lock(&output_mtx);
    printf("\t\t\t\t%zu is eating %zums\n", i, duration);
    pthread_mutex_unlock(&output_mtx);

    usleep(duration * 1000);
}

void put_forks(size_t i) 
{
    pthread_mutex_lock(&critical_region_mtx);

    state[i] = THINKING;
    test(left(i));
    test(right(i));

    pthread_mutex_unlock(&critical_region_mtx);
}

void* philosopher(void* arg) 
{
    size_t i = (size_t)arg;
    while (1) 
	{
        think(i);
        take_forks(i);
        eat(i);
        put_forks(i);
    }
    return NULL;
}

int main() 
{
    printf("dp_14\n");
    srand(time(NULL));

    for (size_t i = 0; i < N; ++i) 
	{
        pthread_mutex_init(&forks[i], NULL);
        pthread_mutex_lock(&forks[i]);
        pthread_create(&philosophers[i], NULL, philosopher, (void*)i);
    }

    for (size_t i = 0; i < N; ++i)
        pthread_join(philosophers[i], NULL);

    return 0;
}
