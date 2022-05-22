#include<stdio.h>
#include<unistd.h>
#include<assert.h>
#include<stdlib.h>
#include<pthread.h>
#include<semaphore.h>

void* process_A(void *arg);
void* process_B(void *arg);
void* process_C(void *arg);
void* process_D(void *arg);
void* process_E(void *arg);
void* process_F(void *arg);
void* process_G(void *arg);
void* process_H(void *arg);
void* process_I(void *arg);

sem_t semA, semB, semC, semD, semE, semF, semG, semH, semI;
pthread_t tidA, tidB, tidC, tidD, tidE, tidF, tidG, tidH, tidI;

int main(int argc, char **argv){

  sem_init(&semA, 0, 1);
  sem_init(&semB, 0, 0);
  sem_init(&semC, 0, 0);
  sem_init(&semD, 0, 0);
  sem_init(&semE, 0, 0);
  sem_init(&semF, 0, 0);
  sem_init(&semG, 0, 0);
  sem_init(&semH, 0, 0);
  sem_init(&semI, 0, 0);

  pthread_create(&tidA, NULL, process_A, NULL);
  pthread_create(&tidB, NULL, process_B, NULL);
  pthread_create(&tidC, NULL, process_C, NULL);
  pthread_create(&tidD, NULL, process_D, NULL);
  pthread_create(&tidE, NULL, process_E, NULL);
  pthread_create(&tidF, NULL, process_F, NULL);
  pthread_create(&tidG, NULL, process_G, NULL);
  pthread_create(&tidH, NULL, process_H, NULL);
  
  process_I(NULL);
  
  sem_destroy(&semA);
  sem_destroy(&semB);
  sem_destroy(&semC);
  sem_destroy(&semD);
  sem_destroy(&semE);
  sem_destroy(&semF);
  sem_destroy(&semG);
  sem_destroy(&semH);
  sem_destroy(&semI);
  
  return 0;
}

void* process_A(void* arg){
  while(1){
    sem_wait(&semA);
    printf("A\n");
    sleep(2);
    sem_post(&semB);
    sem_post(&semC);
    sem_post(&semD);
  }
}

void* process_B(void* arg){
  while(1){
    sem_wait(&semB);
    printf("B\n");
    sleep(2);
    sem_post(&semI);
  }
}

void* process_C(void* arg){
  while(1){
    sem_wait(&semC);
    printf("C\n");
    sleep(2);
    sem_post(&semE);
    sem_post(&semF);
  }
}

void* process_D(void* arg){
  while(1){
    sem_wait(&semD);
    printf("D\n");
    sleep(2);
    sem_post(&semH);
  }
}

void* process_E(void* arg){
  while(1){
    sem_wait(&semE);
    printf("E\n");
    sleep(2);
    sem_post(&semG);
  }
}

void* process_F(void* arg){
  while(1){
    sem_wait(&semF);
    printf("F\n");
    sleep(2);
    sem_post(&semG);
  }
}

void* process_G(void* arg){
  while(1){
    sem_wait(&semG);
    sem_wait(&semG);
    printf("G\n");
    sleep(2);
    sem_post(&semI);
  }
}

void* process_H(void* arg){
  while(1){
    sem_wait(&semH);
    printf("H\n");
    sleep(2);
    sem_post(&semI);
  }
}

void* process_I(void* arg){
  while(1){
    sem_wait(&semI);
    sem_wait(&semI);
    sem_wait(&semI);
    printf("I\n");
    sleep(2);
    sem_post(&semA);
  }
}



