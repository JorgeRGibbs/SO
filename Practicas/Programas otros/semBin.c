#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <pthread.h>
#include <time.h>
#include <sys/types.h>
#include <sys/sem.h>
#include <sys/ipc.h>

int zonacritica[10];
int semaforo;
struct sembuf operacion; //la estructura ya existe, solo hay que declararla

void waitS(int s){
  operacion.sem_num=s;
  operacion.sem_op=-1;
  operacion.sem_flg=SEM_UNDO;
  semop(semaforo,&operacion,1);
}
void signalS(int s){
  operacion.sem_num=s;
  operacion.sem_op=1;
  operacion.sem_flg=SEM_UNDO;
  semop(semaforo,&operacion,1);
}

void * productor(void *argv){
  waitS(0);//wait a si mismo
  for(int i = 0;i < 10;i++)zonacritica[i]=rand()%9;
  signalS(0);//signal al consumidor
  pthread_exit(NULL);
}
void * consumidor(void *argv){
  waitS(0);
  for(int i = 0;i < 10;i++) printf("[c:%d]\t%d\n",pthread_self(),zonacritica[i]);
  signalS(0);
  pthread_exit(NULL);
}

int main(){
  pthread_t hilos[2];

  key_t llave;
  llave=ftok("/bin/ls",0);
  srand(time(NULL));
  semaforo=semget(llave,1,IPC_CREAT | 0777);
  if(semaforo==-1)
  {
    printf("Error en el semaforo\n");
    exit(0);
  }
  if(semctl(semaforo,0,SETVAL,1)==-1){
    printf("\nError al inicializar semaforo\n");
    exit(0);
  }
  pthread_create(&hilos[0],NULL,productor,NULL);
  pthread_join(hilos[0],NULL);
  pthread_create(&hilos[1],NULL,consumidor,NULL);
  pthread_join(hilos[1],NULL);
  semctl(semaforo,0,IPC_RMID,0);
}
