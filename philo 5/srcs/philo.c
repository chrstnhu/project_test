#include "../includes/philo.h"

//Convert seconds and microseconds to millisecondes
long long get_ts_in_ms(struct timeval current, struct timeval start)
{
    long ts_s;
    long ts_us;

    if (current.tv_usec < start.tv_usec)
        ts_s = current.tv_sec - start.tv_sec - 1;
    else
        ts_s = current.tv_sec - start.tv_sec;
    
    if (current.tv_usec < start.tv_usec)
        ts_us = current.tv_usec - start.tv_usec + 1000000 ;
    else
        ts_us = current.tv_usec - start.tv_usec;
    return (ts_s * 1000LL + ts_us / 1000);
}

int main(int argc, char *argv[]) 
{
    t_data data;
    struct timeval start_time, current_time;

    if (argc == 5 || argc == 6) {
        if ((check_number(argc,argv) == 1) || (initialize(&data, argv) == 1)) {
            printf("%sError init philo%s", RED, DEFAULT);
            return 1;
        }
        
        // Récupère le temps de démarrage
        gettimeofday(&start_time, NULL);

        if (data.nb_philo == 1) {
            handle_one_philo(&data);
        } else {
            init_thread(&data);
        }

        // Récupère le temps actuel
        gettimeofday(&current_time, NULL);

        // Calcul du temps écoulé depuis le début du programme
        long long elapsed_time = get_ts_in_ms(current_time, start_time);
        printf("Elapsed time: %lld milliseconds\n", elapsed_time);
    } 
	else 
	{
        printf("%sPlease enter 5/6 arguments :\n", RED);
        printf("./philo nb_of_philo time_to_die time_to_eat time_to_sleep number_of_eat%s\n", DEFAULT);
        return 1;
    }

    return 0;
}
