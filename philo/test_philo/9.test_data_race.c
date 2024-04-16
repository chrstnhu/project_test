/*#include <stdio.h>

#include <pthread.h>
#include <stdlib.h>
#define BIG 1000000000UL

struct s_utils
{
 unsigned long        *a;
 pthread_mutex_t *a_lock;
};

void *change_a(void *arg)
{
	struct s_utils *utils;

	utils = (struct s_utils *)arg;
	pthread_mutex_lock(utils->a_lock);
	for(unsigned long i = 0; i < BIG; i++)
		(*(utils->a))++;
	pthread_mutex_unlock(utils->a_lock);
	return (NULL);
}

int main()
{
	pthread_t       thread_1;
	pthread_t       thread_2;
	struct s_utils  utils;
	unsigned long        a;
	pthread_mutex_t a_lock;

	pthread_mutex_init(&a_lock, NULL);
	utils = (struct s_utils){&a, &a_lock};
	pthread_create(&thread_1, NULL, change_a, &utils);
	pthread_create(&thread_2, NULL, change_a, &utils);
	pthread_join(thread_1, NULL);
	pthread_join(thread_2, NULL);
	printf("a is: %lu\n", a);
}
*/



/*
cc 9.test_data_race.c
➜  test_philo ./a.out              
Before: 0
After: 300

ou 

➜  test_philo cc 9.test_data_race.c
➜  test_philo ./a.out              
Before: 0
After: 200

EX :
Thread #1             Thread #2              Bank Balance

Read Balance  <----------------------------------- 0
balance = 0
                      Read Balance  <------------- 0
                      balance = 0

Deposit +300
balance = 300
                      Deposit +200
                      balance = 200

Write Balance  ----------------------------------> 300
balance = 300
                      Write Balance  ------------> 200
                      balance = 200
*/
#include <unistd.h>
#include <stdio.h>
#include <pthread.h>

// the initial balance is 0
int balance = 0;

// write the new balance (after as simulated 1/4 second delay)
void write_balance(int new_balance)
{
  usleep(250000);
  balance = new_balance;
}

// returns the balance (after a simulated 1/4 seond delay)
int read_balance()
{
  usleep(250000);
  return balance;
}

// carry out a deposit
void* deposit(void *amount)
{
  // retrieve the bank balance
  int account_balance = read_balance();

  // make the update locally
  account_balance += *((int *) amount);

  // write the new bank balance
  write_balance(account_balance);

  return NULL;
}

int main()
{
  // output the balance before the deposits
  int before = read_balance();
  printf("Before: %d\n", before);

  // we'll create two threads to conduct a deposit using the deposit function
  pthread_t thread1;
  pthread_t thread2;

  // the deposit amounts... the correct total afterwards should be 500
  int deposit1 = 300;
  int deposit2 = 200;

  // create threads to run the deposit function with these deposit amounts
  pthread_create(&thread1, NULL, deposit, (void*) &deposit1);
  pthread_create(&thread2, NULL, deposit, (void*) &deposit2);

  // join the threads
  pthread_join(thread1, NULL);
  pthread_join(thread2, NULL);

  // output the balance after the deposits
  int after = read_balance();
  printf("After: %d\n", after);

  return 0;
}