/*
 *  Parallel program with no parallelism
 *  (pthread_join just after pthread_create)
 *
 */

#include <stdlib.h>
#include <stdio.h>
#include <assert.h>
#include <pthread.h>

#define MAXPATH 200

struct _threadargs {
 char filein[MAXPATH];
 char fileout[MAXPATH];
};

typedef struct _threadargs Threadargs;

int *read_file (char *filename, int *size) {
  int *vett;
  int s,v,i=0; 
  FILE *fp;

  fp=fopen(filename,"r");
  assert(fp!=NULL);  
  fscanf(fp,"%d",&s);
  vett=(int *)malloc(s*sizeof(int));
  while(fscanf(fp,"%d",&v)>0) {
    vett[i]=v;
    i++;
  }  
  *size=s; 
  fclose(fp);
  return vett; 
}

void sort (int *vett, int size) {
  int i,j;

  for(i=0;i<size;i++) {
    for(j=0;j<i;j++) {
      if(vett[i]<vett[j]) {
        int temp=vett[i]; //swap 
        vett[i]=vett[j];
        vett[j]=temp;
      }
    }
  } 

  return;
}

void write_file (int *vett, int size, char *filename) {
  int i;
  FILE *fp;

  printf("write %s\n",filename);
  fp=fopen(filename,"w");
  for(i=0;i<size;i++) {
    fprintf(fp,"%d\n",vett[i]);
  }
  fclose(fp);

  return;
} 

void *tf (void *arg) {
  Threadargs *thargs;
  char *filename;
  char fileout[MAXPATH];
  int *vett=NULL, size;

  thargs=(Threadargs *)arg;  
  filename=thargs->filein; 
  vett=read_file(filename,&size);
  assert(vett!=NULL); 
  sort(vett,size);  
  write_file(vett,size,thargs->fileout);
  free(vett); 
  free(thargs);
       
  return;
}

void main (int argc, void **argv) {
  pthread_t threadID ;
  Threadargs *ta; 
  void *exitstatus;
  int i,nt; 

  nt=atoi(argv[3]);
  for(i=0;i<nt;i++) { 
    ta=(Threadargs *)malloc(sizeof(Threadargs));  
    sprintf(ta->filein,"%s",(char *)argv[1]);            
    sprintf(ta->fileout,"%s",(char *)argv[2]);
    sprintf(ta->filein,"%s%d",ta->filein,i+1);
    sprintf(ta->fileout,"%s%d",ta->fileout,i+1);                                
    pthread_create(&threadID, NULL, tf, (void*)(ta));

    /* Wrong position for JOIN !!! */
    pthread_join (threadID , &exitstatus );  
  }

  return;
}
