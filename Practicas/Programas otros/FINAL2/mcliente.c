#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <pthread.h>
#include <sys/types.h>
#include <sys/ipc.h>
#include <fcntl.h>           /* For O_* constants */
#include <sys/stat.h>        /* For mode constants */
#include <semaphore.h>
#include <sys/mman.h>
#include <sys/socket.h>
#include <arpa/inet.h>
#include <netdb.h>

typedef struct{
  char zonacritica[4][3];
}zonacritica;
zonacritica * zonita;
int m,n,shm_fd,socket_id;
struct sockaddr_in servidor;

sem_t * sem1;
sem_t * sem2;
sem_t * sem3;
sem_t * sem4;
sem_t * sem5;
sem_t * sem6;
sem_t * sem7;
sem_t * sem8;

void * consumidor(void *arg){
  int *id=(int *)arg; //recasteo el argumento...
  char buffer[5];
  int valor;
  for(int i=0;i<m;i++){
    for(int j=0; j<3; j++){
    if(sem_getvalue(sem5,&valor)!=0) perror("error");
      else{
        if(valor>0){
          sem_wait(sem5);
          sprintf(buffer,"%c%c%c\0",zonita->zonacritica[0][0],zonita->zonacritica[0][1],zonita->zonacritica[0][2]);
          if(send(socket_id,(void *)buffer,strlen(buffer),0)==-1){perror("error send");}
          if(recv(socket_id,(void *)buffer,sizeof(buffer),0)==-1){perror("error recv");}
          sem_post(sem1);
          //if(!memcmp(buffer,"ok",2))break;
        }
        else{
          if(sem_getvalue(sem6,&valor)!=0) perror("error");
          else{
            if(valor>0){
              sem_wait(sem6);
              sprintf(buffer,"%c%c%c\0",zonita->zonacritica[1][0],zonita->zonacritica[1][1],zonita->zonacritica[1][2]);
              if(send(socket_id,(void *)buffer,strlen(buffer),0)==-1){perror("error send");}
              if(recv(socket_id,(void *)buffer,sizeof(buffer),0)==-1){perror("error recv");}
              sem_post(sem2);
              //if(!memcmp(buffer,"ok",2))break;
            }
            else{
              if(sem_getvalue(sem7,&valor)!=0) perror("error");
              else{
                if(valor>0){
                  sem_wait(sem7);
                  sprintf(buffer,"%c%c%c\0",zonita->zonacritica[2][0],zonita->zonacritica[2][1],zonita->zonacritica[2][2]);
                  if(send(socket_id,(void *)buffer,strlen(buffer),0)==-1){perror("error send");}
                  if(recv(socket_id,(void *)buffer,sizeof(buffer),0)==-1){perror("error recv");}
                  sem_post(sem3);
                  //if(!memcmp(buffer,"ok",2))break;
                }
                else{
                  if(sem_getvalue(sem8,&valor)!=0) perror("error");
                  else{
                    if(valor>0){
                      sem_wait(sem8);
                      sprintf(buffer,"%c%c%c\0",zonita->zonacritica[3][0],zonita->zonacritica[3][1],zonita->zonacritica[3][2]);
                      if(send(socket_id,(void *)buffer,strlen(buffer),0)==-1){perror("error send");}
                      if(recv(socket_id,(void *)buffer,sizeof(buffer),0)==-1){perror("error recv");}
                      sem_post(sem4);
                      //if(!memcmp(buffer,"ok",2))break;
                    }
                    else{
                      j--;}
                      }
                    }
                  }
                }
              }
            }
          }}}
}

int main(int argc, char *argv[]){
    n = atoi(argv[3]);
    m=(n*3)/2;

  //Creacion de memoria compartida
  shm_fd=shm_open("/espacioM",O_CREAT | O_RDWR, 00777);//abre un espacio de memoria compartido
  if(shm_fd==-1){perror("\nerror creando espacio de memoria compartido\n");}
  ftruncate(shm_fd,sizeof(zonacritica));
  zonita = mmap(0,sizeof(zonacritica),PROT_READ | PROT_WRITE,MAP_SHARED,shm_fd,0);
  if((void *)zonita==MAP_FAILED){perror("\nerror mapeando espacio de memoria compartido\n");}
  //Creacion del socket
  socket_id=socket(AF_INET,SOCK_STREAM,0);
  if(socket_id==-1)perror("errooor");
  memset((void *)&servidor,0,sizeof(servidor));
  servidor.sin_family=AF_INET;
  servidor.sin_port=htons(atoi(argv[1]));
  servidor.sin_addr.s_addr=inet_addr(argv[2]);
  if(connect(socket_id,(struct sockaddr *)&servidor,(socklen_t)sizeof(servidor))==-1)perror("errorcito");


  pthread_t hilos[2];//Creacion hilos

  sem1 = sem_open("sem1", O_CREAT,00777,1);
  sem2 = sem_open("sem2", O_CREAT,00777,1);
  sem3 = sem_open("sem3", O_CREAT,00777,1);
  sem4 = sem_open("sem4", O_CREAT,00777,1);
  sem5 = sem_open("sem5", O_CREAT,00777,0);
  sem6 = sem_open("sem6", O_CREAT,00777,0);
  sem7 = sem_open("sem7", O_CREAT,00777,0);
  sem8 = sem_open("sem8", O_CREAT,00777,0);

  if((sem1==SEM_FAILED)||(sem2==SEM_FAILED)||(sem3==SEM_FAILED)||(sem4==SEM_FAILED)||(sem5==SEM_FAILED)||(sem6==SEM_FAILED)||(sem7==SEM_FAILED)||(sem8==SEM_FAILED)){
	sem_close(sem1);sem_close(sem2);sem_close(sem3);sem_close(sem4);sem_close(sem5);sem_close(sem6);sem_close(sem7);sem_close(sem8);
	sem_unlink("sem1");sem_unlink("sem2");sem_unlink("sem3");sem_unlink("sem4");sem_unlink("sem5");sem_unlink("sem6");
	sem_unlink("sem7");sem_unlink("sem8");
	exit(0);
	}

  pthread_create(&hilos[0],NULL,consumidor,NULL);
  pthread_create(&hilos[1],NULL,consumidor,NULL);

  //esperar hilo
  for(int j=0;j<2;j++){
    pthread_join(hilos[j],NULL);
  }
  //asesinar al semaforo
  sem_close(sem1);sem_close(sem2);sem_close(sem3);sem_close(sem4);sem_close(sem5);sem_close(sem6);sem_close(sem7);sem_close(sem8);
	sem_unlink("sem1");sem_unlink("sem2");sem_unlink("sem3");sem_unlink("sem4");sem_unlink("sem5");sem_unlink("sem6");
	sem_unlink("sem7");sem_unlink("sem8");
  printf("\n");
  return 0;
}
