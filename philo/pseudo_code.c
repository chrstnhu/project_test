//Initialisation 

./philo number_of_philosophers time_to_die time_to_eat time_to_sleep [number_of_times_each_philosopher_must_eat]

ft_atoi pour avoir en chiffre

argv[0] = ./philo

argv[1] = number_of_philosophers | number of fork
(philo = n - 1 |   philo = n  | philo n + 1)

// Beginning of the simulation / since last meal
argv[2] = time_to_die   (in milliseconds)

// Time to eat with 2 forks
argv[3] = time_to_eat   (in milliseconds)

// Time to sleep
argv[4] = time_to_sleep (in milliseconds)


// Number of time to eat
argv[5] = number_of_times_each_philosopher_must_eat

// Time who they are thinking (doing nothing)
time_to_think = time_to_eat - time_to_sleep


_______________________________________________________________________________________________________

// On the shell
				
  current timestamps  philo     commentaire
◦  timestamp_in_ms      X     has taken a fork
◦  timestamp_in_ms      X     is sleeping
◦  timestamp_in_ms      X     is thinking
◦  timestamp_in_ms      X     died               ( < 10 ms after the actual death of the philosopher)


//Rules 
• Each philosopher should be a thread.
pthread_t philosopher[nombre max de philosophe];


• There is one fork between each pair of philosophers. Therefore, if there are several
philosophers, each philosopher has a fork on their left side and a fork on their right
side. If there is only one philosopher, there should be only one fork on the table.
pthread_mutex_t forks[nb de philosophe];

pthread_mutex_t	mutex;
pthread_mutex_init(&mutex, NULL);

//Create a thread
pthread_t t1;
pthread_create(t1, NULL, &routine, NULL)

//Similar to wait
pthread_join(t1, NULL);

//Destroy mutex
pthread_mutex_destroy(&mutex);

• To prevent philosophers from duplicating forks, you should protect the forks state
with a mutex for each of them.
pthread_mutex_t left_fork
pthread_mutex_t	right_fork

//Protect other threads executing at the same time between mutex
pthread_mutex_lock(&mutex)
//variable / function 
pthread_mutex_unlock(&mutex)
_____________________________________________________________________________________________________

// External function

memset

printf / write
malloc / free

//sleep the process -> thread
int usleep(useconds_t usec);
 The usleep() function suspends execution of the calling thread
       for (at least) usec microseconds.  The sleep may be lengthened
       slightly by any system activity or by the time spent processing
       the call or by the granularity of system timers

//Avoir le temps
int gettimeofday(struct timeval *tv, struct timezone *tz);
Les fonctions gettimeofday() servent à lire ou programmer l heure ainsi que le fuseau horaire (timezone). 
L argument tv est une structure timeval (décrite dans <sys/time.h>) :

struct timeval 
{
    time_t      tv_sec;  /* secondes */
    suseconds_t tv_usec; /* microsecondes */
};

et donne le nombre de secondes et microsecondes écoulées.

Si soit tv soit tz est NULL, la structure correspondante n est ni remplie ni renvoyée. 

// to get time :
#include <sys/time.h>
struct timeval tv;

gettimeofday(&tv,NULL);
tv.tv_sec // seconds
tv.tv_usec // microseconds

microseconde -> milliseconde  (1 -> 000,1)
long long	timestamp(void)
{
	struct timeval	t;

	gettimeofday(&t, NULL);
	return ((t.tv_sec * 1000) + (t.tv_usec / 1000));
}

//Initialise mutex
pthread_mutex_t	mutex;
pthread_mutex_init(&mutex, NULL);

//Create a thread
pthread_t t1;
pthread_create(t1, NULL, &routine, NULL)

//Protect other threads executing at the same time between mutex
pthread_mutex_lock(&mutex)
//variable / function 
pthread_mutex_unlock(&mutex)

//Similar to wait
pthread_join(t1, NULL);

//Destroy mutex
pthread_mutex_destroy(&mutex);

//
pthread_detach



Déclarations :
    - Structure Philosophe avec ID, état, fourchette gauche et droite, et temps du dernier repas
    - Mutex pour les fourchettes et l impression

Fonctions :
    - Manger(Philosophe)

		pthread mutex lock
		lock = 1;
		if (lock == 1 && (!philo[i] sleep && !philo[i] think))
		{
			if (philo eat)
			{
				time = time_to_eat;
				printf("time : %s, %d philo eat", time, philo[i])
				eat++;
			}
		}
		pthread mutex unlock
		lock = 0;

    - Dormir(Philosophe)

		pthread mutex lock
		lock = 1;
		if (lock == 1 && (!philo[i] eat && !philo[i] think))
		{
			if (philo sleep)
			{
				time = time_to_sleep;
				printf("time : %s, %d philo sleep", time, philo[i])
				sleep++;
			}
		}
		pthread mutex unlock
		lock = 0;


    - Penser(Philosophe)
    
		pthread mutex lock
		lock = 1;
		if (lock == 1 && (!philo[i] eat && !philo[i] sleep))
		{
			if (!philo eat && !philo sleep)
			{
					time = time_to_think;
				printf("time : %s, %d philo think", time, philo[i])
				think++;
			}
		}
		pthread mutex unlock
		lock = 0;
	
	- Vérifier_Mort(Philosophe) 
		-> time between the start of two meal
		if ((time first meal - time second meal) > (time_to_die))
			printf("1 philo died")
			exit(1);


		

    - Action_Philosophe(void*)

	thread philo[i]
	pthread_create(t1, NULL, &routine, NULL)
	

	i = 0;
	while (i < nb->philo)
	{
		if (philo[i] && philo[i + 2])
		{
			eat;
			if (eat == time_to_eat)
				sleep;
		}
		if (philo[i] != eat && philo != sleep)
		{
			think;
		}
		i++;
	}


Programme Principal :
    - Validation des arguments de la ligne de commande
    - Initialisation des mutex
    - Création des threads pour chaque philosophe
    - Attente de la fin des threads

	pthread_join(t[i], NULL);

    - Destruction des mutex


//Pseudo code :
1. Définir des structures : 
Créez des structures pour Philosopher et Fork, 
incluant toutes les données nécessaires telles que leurs identifiants et leurs états.

2 Initialiser les mutex : 
Initialisez des mutex pour chaque fourchette et toute autre ressource partagée.
-> fourchette
-> eat
-> sleep
-> think
-> time


3. Implémenter le comportement des philosophes : 
Créez une fonction comportement_philosophe qui représente le comportement d un philosophe. 
Cette fonction devrait gérer les actions du philosophe telles que manger, dormir et penser.
n     -> eat   -> sleep -> think -> eat   -> sleep
n + 1 -> think -> eat   -> sleep -> think -> eat
n + 2 -> eat   -> sleep -> think -> eat   -> sleep
n + 3 -> think -> eat   -> sleep -> think -> eat
n - 1 -> think -> think -> eat   -> sleep -> think


4. Créer les threads des philosophes : 
Créez une fonction thread_philosophes qui représente la fonction de thread pour chaque philosophe. 
À l intérieur de cette fonction, appelez comportement_philosophe et passez les données appropriées.

action (eat, sleep, think)


5. Dans la fonction principale : 
Créez des threads pour chaque philosophe, en passant les données nécessaires à chaque thread.

6. Assurer la synchronisation : 
Assurez une synchronisation adéquate à l aide des mutex pour éviter les courses de données, 
en particulier lors de l accès à des ressources partagées telles que les fourchettes.

7. Implémenter la journalisation : 
Assurez-vous que les événements sont correctement enregistrés selon le format spécifié.

8. Test : 
Testez votre programme avec différentes configurations pour garantir la correction et la synchronisation.
