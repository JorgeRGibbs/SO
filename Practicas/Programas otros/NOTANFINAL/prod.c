#include "init.h"

sem_t * sem[3];
int shm_fd;
mdata * ptr;

void * producer();

int main(int argc, char * argv[])
{
  if(argc!=2) return 0;
  int thread_num = atoi(argv[1]),index[atoi(argv[1])];

  for(int i=0;i<ZS;i++)index[i]=i;

  shm_fd = shm_open("/zc_0", O_CREAT | O_RDWR, 0666);
  if(shm_fd==-1){perror("OPEN");}
  ftruncate(shm_fd,sizeof(mdata));
  ptr = (mdata *)mmap(0,sizeof(mdata),PROT_READ | PROT_WRITE,MAP_SHARED,shm_fd,0);
  if((void*)ptr==MAP_FAILED){perror("MAP");}

  for(int i=0;i<ZN;i++)ptr->data[i][0]='v';
  
  pthread_t hilos[thread_num];
  
  sem[0] = sem_open("semP",O_CREAT,00666,2);
  sem[1] = sem_open("semC",O_CREAT,00666,0);
  sem[2] = sem_open("semZ",O_CREAT,00666,1);
  
  if(sem[0]==SEM_FAILED||sem[1]==SEM_FAILED||sem[2]==SEM_FAILED)
    {printf("\nError\n");exit(0);}
  
  for(int i=0;i<thread_num;i++)
    {
      pthread_create(&hilos[i],NULL,producer,(void *)&index[i]);
    }
  
  for(int j=0;j<thread_num;j++) pthread_join(hilos[j],NULL);

  for(int i=0;i<3;i++)sem_close(sem[i]);
  sem_unlink("semP");sem_unlink("semC");sem_unlink("semZ");
  
  return 0;
}
void * producer(void * tmp)
{
  int * index = (int *)tmp;
  char flg = 'n';
  for(int j = 0;j < 5;j++)
    {
      sem_wait(sem[0]);sem_wait(sem[2]);

      for(int i=0;i<ZN;i++)
	{
	  flg='n';
	  if(ptr->data[i][0]=='v')
	    {
	      flg = 'y';
	      ptr->data[i][0]='l';
	      sem_post(sem[2]);
	      switch(*index)
		{
		case 0:for(int k=1;k<ZS;k++)ptr->data[i][k]='x';break;
		case 1:for(int k=1;k<ZS;k++)ptr->data[i][k]='y';break;
		case 2:for(int k=1;k<ZS;k++)ptr->data[i][k]='z';break;
		}
	      puts("escrito");
	      ptr->data[i][0]='d';sem_post(sem[1]);
	      break;
	    }
	  if(i==ZN-1&&flg=='n'){sem_post(sem[2]);j--;}
	}}pthread_exit(NULL);
}
