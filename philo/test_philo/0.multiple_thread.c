#include <stdio.h> 
#include <stdlib.h> 
#include <unistd.h> 
#include <pthread.h> 
  
// Let us create a global variable to change it in threads 
int g = 0;

// The function to be executed by all threads 
void	*myThreadFun(void *vargp)
{
    // Store the value argument passed to this thread 
	int	*myid = (int *)vargp;
    // Let us create a static variable to observe its changes 
	static int s;

	s = 0;
    // Change static and global variables 
	s++;
	g++;
    // Print the argument, static and global variables
	printf("Thread ID : %d, Static : %d, Global : %d\n", *myid, s++, g++);
	return (0);
}
  
int main()
{ 
    int i;
	pthread_t tid;
  
	i = 0;
    // Let us create three threads 
	while (i < 3)
	{
/*
									1                2                               3                     4
	int pthread_create (pthread_t * thread, pthread_attr_t * attr, void * (* start_routine) (void *), void * arg);


	1.Type opaque, sa valeur réelle dépend de l'implémentation (sur Linux il s'agit en générale du type unsigned long). 
	
	2.Ce type correspond à l'identifiant du thread qui sera créé, tout comme les processus ont leur propre identifiant.
    
	3.Le type pthread_attr_t est un autre type opaque permettant de définir des attributs spécifiques pour chaque thread.
	Il faut simplement savoir que l'on peut changer le comportement de la gestion des threads par exemple, 
	les régler pour qu'ils tournent sur un système temps réel ! 
	En général on se contente des attributs par défaut donc en mettant cet argument à NULL.
    
	4.Chaque thread dispose d'une fonction à exécuter, c'est en même temps sa raison de vivre… 
	Cet argument permet de transmettre un pointeur sur la fonction qu'il devra exécuter.
    Ce dernier argument représente un argument que l'on peut passer à la fonction que le thread doit exécuter.

*/
		pthread_create(&tid, NULL, myThreadFun, (void *)&tid);
		i++;
	}
	pthread_exit(NULL);
	return (0);
} 


// Please note that above is simple example to show how threads work.
// Accessing a global variable in a thread is generally a bad idea. 
// What if thread 2 has priority over thread 1 and thread 1 needs to change the variable. 
// In practice, if it is required to access global variable by multiple threads, 
// then they should be accessed using a mutex. 