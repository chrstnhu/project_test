/*
Attention au deadlock

Un interblocage peut aussi arriver lorsqu’un thread attend un mutex qu’il possède déjà !

 Par exemple, thread T1 a acquis le mutex M1 et attend le mutex M2. 
 Entre temps, le thread T2 a acquis le mutex M2 et attend le mutex M1. 
 Dans ce cas, le programme reste en suspens et doit être tué.
*/

#include <stdio.h>
#include <pthread.h>

#define NC	"\e[0m"
#define YELLOW	"\e[33m"
#define BYELLOW	"\e[1;33m"
#define RED	"\e[31m"
#define GREEN	"\e[32m"

typedef	struct s_locks
{
	pthread_mutex_t	lock_1;
	pthread_mutex_t	lock_2;
	unsigned int	count;
}	t_locks;

// Le premier thread invoque cette routine :
void	*thread_1_routine(void *data)
{
	pthread_t	tid;
	t_locks		*locks;
	
	tid = pthread_self();
	locks = (t_locks *)data;
	printf("%sThread [%ld]: veut verrouiller lock 1%s\n", YELLOW, tid, NC);
	pthread_mutex_lock(&locks->lock_1);
	printf("%sThread [%ld]: possede lock 1%s\n", BYELLOW, tid, NC);
	printf("%sThread [%ld]: veut verrouiller lock 2%s\n", YELLOW, tid, NC);
	pthread_mutex_lock(&locks->lock_2);
	printf("%sThread [%ld]: possede lock 2%s\n", BYELLOW, tid, NC);
	locks->count += 1;
	printf("%sThread [%ld]: deverouille lock 2%s\n", BYELLOW, tid, NC);
	pthread_mutex_unlock(&locks->lock_2);
	printf("%sThread [%ld]: deverouille lock 1%s\n", BYELLOW, tid, NC);
	pthread_mutex_unlock(&locks->lock_1);
	printf("%sThread [%ld]: termine%s\n", YELLOW, tid, NC);
	return (NULL); // Le thread termine ici.
}

// Le deuxieme thread invoque cette routine :
void	*thread_2_routine(void *data)
{
	pthread_t	tid;
	t_locks		*locks;
	
	tid = pthread_self();
	locks = (t_locks *)data;
	printf("%sThread [%ld]: veut verrouiller lock 2%s\n", YELLOW, tid, NC);
	pthread_mutex_lock(&locks->lock_2);
	printf("%sThread [%ld]: possede lock 2%s\n", BYELLOW, tid, NC);
	printf("%sThread [%ld]: veut verrouiller lock 1%s\n", YELLOW, tid, NC);
	pthread_mutex_lock(&locks->lock_1);
	printf("%sThread [%ld]: possede lock 1%s\n", BYELLOW, tid, NC);
	locks->count += 1;
	printf("%sThread [%ld]: deverouille lock 1%s\n", BYELLOW, tid, NC);
	pthread_mutex_unlock(&locks->lock_1);
	printf("%sThread [%ld]: deverouille lock 2%s\n", BYELLOW, tid, NC);
	pthread_mutex_unlock(&locks->lock_2);
	printf("%sThread [%ld]: termine%s\n", YELLOW, tid, NC);
	return (NULL); // Le thread termine ici.
}

int	main(void)
{
	pthread_t	tid1;	// Identifiant du premier thread
	pthread_t	tid2;	// Identifiant du second thread
	t_locks		locks;	// Structure contenant 2 mutex

	locks.count = 0;
	// Initialisation des deux mutex :
	pthread_mutex_init(&locks.lock_1, NULL);
	pthread_mutex_init(&locks.lock_2, NULL);
	// Création des threads :
	pthread_create(&tid1, NULL, thread_1_routine, &locks);
	printf("Main: Creation du premier thread [%ld]\n", tid1);
	pthread_create(&tid2, NULL, thread_2_routine, &locks);
	printf("Main: Creation du second thread [%ld]\n", tid2);
	// Union des threads :
	pthread_join(tid1, NULL);
	printf("Main: Union du premier thread [%ld]\n", tid1);
	pthread_join(tid2, NULL);
	printf("Main: Union du second thread [%ld]\n", tid2);
	// Évaluation du résultat :
	if (locks.count == 2)
		printf("%sMain: OK: Le compte est %d\n", GREEN, locks.count);
	else
		printf("%sMain: ERREUR: Le compte est%u\n", RED, locks.count);
	// Destruction des mutex :
	pthread_mutex_destroy(&locks.lock_1);
	pthread_mutex_destroy(&locks.lock_2);
	return (0);
}
