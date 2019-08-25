#include <stdlib.h>
#include <pthread.h>
#include <unistd.h>
#include <stdio.h>

int SHOP[5] = {1000, 1000, 1000, 1000, 1000};
pthread_mutex_t mutex = PTHREAD_MUTEX_INITIALIZER;

#define handle_error(msg)\
 do { perror(msg); exit(EXIT_FAILURE); } while(0);

int rand_from_to(const int min, const int max);

void *customer(void *thread_data);

void *loader(void *thread_data);

 void main(){
 	int i, num;
 	pthread_t tid[4];
 	char name[3][8] = {"Masha", "Sasha", "Seregya"};

 	for(i = 0; i < 5; i++){
 		num = rand_from_to(0, 100);
 		SHOP[i] += num;
 	}

 	for(i = 0; i < 3; i++){
 		pthread_create(&(tid[i]), NULL, customer, name[i]);
 	}
 	pthread_create(&(tid[3]), NULL, loader, NULL);

 	pthread_join(tid[0], NULL);
 	pthread_join(tid[1], NULL);
 	pthread_join(tid[2], NULL);
 	pthread_cancel(tid[3]);
 	pthread_mutex_destroy(&mutex);
 	exit(EXIT_SUCCESS);
 }

 int rand_from_to(const int min, const int max) {
    return rand() % (max - min + 1) + min;
}

void *customer(void *thread_data){
	int i, money;
	char *name = (char *)thread_data;
	money = rand_from_to(0, 1000);
	money += 10000;

	while(money != 0){
		i = rand_from_to(0, 4);
		if (SHOP[i] == 0) continue;

		if ((money - SHOP[i]) >= 0){
			pthread_mutex_lock(&mutex);
			money -= SHOP[i];
			printf("\n----------------------------\n");
			printf("My name is %s i buy %d product and i have %d money", name, SHOP[i], money);
			printf("\n----------------------------\n");
			SHOP[i] = 0;
			pthread_mutex_unlock(&mutex);
		} else{
			pthread_mutex_lock(&mutex);
			SHOP[i] -= money;
			printf("\n----------------------------\n");
			printf("My name is %s i spent all my money", name);
			printf("\n----------------------------\n");
			money = 0;
			pthread_mutex_unlock(&mutex);
		}
		sleep(3);
	}

	pthread_exit(EXIT_SUCCESS);
}

void *loader(void *thread_data){
	int product = 500, i;

	while(1){
		pthread_mutex_lock(&mutex);
		i = rand_from_to(0, 4);
		SHOP[i] += product;
		printf("\n----------------------------\n");
		printf("I'm load shop number %d and it have %d product", i, SHOP[i]);
		printf("\n----------------------------\n");
		pthread_mutex_unlock(&mutex);
		sleep(2);
	}

	pthread_exit(EXIT_SUCCESS);
}