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
  for(int i = 0;i < 10;i++) printf("(p:%x)\t%d\n",pthread_self(),zonacritica[i]=rand()%9);
  signalS(1);//signal al consumidor
  pthread_exit(NULL);
}
void * consumidor(void *argv){
  waitS(1);//wait a si mismo
  for(int i = 0;i < 10;i++) printf("[c:%d]\t%d\n",pthread_self(),zonacritica[i]);
  signalS(0);//signal al consumidor
  pthread_exit(NULL);
}

int main(){
  pthread_t hilos[10];
  //Creacion hilo
  key_t llave;
  llave=ftok("/bin/ls",0);//llave para ligar a los semaforos
  srand(time(NULL));
  //1 creacion del semaforo
  //si lo busco el semaforo ya existe... debo destruir el semaforo porque si no ya queda inicializado con valores de antes...
  semaforo=semget(llave,2,IPC_CREAT | 0777);
  //se valida el semaforo en caso de que se cree con error
  if(semaforo==-1)
  {
    printf("Error en el semaforo\n");
    exit(0);
  }
  if((semctl(semaforo,0,SETVAL,1) || semctl(semaforo,1,SETVAL,0))==-1){
    printf("\nerrorcito al inicializar semaforo\n");
    exit(0);
  }
  //Creacion del hilo
  for(int i=0;i<5;i++)
    {
      pthread_create(&hilos[i*2],NULL,productor,NULL);
      pthread_create(&hilos[i*2+1],NULL,consumidor,NULL);
    }
  //esperar hilo
  for(int j=0;j<10;j++){
    pthread_join(hilos[j],NULL);
  }
  //matar al semaforo... pero si el programa trono antes desde consola hay que matarlo... o antes del exit(0)
  semctl(semaforo,0,IPC_RMID,0);
}
