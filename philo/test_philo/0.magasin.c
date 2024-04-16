#include "../includes/philo.h"

#include <stdio.h>
#include <stdlib.h>
#include <pthread.h>
#include <string.h> 

#include <unistd.h>
#include <stdint.h>

# define psleep(sec) sleep ((sec))
 
#define INITIAL_STOCK   20
#define NB_CLIENTS      5
 


/* Structure stockant les informations des threads clients et du magasin. */
 typedef struct
{
    int stock;
    pthread_mutex_t mutex; // Ajout du mutex
    pthread_t thread_store;
    pthread_t thread_clients[NB_CLIENTS];
} store_t;
 
static store_t store =
{
   .stock = INITIAL_STOCK,
};
 
 
/* Fonction pour tirer un nombre au sort entre 0 et max. */
static int get_random (int max)
{
	double val;
 
	val = (double) max * rand ();
	val = val / (RAND_MAX + 1.0);
 
	return ((int) val);
}
 
long long current_timestamp()
{
    struct timeval te;
    gettimeofday(&te, NULL); // Obtenir l'heure actuelle
    long long milliseconds = te.tv_sec * 1000LL + te.tv_usec / 1000; // Convertir en millisecondes
    return milliseconds;
}

/* Fonction pour le thread du magasin. */
static void *fn_store(void *p_data)
{
    while (1)
    {
		//ADD
        pthread_mutex_lock(&store.mutex); // Verrouillage du mutex
        if (store.stock <= 0)
        {
            printf("Le stock est épuisé. Arrêt du programme.\n");
            exit(EXIT_FAILURE);
        }
        pthread_mutex_unlock(&store.mutex); // Déverrouillage du mutex
    }
    return (NULL);
}

/* Fonction pour les threads des clients. */
static void *fn_clients(void *p_data)
{
    intptr_t nb = (intptr_t) p_data; // Utilisation de intptr_t pour convertir en pointeur

    while (1)
    {
        int val = get_random(6);

        psleep(get_random(3));

        // long long start_time = current_timestamp(); // Enregistrer le temps de début

		//ADD
        pthread_mutex_lock(&store.mutex); // Verrouillage du mutex
        if (store.stock >= val)
        {
            store.stock -= val;
            printf("Client %ld prend %d du stock, reste %d en stock !\n", nb, val, store.stock);
        } else
            printf("\nStock insuffisant pour le client %ld. Attente de réapprovisionnement.\n", nb);
        pthread_mutex_unlock(&store.mutex); // Déverrouillage du mutex

        // long long end_time = current_timestamp(); // Enregistrer le temps de fin
        // printf("\nTemps écoulé pour l'opération : %lld millisecondes\n", end_time - start_time); // Afficher le temps écoulé
    }
    return (NULL);
}


int main (void)
{
	int i = 0;
	int ret = 0;

	//Add
	if (pthread_mutex_init(&store.mutex, NULL) != 0)
    {
        printf("\nMutex initialization failed\n");
        return 1;
    }
   /* Creation du thread du magasin. */
	printf ("Creation du thread du magasin !\n");
	ret = pthread_create(&store.thread_store, NULL, fn_store, NULL);
 
   /* Creation des threads des clients si celui du magasin a reussi. */
	if (!ret)
	{
      	printf ("Creation des threads clients !\n");
		while (i < NB_CLIENTS)
		{
			ret = pthread_create( &store.thread_clients[i], NULL, fn_clients, (void *)(intptr_t)i);
        	if (ret)
        		fprintf (stderr, "%s", strerror (ret));
			i++;
    	}
	}
	else
		fprintf (stderr, "%s", strerror (ret));

   /* Attente de la fin des threads. */
	i = 0;
	while (i < NB_CLIENTS)
	{
		pthread_join(store.thread_clients[i], NULL);
		i++;
	}
	pthread_join(store.thread_store, NULL);

	//add
    pthread_mutex_destroy(&store.mutex); // Destruction du mutex après usage
	return (EXIT_SUCCESS);
}