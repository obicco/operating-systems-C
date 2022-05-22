#include<stdio.h>
#include<unistd.h>
#include<assert.h>
#include<stdlib.h>
#include <sys/types.h>
#include <sys/wait.h>


typedef int* pipe_sem_t;

pipe_sem_t pipe_sem_create(int n);
void pipe_sem_destroy(pipe_sem_t sem);
void pipe_sem_wait(pipe_sem_t sem);
void pipe_sem_signal(pipe_sem_t sem);
void spawn(void (*f)(void));
void process_A();
void process_B();
void process_C();
void process_D();
void process_E();
void process_F();
void process_G();
void process_H();
void process_I();

pipe_sem_t semA, semBCD, semEF, semG, semH, semI;

pipe_sem_t pipe_sem_create(int n){
  int *fds;
  int i;
  fds = malloc(2*sizeof(int));
  pipe(fds);
  
  for(i=0; i<n; i++) pipe_sem_signal((pipe_sem_t) fds);
  return (pipe_sem_t) fds;
}

void pipe_sem_destroy(pipe_sem_t sem){
  close(sem[0]);
  close(sem[1]);
  free(sem);
}

void pipe_sem_wait(pipe_sem_t sem){
  char token;
  read(sem[0], (void*) &token, 1);
}

void pipe_sem_signal(pipe_sem_t sem){
  char token = '*';
  write(sem[1], (void*) &token, 1);
}

void spawn(void (*f)(void)){
  pid_t pid;
  pid = fork();
  if(pid == 0){
    f();
    exit(0);
  }
}

int main(int argc, char **argv){

  semA = pipe_sem_create(1);
  semBCD = pipe_sem_create(0);
  semEF = pipe_sem_create(0);
  semG = pipe_sem_create(0);
  semH = pipe_sem_create(0);
  semI = pipe_sem_create(0);

  while(1){
    
    spawn(process_A);
    spawn(process_B);
    spawn(process_C);
    spawn(process_D);
    spawn(process_E);
    spawn(process_F);
    spawn(process_G);
    spawn(process_H);
    
    process_I();
  }
  
  pipe_sem_destroy(semA);
  pipe_sem_destroy(semBCD);
  pipe_sem_destroy(semEF);
  pipe_sem_destroy(semG);
  pipe_sem_destroy(semH);
  pipe_sem_destroy(semI);
  
  return 0;
}

void process_A(){
  int i;
  pipe_sem_wait(semA);
  printf("A\n");
  sleep(2);
  for(i=0; i<3; i++) pipe_sem_signal(semBCD);
}

void process_B(){
  pipe_sem_wait(semBCD);
  printf("B\n");
  sleep(2);
  pipe_sem_signal(semI);
}

void process_C(){
  int i;
  pipe_sem_wait(semBCD);
  printf("C\n");
  sleep(2);
  for(i=0; i<2; i++) pipe_sem_signal(semEF);
}

void process_D(){
  pipe_sem_wait(semBCD);
  printf("D\n");
  sleep(2);
  pipe_sem_signal(semH);
}

void process_E(){
  pipe_sem_wait(semEF);
  printf("E\n");
  sleep(2);
  pipe_sem_signal(semG);
}

void process_F(){
  pipe_sem_wait(semEF);
  printf("F\n");
  sleep(2);
  pipe_sem_signal(semG);
}

void process_G(){
  pipe_sem_wait(semG);
  pipe_sem_wait(semG);
  printf("G\n");
  sleep(2);
  pipe_sem_signal(semI);
}

void process_H(){
  pipe_sem_wait(semH);
  printf("H\n");
  sleep(2);
  pipe_sem_signal(semI);
}

void process_I(){
  pipe_sem_wait(semI);
  pipe_sem_wait(semI);
  pipe_sem_wait(semI);
  printf("I\n");
  sleep(2);
  pipe_sem_signal(semA);
}



