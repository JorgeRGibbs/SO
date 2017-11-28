#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/types.h> 
#include <sys/socket.h>
#include <arpa/inet.h>
#include <netinet/in.h>
#include <netdb.h>
#include <string.h>
#include <pthread.h>

#define BACKLOG 5

struct sockaddr_in servidor;
struct sockaddr_in cliente;
char fn[500];

void * comunicacion(void * parametro){
  FILE * img;
  char buffer[1000];
  int * id_canal=(int *)parametro;
  int no_bytes;
  char fn[500];
  long long filesize,aux;

  printf("Nombre del archivo: ");
  scanf("%[^\n]s",fn);
  //paso 5 recibir informacion
  printf("La IP: %s El canal: %d\n", inet_ntoa(cliente.sin_addr),*id_canal);
  puts("Recibiendo tamaño de la imagen...");
  
  recv(*(id_canal),&filesize,sizeof(filesize),0);
  printf("IMG TAM: %d\n",filesize);
  if((img=fopen(fn,"wb"))==NULL){perror("Error abriendo archivo");exit(1);}
  if(send(*(id_canal),(void *)buffer,sizeof(buffer),0)==-1){
    printf("\nerror al enviar\n");exit(1);}
  
  puts("Recibiendo imagen...");
  while(1)
    {
      printf("IMG TAM: %d\n",filesize);
      if((no_bytes=recv(*(id_canal),(void *)buffer,sizeof(buffer),0))==-1){
	printf("error al recibir\n");exit(1);}
      else
	{
	  fwrite(buffer,sizeof(char),sizeof(buffer),img);
	  printf("IMG TAM: %d\n",filesize);
	  filesize -=sizeof(buffer);
	  printf("IMG TAM: %d\n",filesize);
	  if(filesize<=0)break;
	  strcpy(buffer,"\t\t\t(ŏ_ŏ)");
	  if(send(*(id_canal),(void *)buffer,sizeof(buffer),0)==-1){
	    printf("\nerror al enviar\n");exit(1);}
	  /*buffer[no_bytes]='\0';
	  printf("Mensaje Cliente:%s\n",buffer);
	  //paso 6 enviar respuesta
	  if(strcmp("/q",buffer)==0){
	    printf("Cerre Canal %d\n\n",*id_canal);
	    break;
	  }
	  else{
	    printf("\n\n");
	  }
	  strcpy(buffer,"\t\t\t(ŏ_ŏ)");
	  if(send(*(id_canal),(void *)buffer,sizeof(buffer),0)==-1){
	    printf("\nerror al enviar\n");exit(1);}*/
	}
    }
  //paso 7 cerrar el canal
  close(*(id_canal));
  fclose(img);
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
  if(argc!=2){printf("Uso: %s <puerto>\n",argv[0]);exit(1);}
  if((id_socket=socket(AF_INET,SOCK_STREAM,0))==-1)
    {printf("\n error al crear el socket\n");exit(1);}  
  printf("\nSocket creado\n");
  
  memset((void *)&servidor,0,sizeof(servidor));
  servidor.sin_family=AF_INET; //la misma familia del socket
  servidor.sin_port=htons(atoi(argv[1])); //transforma el puerto de string a int y luego a host
  servidor.sin_addr.s_addr=INADDR_ANY;
  //paso 2 Hacer la publicacion del socket
  if(bind(id_socket,(struct sockaddr *)&servidor,(socklen_t)sizeof(servidor))==-1)
    {printf("\nerror al hacer la publicacion del socket\n");close(id_socket);exit(1);}
  printf("\nsocket abierto\n");
  
  //paso 3 pornerse a escuchar peticiones
  if(listen(id_socket,BACKLOG)==-1){printf("\nerror al escuchar en socket\n");
    close(id_socket);exit(1);}
  longC=sizeof(cliente);
  //paso 4 aceptar peticion

  printf("Nombre del archivo: ");
  scanf("%[^\n]s",fn);
  while(1)
    {
      printf("\nEsperando al cliente\n");
      id_canal=(int *)malloc(sizeof(int));

      *id_canal=accept(id_socket,(struct sockaddr *)&cliente,&longC);
      if(*id_canal==-1)
	{printf("\nerror al aceptar la peticion\n");close(id_socket);
	  perror("accept");free(id_canal);exit(1);}

      int res = getpeername(*(id_canal), (struct sockaddr *)&cliente, &longC);
      printf("RES: %d\n",res );
      char clientip[20];
      strcpy(clientip, inet_ntoa(cliente.sin_addr));

      printf("IP CLIENTE : %s\n",clientip );


      hilos=(pthread_t *) malloc(sizeof(pthread_t));
      pthread_create(hilos,NULL,comunicacion,(void *)id_canal);
      free(hilos);
    }
  return 0;
}
