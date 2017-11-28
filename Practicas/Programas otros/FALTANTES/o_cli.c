#include <sys/socket.h>
#include <sys/types.h>
#include <netinet/in.h>
#include <netdb.h>
#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <unistd.h>
#include <errno.h>
#include <arpa/inet.h>

int main(int argc,char * argv[])
{
  if(argc!=4){printf("USO: [cliente] <puerto> <IP servidor> <archivo>\n");exit(1);}
  int socket_id;
  int bytes = 0;
  char recvBuff[256];
  struct sockaddr_in serv_addr;

  memset(recvBuff, '0', sizeof(recvBuff));

  if((socket_id = socket(AF_INET, SOCK_STREAM, 0))==-1){perror("Socket");exit(1);}

  serv_addr.sin_family = AF_INET;
  serv_addr.sin_port = htons(atoi(argv[1]));
  serv_addr.sin_addr.s_addr = inet_addr(argv[2]);

  if(connect(socket_id, (struct sockaddr *)&serv_addr, sizeof(serv_addr))<0)
    {perror("Connect");exit(1);}

  FILE *fp;
  fp = fopen(argv[3], "wb"); 
  if(NULL == fp){printf("Error opening file");exit(1);}
  
  while((bytes = read(socket_id, recvBuff, 256)) > 0)
    {
      fwrite(recvBuff, 1,bytes,fp);
    }

  if(bytes < 0) perror("Lectura");
  
  return 0;
}
