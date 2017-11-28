
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/types.h> 
#include <sys/socket.h>
#include <sys/stat.h>
#include <arpa/inet.h>
#include <netinet/in.h>
#include <netdb.h>
#include <string.h>
#include <pthread.h>
#include <semaphore.h>
#include <fcntl.h>

#define BACKLOG 4

sem_t * file_sem;

FILE * file_cntrl;

struct sockaddr_in server;
struct sockaddr_in client;

void dofile(FILE * file,char * path, char * flag, char * msg)
{
  file = fopen(path,flag);if(file==NULL){puts("Error with file");exit(0);}
  fprintf(file,"%c%c%c\n",msg[0],msg[1],msg[2]);
  fclose(file);
}

void archivito(char * msg)
{
  if(msg[0]=='A')
    {
      if(msg[1]=='M'&&msg[2]=='A'){dofile(file_cntrl,"./ANA/m_att.txt","a",msg);}
      else if(msg[1]=='M'&&msg[2]=='M'){dofile(file_cntrl,"./ANA/m_mov.txt","a",msg);}
      else if(msg[1]=='M'&&msg[2]=='T'){dofile(file_cntrl,"./ANA/m_tel.txt","a",msg);}
      else if(msg[1]=='L'&&msg[2]=='A'){dofile(file_cntrl,"./ANA/l_att.txt","a",msg);}
      else if(msg[1]=='L'&&msg[2]=='M'){dofile(file_cntrl,"./ANA/l_mov.txt","a",msg);}
      else if(msg[1]=='L'&&msg[2]=='T'){dofile(file_cntrl,"./ANA/l_tel.txt","a",msg);}}
  else if(msg[0]=='L')
    {
      if(msg[1]=='M'&&msg[2]=='A'){dofile(file_cntrl,"./LUIS/m_att.txt","a",msg);}
      else if(msg[1]=='M'&&msg[2]=='M'){dofile(file_cntrl,"./LUIS/m_mov.txt","a",msg);}
      else if(msg[1]=='M'&&msg[2]=='T'){dofile(file_cntrl,"./LUIS/m_tel.txt","a",msg);}
      else if(msg[1]=='L'&&msg[2]=='A'){dofile(file_cntrl,"./LUIS/l_att.txt","a",msg);}
      else if(msg[1]=='L'&&msg[2]=='M'){dofile(file_cntrl,"./LUIS/l_mov.txt","a",msg);}
      else if(msg[1]=='L'&&msg[2]=='T'){dofile(file_cntrl,"./LUIS/l_tel.txt","a",msg);}}
  else if(msg[0]=='P')
    {
      if(msg[1]=='M'&&msg[2]=='A'){dofile(file_cntrl,"./PACO/m_att.txt","a",msg);}
      else if(msg[1]=='M'&&msg[2]=='M'){dofile(file_cntrl,"./PACO/m_mov.txt","a",msg);}
      else if(msg[1]=='M'&&msg[2]=='T'){dofile(file_cntrl,"./PACO/m_tel.txt","a",msg);}
      else if(msg[1]=='L'&&msg[2]=='A'){dofile(file_cntrl,"./PACO/l_att.txt","a",msg);}
      else if(msg[1]=='L'&&msg[2]=='M'){dofile(file_cntrl,"./PACO/l_mov.txt","a",msg);}
      else if(msg[1]=='L'&&msg[2]=='T'){dofile(file_cntrl,"./PACO/l_tel.txt","a",msg);}}
}

void * comunicacion(void * parametro){
  char buffer[3];
  int * id_canal=(int *)parametro;
  int no_bytes,cont=0;

  file_sem = sem_open("file_sem",O_CREAT,00666,1);
  if(file_sem==SEM_FAILED){puts("ups!");exit(0);}
  
  //paso 5 recibir informacion
  printf("La IP: %s El canal: %d\n", inet_ntoa(client.sin_addr),*id_canal);
  while(1){
    if((no_bytes=recv(*(id_canal),(void *)buffer,sizeof(buffer),0))==-1){
      printf("error al recibir\n");exit(1);}
    else{
      buffer[no_bytes]='\0';
      //usleep(600);
      sem_wait(file_sem);
      archivito(buffer);
      sem_post(file_sem);
      cont++;
      printf("%d - Client[%d]:%s\n",cont,*id_canal,buffer);
      //paso 6 enviar respuesta
      if(strcmp("\0",buffer)==0){
	printf("Cerre Canal %d\n\n",*id_canal);
	break;
      }
      else{
	printf("\n\n");
      }
      strcpy(buffer,"ok");
      if(send(*(id_canal),(void *)buffer,sizeof(buffer),0)==-1){
	printf("\nerror al enviar\n");
	exit(1);
      }
    }
  }
  //paso 7 cerrar el canal
  close(*(id_canal));
  free(id_canal);
  return 0;
}


int main (int argc, char *argv[])
{
  pthread_t *hilos;

  int *id_canal;
  socklen_t longC;
  //paso 1 Creacion del socket
  int id_socket;
  if(argc!=2)
    {
      printf("Modo de uso: %s <puerto>\n",argv[0]);
      exit(1);
    }
  if((id_socket=socket(AF_INET,SOCK_STREAM,0))==-1)
    {
      printf("\n error al crear el socket\n");
      exit(1);
    }
  
  printf("\nSocket creado\n");
  memset((void *)&server,0,sizeof(server));
  server.sin_family=AF_INET; //la misma familia del socket
  server.sin_port=htons(atoi(argv[1])); //transforma el puerto de string a int y luego a host
  server.sin_addr.s_addr=INADDR_ANY;
  //paso 2 Hacer la publicacion del socket
  if(bind(id_socket,(struct sockaddr *)&server,(socklen_t)sizeof(server))==-1)
    {
      printf("\nerror al hacer la publicacion del socket\n");
      close(id_socket);
      exit(1);
    }
  printf("\nsocket abierto\n");
  //paso 3 pornerse a escuchar peticiones
  if(listen(id_socket,BACKLOG)==-1) //el 5 esta casi de mas xdxdx
    {
      printf("\nerror al escuchar en socket\n");
      close(id_socket);
      exit(1);
    }
  longC=sizeof(client);
  //paso 4 aceptar peticion
  while(1)
    {
      printf("\nEsperando al client\n");
      id_canal=(int *)malloc(sizeof(int));

      *id_canal=accept(id_socket,(struct sockaddr *)&client,&longC);
      if(*id_canal==-1)
	{

	  printf("\nerror al aceptar la peticion\n");
	  close(id_socket);
	  perror("accept");
	  free(id_canal);
	  exit(1);
	}

      int res = getpeername(*(id_canal), (struct sockaddr *)&client, &longC);
      printf("RES: %d\n",res );
      char clientip[20];
      strcpy(clientip, inet_ntoa(client.sin_addr));

      printf("IP CLIENT : %s\n",clientip );


      hilos=(pthread_t *) malloc(sizeof(pthread_t));
      pthread_create(hilos,NULL,comunicacion,(void *)id_canal);
      free(hilos);
    }
  return 0;
}
