#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <pthread.h>
#include <time.h>
#include <sys/types.h>
#include <sys/sem.h>
#include <sys/ipc.h>

char * zonacritica;
int semaforo;
struct sembuf op;

void sendS(int,char);
void * productor();
void * consumidor();

int main(int argc, char * argv[])
{
  int thread_num = atoi("5");
  zonacritica = malloc(5*sizeof(char));

  //if(!thread_num%2 || argc!=2) return 0;

  pthread_t hilos[thread_num];
  key_t llave = ftok("/bin/ls",0);

  semaforo=semget(llave,2,IPC_CREAT | 0777);
  if(semaforo==-1){puts("Error en arreglo\n");semctl(semaforo,0,IPC_RMID,0); exit(0);}

  if((semctl(semaforo,0,SETVAL,1) || semctl(semaforo,1,SETVAL,0))==-1)
    {printf("\nError\n");semctl(semaforo,0,IPC_RMID,0);exit(0);}

  for(int i=0;i<thread_num/2;i++)
    {
      pthread_create(&hilos[i*2],NULL,productor,NULL);
      pthread_create(&hilos[i*2+1],NULL,consumidor,NULL);
    }
  for(int j=0;j<thread_num;j++) pthread_join(hilos[j],NULL);

  semctl(semaforo,0,IPC_RMID,0);

  return 0;
}

void sendS(int s,char type)
{
  op.sem_num=s;
  if(type=='w')op.sem_op=-1;else if(type=='s')op.sem_op=1;
  op.sem_flg=SEM_UNDO;
  semop(semaforo,&op,1);
}

void * productor()
{
  for(int j = 0;j < 2;j++)
    {
      sendS(0,'w');
      for(int i = 0;i < 10; i++)
	{
	  if(i==0)puts("~*~");
	  zonacritica[i] = rand()%48+48;
	  printf("(P) · %lu\t%c\n",pthread_self(),zonacritica[i]);
	  if(i==9)puts("~+~");
	}
      sendS(1,'s');
    }
  pthread_exit(NULL);
}

void * consumidor()
{
  for(int i = 0;i<2;i++)
    {
      sendS(1,'w');
      for(int j=0;j<10;j++)
	{
	  if(j==0)puts("¬*¬");
	  printf("[C] · %lu\t%c\n",pthread_self(),zonacritica[j]);
	  if(j==9)puts("¬+¬");
	}
      sendS(0,'s');
    }
  pthread_exit(NULL);
}
