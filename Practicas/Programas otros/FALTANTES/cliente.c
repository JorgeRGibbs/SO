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

struct sockaddr_in server;

int main(int argc, char*argv[])
{
  int socket_id,aux;
  long long filesize;
  FILE * img;
  char buffer[1000];
  char fn[500];
  ssize_t readsize;
  size_t bufsize = sizeof(buffer);

  //CREACIÓN DEL SOCKET
  if(argc!=3){printf("Uso: %s <puerto> <ip del servidor>\n",argv[0]); exit(1);}
  if((socket_id=socket(AF_INET,SOCK_STREAM,0))==-1)
    {printf("error al crear el socket\n");exit(1);}
  printf("Socket creado\n");

  //LLENADO DE ESTRUCTURA DE DIRECCIÓN DEL SERVIDOR
  memset((void *)&server,0,sizeof(server));
  server.sin_family=AF_INET; //la misma familia del socket
  server.sin_port=htons(atoi(argv[1])); //transforma el puerto de string a int y luego a host
  server.sin_addr.s_addr=inet_addr(argv[2]);

  //CONECTANDO AL SERVIDOR
  puts("Conectando al servidor...");
  while(1) if(connect(socket_id,
		      (struct sockaddr *)&server,
		      (socklen_t)sizeof(server))!=-1){break;}
  puts("Conexión OK");

  //OBTENCIÓN DE TAMAÑO DE IMAGEN
  printf("Nombre del archivo: ");
  scanf("%[^\n]s",fn);
  if((img = fopen(fn,"rb"))==NULL){perror("Error abriendo archivo");exit(1);}
  fseek(img,0L,SEEK_END);
  filesize = ftell(img);
  //uint32_t un = htonl(filesize);
  rewind(img);
  printf("IMG TAM: %ld\n",filesize);

  if((send(socket_id,&filesize,sizeof(filesize),0))==-1)exit(1);
  aux=recv(socket_id,(void *)buffer,sizeof(buffer),0);
  
  if(aux==-1){printf("\nerror al recibir\n");exit(1);}
  //ENVIADO DE IMAGEN
  puts("enviando imagen...");
  while(1)
    {
      if(send(socket_id,(void *)buffer,strlen(buffer),0)==-1)
	{printf("[error al enviar]\n");/*fseek(img,-buffer,SEEK_CUR);*/break; }
      printf("IMG TAM: %d\n",filesize);
      filesize -= bufsize;
      aux=recv(socket_id,(void *)buffer,sizeof(buffer),0);
      if(aux==-1){printf("\nerror al recibir\n");exit(1);}
      if(filesize<=0)break;
    }
  close(socket_id);
  fclose(img);
}
