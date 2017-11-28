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

int main()
{
  char buff[400];
  printf("%d\n",sizeof(buff));
  long long one = 1000000000;
  int two = 2;
}
