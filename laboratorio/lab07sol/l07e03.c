#include <stdlib.h>
#include <stdio.h>
#include <assert.h>
#include <pthread.h>

#define MAXPATH 200
#define MAXSIZE 1000

int **m_global; 
int *thflags_global; 

struct _threadargs  {
 int id;
 char filein[MAXPATH];
 char fileout[MAXPATH];
};

typedef struct _threadargs Threadargs;

int *read_file(char *filename, int *size) {
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

void sort(int *vett, int size) {
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
}

void write_file(int *vett,int size,char *filename) {
   int i;
   FILE *fp;
   printf("write %s\n",filename);
   fp=fopen(filename,"w");
   for(i=0;i<size;i++) {
     fprintf(fp,"%d\n",vett[i]);
   }
   fclose(fp);
} 

void *tf(void *arg){
  int i; 
  Threadargs *thargs;
  thargs=(Threadargs *)arg;  
  char *filename;
  filename=thargs->filein; 
  char fileout[200];
  int *vett=NULL, size;
  vett=read_file(filename,&size);
  assert(vett!=NULL); 
  sort(vett,size);  
  //write_file(vett,size,thargs->fileout);
  //copy vect on global matrix
         
  m_global[thargs->id][0]=size; 
  for(i=1;i<=size;i++) {
    m_global[thargs->id][i]=vett[i-1]; 
  } 
  thflags_global[thargs->id]=1;

  free(vett); 
  free(thargs);       
  pthread_exit((int *)NULL);
  //return NULL;
}

int merge (
  int *v1,int *v2,int *vmerge,int size_v1, int size_v2
  )
{ 
  int size_vmerge;
  int i,j,k;
  i=j=k=0; 

  while (i<size_v1 && j<size_v2) { 
    if (v1[i]<v2[j])
	vmerge[k++]=v1[i++];
    else
	vmerge[k++]=v2[j++];
   } 
   while (i<size_v1) {   
	vmerge[k++]=v1[i++];
   }
   while (j<size_v2) {
     vmerge[k++]=v2[j++];
   }
   size_vmerge=size_v1+size_v2; 
   return size_vmerge;  
}

void print(int *vett,int size) {
  int i; 
  for(i=0;i<size;i++)
	printf("%d ",vett[i]);
  printf("\n");
}

void main (int argc,void **argv) {
  pthread_t *threadID ;
  Threadargs *ta; 
  void *exitstatus;
  int i,nt,m,indexr,indexc,index; 

  nt=atoi(argv[1]);
  threadID=(pthread_t *)malloc(nt*sizeof(pthread_t));   

  //alloc flags
  thflags_global=(int *)malloc(nt*sizeof(int));    
      
  //init flags (we can also use memset)        
  for(m=0;m<nt;m++) {
    thflags_global[m]=0;
  }              

  //alloc m_global
  printf("alloc m_global\n"); 
  m_global = (int**)malloc(nt * sizeof(int*));
  for (index=0;index<nt;index++) 	{
    m_global[index] = (int*)malloc(MAXSIZE * sizeof(int));
  }
        
  //init m_global  
  printf("init m_global\n"); 
  for (indexr=0;indexr<nt;indexr++) {
    for (indexc=0;indexc<MAXSIZE;indexc++) {
      m_global[indexr][indexc] =0;
    }
  } 

  for(i=0;i<nt;i++) { 
    printf("creating th %d\n",i); 
    ta=(Threadargs *)malloc(sizeof(Threadargs));  
    ta->id=i; 
    sprintf(ta->filein,"%s",(char *)argv[2]);            
    sprintf(ta->fileout,"%s",(char *)argv[3]);
    sprintf(ta->filein,"%s%d",ta->filein,i+1);
    sprintf(ta->fileout,"%s%d",ta->fileout,i+1);                                
    pthread_create(&threadID[i],NULL,tf,(void*)(ta));	            
  }

  int ndone=0;
  int t=0;
  int *vmerge;
  int size_merge=0;

  //alloc vmerge
  vmerge=(int *)malloc((MAXSIZE*nt)*sizeof(int));    
 
  //init vmerge
  for(m=0;m<(MAXSIZE*nt);m++) {
    vmerge[m]=0;
  }                
          
  printf("Start polling: nt=%d\n",nt); 
  while(ndone<nt) {
    if (thflags_global[t]==1) {
      thflags_global[t]=0; 
      int *vt=(int *)(m_global[t]+1);
      int vt_size=m_global[t][0];
      int *vtmp;
      int vtmp_size=size_merge; 
      //malloc vtmp
      vtmp=(int *)malloc((MAXSIZE*nt)*sizeof(int));    
      //init - vtmp copy vmerge in vtmp (we can also use memcopy)
      for(m=0;m<vtmp_size;m++) {
        vtmp[m]=vmerge[m];
      }               

      //printf("Before merge : size vt %d, size vtmp %d, size vmerge %d\n",vt_size,vtmp_size,size_merge);  
      size_merge=merge(vt,vtmp,vmerge,vt_size,vtmp_size); 
      //printf("After merge : size vt %d, size vtmp %d, size vmerge %d\n",vt_size,vtmp_size,size_merge);  
      free(vtmp);
      printf("step %d : vett merge= ",ndone);  
      print(vmerge,size_merge); 
      ndone++; 
    }  
    t++;
    t=t%nt; //if t > nt -> restart from nt
    sleep(1); 
  }

  write_file(vmerge,size_merge,"finaloutput.txt"); 

  //release memory
  free(vmerge);  
  for (index=0;index<nt;++index) {
     free(m_global[index]);	
  }   
  free(m_global);         
  free(threadID);
  return;
}

