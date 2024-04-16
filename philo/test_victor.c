#include <stdio.h>
#include <pthread.h>

# define NC	"\e[0m"
# define YELLOW	"\e[1;33m"


/*
Ceci s’appelle une situation de compétition ou un accès concurrent (« data race » en anglais). 
Ce genre de situation survient quand le programme dépend de la progression ou du timing 
d’autres événements incontrôlables. Et il est impossible de prédire si le système d’exploitation 
choisira le bon ordonnancement pour nos threads.
--> empêcher un thread de lire une valeur quand l un le modifier -> mutex !

*/
/*
philo ./a.out
Main: Creation du premier thread [139791055525440]
Thread [139791055525440]: Le plus grand ennui c'est d'exister sans vivre.
Main: Creation du second thread [139791047132736]
Thread [139791047132736]: Le plus grand ennui c'est d'exister sans vivre.
Main: Union du premier thread [139791055525440]
Main: Union du second thread [139791047132736]
➜  philo ./a.out
Main: Creation du premier thread [139974536074816]
Thread [139974536074816]: Le plus grand ennui c'est d'exister sans vivre.
Main: Creation du second thread [139974527682112]
Main: Union du premier thread [139974536074816]
Thread [139974527682112]: Le plus grand ennui c'est d'exister sans vivre.
Main: Union du second thread [139974527682112]

pas dans le meme ordre parce qu il s execute parallelement
*/

// thread_routine est la fonction que le thread invoque directement
// après sa création. Le thread se termine à la fin de cette fonction.
void	*thread_routine(void *data)
{
	pthread_t tid;

	// La fonction pthread_self() renvoie
	// l'identifiant propre à ce thread.
	tid = pthread_self();
	printf("%sThread [%ld]: Le plus grand ennui c'est d'exister sans vivre.%s\n",
		YELLOW, tid, NC);
	return (NULL); // Le thread termine ici.
}

int	main(void)
{
	pthread_t	tid1;	// Identifiant du premier thread
	pthread_t	tid2;	// Identifiant du second thread

	// Création du premier thread qui va directement aller
	// exécuter sa fonction thread_routine.
	pthread_create(&tid1, NULL, thread_routine, NULL);
	printf("Main: Creation du premier thread [%ld]\n", tid1);
	// Création du second thread qui va aussi exécuter thread_routine.
	pthread_create(&tid2, NULL, thread_routine, NULL);
	printf("Main: Creation du second thread [%ld]\n", tid2);
	// Le main thread attend que le nouveau thread
	// se termine avec pthread_join.
	pthread_join(tid1, NULL);
	printf("Main: Union du premier thread [%ld]\n", tid1);
	pthread_join(tid2, NULL);
	printf("Main: Union du second thread [%ld]\n", tid2);
	return (0);
}
