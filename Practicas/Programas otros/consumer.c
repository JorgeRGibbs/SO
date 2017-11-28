#include "init.h"

sem_t * sem[4];
int shm_fd,socket_id;
mdata * ptr;
struct sockaddr_in con_serv;

//0: Producer 1: Consumer 2: Critical Zone

void * consumer();

int main(int argc, char * argv[])
{
  if(argc!=4) return 0;
  int thread_num = atoi(argv[1]),index[atoi(argv[1])];
  pthread_t hilos[thread_num];

  for(int i=0;i<ZS;i++)index[i]=i;

  shm_fd = shm_open("/zc_0", O_CREAT | O_RDWR, 0666);
  if(shm_fd==-1){perror("OPEN");}
  ftruncate(shm_fd,sizeof(mdata));
  ptr = (mdata *)mmap(0,sizeof(mdata),PROT_READ | PROT_WRITE,MAP_SHARED,shm_fd,0);
  if((void*)ptr==MAP_FAILED){perror("MAP");}

  if((socket_id=socket(AF_INET,SOCK_STREAM,0))==-1){puts("error");exit(0);}
  memset((void *)&con_serv,0,sizeof(con_serv));
  con_serv.sin_family=AF_INET;
  con_serv.sin_port=htons(atoi(argv[2]));
  con_serv.sin_addr.s_addr=inet_addr(argv[3]);
  if(connect(socket_id,(struct sockaddr *)&con_serv,(socklen_t)sizeof(con_serv))==-1)
    {printf("\nerror al conectarse al socket\n");exit(1);}
  
  sem[0] = sem_open("semP",O_CREAT,00666,2);
  sem[1] = sem_open("semC",O_CREAT,00666,0);
  sem[2] = sem_open("semZ",O_CREAT,00666,1);
  sem[3] = sem_open("semConn",O_CREAT,00666,1);
  
  if(sem[0]==SEM_FAILED||sem[1]==SEM_FAILED||
     sem[2]==SEM_FAILED||sem[3]==SEM_FAILED)
    {printf("\nError\n");exit(0);}
  
  for(int i=0;i<thread_num;i++) pthread_create(&hilos[i],NULL,consumer,NULL);
  for(int j=0;j<thread_num;j++) pthread_join(hilos[j],NULL);

  for(int i=0;i<4;i++)sem_close(sem[i]);
  sem_unlink("semP");sem_unlink("semC");sem_unlink("semZ");sem_unlink("semConn");
  puts("Done.");
  return 0;
}
void * consumer()
{
  char buffer[4];
  int flg = 'n';
  for(int i = 0;i<5;i++)
    {
      sem_wait(sem[1]);sem_wait(sem[2]);
      for(int j=0;j<ZN;j++)
	{
	  flg='n';
	  if(ptr->data[j][0]=='d')
	    {
	      flg='y';
	      ptr->data[j][0]='r';
	      sem_post(sem[2]);
	      sem_wait(sem[3]);
	      while(1)
		{
		  sprintf(buffer,"%c%c%c\0",ptr->data[j][1],ptr->data[j][2],
			  ptr->data[j][3],ptr->data[j][4],ptr->data[j][5]);
		  if(send(socket_id,(void *)buffer,strlen(buffer),0)==-1)
		    {printf("[error al enviar]\n");continue;}
		  if(recv(socket_id,(void *)buffer,sizeof(buffer),0)==-1)
		    {printf("[error al recibir confirmación]\n");continue;}
		  if(!memcmp(buffer,"ok",2))break;
	      }
	      sem_post(sem[3]);
	      ptr->data[j][0]='v';
	      sem_post(sem[0]);
	      break;
	    }
	  if(j==ZN-1&&flg=='n'){sem_post(sem[2]);i--;}
	}
    }
  pthread_exit(NULL);
}
