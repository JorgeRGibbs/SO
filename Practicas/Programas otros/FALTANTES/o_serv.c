#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <errno.h>
#include <string.h>
#include <sys/types.h>

#define BACKLOG 3

int main(int argc, char *argv[])
{
  if(argc!=3){printf("USO: [servidor] <puerto> <archivo>\n");exit(1);}
  int socket_id;
  int id_canal;
  struct sockaddr_in servidor;
  int numrv;
  unsigned char buff[256];

  if((socket_id = socket(AF_INET, SOCK_STREAM, 0))==-1){perror("Socket");exit(1);}

  printf("Se abrió Socket\n");

  //Se inicializan variables
  memset(&servidor, '0', sizeof(servidor));
  memset(buff, '0', sizeof(buff));
  //Se inicializa estructura
  servidor.sin_family = AF_INET;
  servidor.sin_addr.s_addr = htonl(INADDR_ANY);
  servidor.sin_port = htons(atoi(argv[1]));

  if(bind(socket_id, (struct sockaddr*)&servidor,sizeof(servidor))==-1)
    {close(socket_id);perror("Bind");exit(1);}
  
  if(listen(socket_id, BACKLOG) == -1){perror("Listen\n");exit(1);}
  
  while(1)
    {
      id_canal = accept(socket_id, (struct sockaddr*)NULL ,NULL);

      FILE *fp = fopen(argv[2],"rb");
      if(fp==NULL){perror("Archivo:");}

      while(1)
        {
	  printf("Enviando...\n");
	  memset(buff, '0', sizeof(buff));
	  int nread = fread(buff,1,256,fp);
	  
	  if(nread > 0)write(id_canal, buff, nread);
	  if (nread < 256)
            {
	      if (feof(fp))printf("terminado\n");
	      if (ferror(fp))printf("Lectura\n");
	      break;
            }
        }
      close(id_canal);
    }
  return 0;
}


