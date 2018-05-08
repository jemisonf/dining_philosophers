#include <pthread.h>
#include <string.h>
#include <stdio.h>
#include <stdlib.h>
#include <semaphore.h>
#include <time.h>
#include <unistd.h>

struct philosopher {
	char name[256];
	int index;
};

sem_t forks[5];
sem_t max_forks;
struct philosopher philosophers[5];
pthread_t philosopher_threads[5];

int right(int i) {
	return (i + 1) % 5;
}

int left(int i) {
	return i;
}

void * philosopher_thread(void* args) {
	struct philosopher* self = (struct philosopher *) args;
	int randnum;
	fflush(stdout);
	while(1) {
		randnum = (rand() % 20) + 1;
		printf("%s is thinking for %d seconds\n", self->name, randnum);
		sleep(randnum);
#ifdef VERBOSE
		printf("%s is done thinking\n", self->name);
#endif
		sem_wait(&max_forks);
#ifdef VERBOSE
		printf("%s is picking up fork %d\n", self->name, right(self->index));
#endif
		sem_wait(&forks[right(self->index)]);
#ifdef VERBOSE
		printf("%s has picked up fork %d and is picking up fork %d\n", self->name, right(self->index), left(self->index));
#endif
		sem_wait(&forks[left(self->index)]);
		randnum = (rand() % 8) + 2;
		printf("%s has picked up both forks and is eating for %d seconds\n", self->name, randnum);
		sem_post(&forks[right(self->index)]);
		sem_post(&forks[left(self->index)]);
		sem_post(&max_forks);
		sleep(randnum);

	}
	return 0;
}

int main(int arc, char* arv[]) {
	char * names[5] = {"Locke", "Rousseau", "Hobbes",  "Kanye", "Marx"};
	int i;
	srand(time(NULL));
	sem_init(&max_forks, 0, 4);	
#ifndef VERBOSE
	printf("Compile with 'make verbose' to see more information about philosopher actions\n");
#endif
	for (i = 0; i < 4; i++) {
		sem_init(&forks[i], 0, 1);
		strcpy(philosophers[i].name, names[i]);	
		philosophers[i].index = i;
		pthread_create(&(philosopher_threads[i]), NULL, &philosopher_thread, (void *) &(philosophers[i]));
	}
	i = 4;
	sem_init(&forks[i], 0, 1);
	strcpy(philosophers[i].name, names[i]);	
	philosophers[i].index = i;
	philosopher_thread(&philosophers[i]);
}
