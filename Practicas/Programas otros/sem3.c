#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <pthread.h>
#include <time.h>
#include <sys/types.h>
#include <fcntl.h>
#include <semaphore.h>
#include <sys/ipc.h>
#define ZN 2
#define ZS 6

char ** zcrit;
sem_t * sem[3];
//0: Producer 1: Consumer 2: Critical Zone

void * producer();
void * consumer();

int main(int argc, char * argv[])
{
  if(argc!=2||atoi(argv[1])%2) return 0;
  int thread_num = atoi(argv[1]),index[atoi(argv[1])/2];

  for(int i=0;i<ZS;i++)index[i]=i;
  
  zcrit = malloc(ZN*sizeof(char *));
  for(int i=0;i<ZN;i++)zcrit[i]=malloc(ZS*sizeof(char));
  
  for(int i=0;i<ZN;i++)zcrit[i][0]='v';
  
  pthread_t hilos[thread_num];
  
  sem[0] = sem_open("semP",O_CREAT,00777,2);
  sem[1] = sem_open("semC",O_CREAT,00777,0);
  sem[2] = sem_open("semZ",O_CREAT,00777,1);
  
  if(sem[0]==SEM_FAILED||sem[1]==SEM_FAILED||sem[2]==SEM_FAILED)
    {printf("\nError\n");exit(0);}
  
  for(int i=0;i<thread_num/2;i++)
    {
      pthread_create(&hilos[i*2],NULL,producer,(void *)&index[i]);
      pthread_create(&hilos[i*2+1],NULL,consumer,NULL);
    }
  
  for(int j=0;j<thread_num;j++) pthread_join(hilos[j],NULL);

  for(int i=0;i<3;i++)sem_close(sem[i]);
  sem_unlink("semP");sem_unlink("semC");sem_unlink("semZ");
  
  return 0;
}
void * producer(void * tmp)
{
  int * index = (int *)tmp;
  char flg = 'n';
  for(int j = 0;j < 5;j++)
    {
      sem_wait(sem[0]);sem_wait(sem[2]);

      for(int i=0;i<ZN;i++)
	{
	  flg='n';
	  if(zcrit[i][0]=='v')
	    {
	      flg = 'y';
	      zcrit[i][0]='l';
	      sem_post(sem[2]);
	      switch(*index)
		{
		case 0:for(int k=1;k<ZS;k++)zcrit[i][k]='x';break;
		case 1:for(int k=1;k<ZS;k++)zcrit[i][k]='y';break;
		case 2:for(int k=1;k<ZS;k++)zcrit[i][k]='z';break;
		}
	      zcrit[i][0]='d';sem_post(sem[1]);
	      break;
	    }
	  if(i==ZN-1&&flg=='n'){sem_post(sem[2]);j--;}
	}}pthread_exit(NULL);
}

void * consumer()
{
  int flg = 'n';
  for(int i = 0;i<5;i++)
    {
      sem_wait(sem[1]);sem_wait(sem[2]);
      for(int j=0;j<ZN;j++)
	{
	  flg='n';
	  if(zcrit[j][0]=='d')
	    {
	      flg='y';
	      zcrit[j][0]='r';
	      sem_post(sem[2]);
	      printf("Consumer No. %d: \t%c%c%c%c%c\n",pthread_self(),
		     zcrit[j][1],zcrit[j][2],zcrit[j][3],zcrit[j][4],zcrit[j][5]);
	      zcrit[j][0]='v';
	      sem_post(sem[0]);
	      break;
	    }
	  if(j==ZN-1&&flg=='n'){sem_post(sem[2]);i--;}
	}
    }
  pthread_exit(NULL);
}
