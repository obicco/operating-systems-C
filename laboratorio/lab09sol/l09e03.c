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

sem_t semA, semBCD, semEF, semG, semH, semI;
pthread_t tidA, tidB, tidC, tidD, tidE, tidF, tidG, tidH, tidI;

int main(int argc, char **argv){

  sem_init(&semA, 0, 1);
  sem_init(&semBCD, 0, 0);
  sem_init(&semEF, 0, 0);
  sem_init(&semG, 0, 0);
  sem_init(&semH, 0, 0);
  sem_init(&semI, 0, 0);

  while(1){
    
    pthread_create(&tidA, NULL, process_A, NULL);
    pthread_create(&tidB, NULL, process_B, NULL);
    pthread_create(&tidC, NULL, process_C, NULL);
    pthread_create(&tidD, NULL, process_D, NULL);
    pthread_create(&tidE, NULL, process_E, NULL);
    pthread_create(&tidF, NULL, process_F, NULL);
    pthread_create(&tidG, NULL, process_G, NULL);
    pthread_create(&tidH, NULL, process_H, NULL);
    
    process_I(NULL);
  }
  
  sem_destroy(&semA);
  sem_destroy(&semBCD);
  sem_destroy(&semEF);
  sem_destroy(&semG);
  sem_destroy(&semH);
  sem_destroy(&semI);
  
  return 0;
}

void* process_A(void* arg){
  int i;
  sem_wait(&semA);
  printf("A\n");
  sleep(2);
  for(i=0; i<3; i++) sem_post(&semBCD);
  return NULL;
}

void* process_B(void* arg){
  sem_wait(&semBCD);
  printf("B\n");
  sleep(2);
  sem_post(&semI);
  return NULL;
}

void* process_C(void* arg){
  int i;
  sem_wait(&semBCD);
  printf("C\n");
  sleep(2);
  for(i=0; i<2; i++) sem_post(&semEF);
  return NULL;
}

void* process_D(void* arg){
  sem_wait(&semBCD);
  printf("D\n");
  sleep(2);
  sem_post(&semH);
  return NULL;
}

void* process_E(void* arg){
  sem_wait(&semEF);
  printf("E\n");
  sleep(2);
  sem_post(&semG);
  return NULL;
}

void* process_F(void* arg){
  sem_wait(&semEF);
  printf("F\n");
  sleep(2);
  sem_post(&semG);
  return NULL;
}

void* process_G(void* arg){
  sem_wait(&semG);
  sem_wait(&semG);
  printf("G\n");
  sleep(2);
  sem_post(&semI);
  return NULL;
}

void* process_H(void* arg){
  sem_wait(&semH);
  printf("H\n");
  sleep(2);
  sem_post(&semI);
  return NULL;
}

void* process_I(void* arg){
  sem_wait(&semI);
  sem_wait(&semI);
  sem_wait(&semI);
  printf("I\n");
  sleep(2);
  sem_post(&semA);
  return NULL;
}



