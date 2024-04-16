

#include <stdio.h> 
#include <stdlib.h> 
#include <unistd.h>  //Header file for sleep(). man 3 sleep for details. 
#include <pthread.h> 
  
// A normal C function that is executed as a thread  
// when its name is specified in pthread_create() 
void *myThreadFun(void *vargp) 
{ 
    sleep(1); 
    printf("Printing GeeksQuiz from Thread \n"); 
    return NULL; 
} 
   
int main() 
{ 
	//Declare pthread
    pthread_t thread_id; 

    printf("Before Thread\n"); 
    
	//Create pthread (4 args)
	//1st arg -> pointer to thread_id which is set by this function. 
	//2nd arg -> specifies attributes. If the value is NULL, then default attributes shall be used. 
	//3rd arg -> name of function to be executed for the thread to be created. 
	//4th arg -> used to pass arguments to the function, myThreadFun. 
	pthread_create(&thread_id, NULL, myThreadFun, NULL); 

	//The pthread_join() function for threads is the equivalent of wait() for processes. 
	//A call to pthread_join blocks the calling thread until the thread with identifier equal 
	//to the first argument terminates. 
    pthread_join(thread_id, NULL); 
    printf("After Thread\n"); 
    exit(0); 
}

