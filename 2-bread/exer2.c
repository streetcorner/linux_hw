
#include <sys/ipc.h>
#include <stdio.h>
#include <unistd.h>
#include <pthread.h>
#include <semaphore.h>

#define NUM_OF_BAKERS 3
typedef sem_t semphore;
semphore customers;
semphore bakers;
semphore mutex; /*用于互斥*/// 1 indicates empty 
int waitting=0; //number of customer waiting.
int count=0;

void *baker(void *arg)
{
	while(1)
	{
		sem_wait(&bakers);
		sem_wait(&customers);//call customer
		sem_wait(&mutex);	
		printf("Give bread or cake\n");				
		waitting--;	
		sem_post(&mutex);
		sem_post(&bakers);
		sleep(6);
		
	}
}

void *customer(void *arg){
	while(1){
		sem_wait(&mutex);//go int linjiequ
		printf("\ncustomer %d comes:\n",count);
		printf("%d customers is waitting...\n",waitting);
		count++;
		waitting++;	
		sem_post(&customers);	
		sem_post(&mutex);//release waiting
		//printf("Wait\n");

		sleep(1);
	}	
	

}
int main(int argc, char const *argv[]){
	sem_init(&customers,0,0);
	sem_init(&bakers,0,3);
	sem_init(&mutex,0,1);

	pthread_t Thread_B[NUM_OF_BAKERS];
	pthread_t Thread_C;
	
	for(int i=0; i< NUM_OF_BAKERS; i++){
		if (pthread_create(&Thread_B[i],NULL,baker,NULL)){
			printf("baker failed\n");
		}
	}
	if (pthread_create(&Thread_C,NULL,customer,NULL)){
		printf("customer failed\n");
	}
	
	for(int i=0; i< NUM_OF_BAKERS; i++){
		pthread_join(Thread_B[i],NULL);
	}
	pthread_join(Thread_C,NULL);
	
	
	sem_destroy(&customers);
	sem_destroy(&bakers);
	sem_destroy(&mutex);
}

