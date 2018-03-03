
#include <sys/ipc.h>
#include <stdio.h>
#include <unistd.h>
#include <pthread.h>
#include <semaphore.h>
#define CHAIRS 4 /*为等候的顾客准备的椅子数*/
typedef sem_t semphore;
semphore customers;
semphore barbers;
semphore mutex; /*用于互斥*/// 1 indicates empty 
int waitting=0; //number of customer waiting.
int count=0;

void *barber(void *arg)
{
	while(1)
	{
		sem_wait(&customers);//call customer
		sem_wait(&mutex);
		printf("Cut hair\n");				
		waitting--;
		
		sem_post(&mutex);//release waiting

		sem_post(&barbers);
		sleep(3);
	}
}

void *customer(void *arg){
	while(1){
		sem_wait(&mutex);//go int linjiequ
		printf("\nCustomer %d comes:\n",count);
		if(waitting< CHAIRS)
		{
			count++;
			printf("%d customers is waitting... \n",waitting);
			waitting++;
			if(waitting==1) sem_wait(&barbers);//weakup the barber
			sem_post(&mutex);//release waiting

			sem_post(&customers);	
			printf("Please sit\n");
		}
		else {
			printf("Customer %d leave\n",count);
			count++;
			sem_post(&mutex);//no seat, leave the barbershop
		}
		sleep(1);
	}	
	
}
int main(int argc, char const *argv[]){
	sem_init(&customers,0,0);
	sem_init(&barbers,0,1);
	sem_init(&mutex,0,1);

	pthread_t Thread_B;
	pthread_t Thread_C;
	
	if (pthread_create(&Thread_B,NULL,barber,NULL)){
		printf("Barber failed\n");
	}
	if (pthread_create(&Thread_C,NULL,customer,NULL)){
		printf("Customer failed\n");
	}
	pthread_join(Thread_B,NULL);
	pthread_join(Thread_C,NULL);
	
	
	sem_destroy(&customers);
	sem_destroy(&barbers);
	sem_destroy(&mutex);
}

