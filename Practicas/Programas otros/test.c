#include <stdio.h>
#include <unistd.h>
#include <signal.h>
#include <fcntl.h>
#include <stdlib.h>
#include <wait.h>
#include <sys/types.h>
#include <string.h>

void procCalc(int ini,int fin,
	      int cA_rB,int colsB,
	     int ** A,int ** B)
{
  int it1,it2,it3,accum,news=fin-ini;
  int C[news][colsB];
  int fd;
  fd = open("res", O_WRONLY | O_CREAT | O_APPEND, S_IRUSR | S_IWUSR);
  char str[255];
  for(ini,it3=0;ini<=fin;ini++,it3++)
    {
      str[0]='\0';
      sprintf(str+strlen(str),"%d|",ini);
      for(it1=0;it1<colsB;it1++)
	{
	  for(it2=0,accum=0;it2<cA_rB;it2++) accum+=A[ini][it2]*B[it2][it1];
	  //C[it3][it1]=accum;
	  sprintf(str+strlen(str),"[%d]",accum);
	}
      sprintf(str+strlen(str),"|\n",ini);
      write(fd,str,strlen(str));
    }
  //exit(0);
}
int ** mMemRes(int R,int C)
{
  int it, ** matrix;
  matrix = malloc(R*sizeof(int *));
  for(it=0;it<C;it++) matrix[it] = malloc(C*sizeof(int));
  return matrix;
}
void mFill(int rowsA,int cA_rB,int colsB,int ** A,int ** B,int num1,int num2)
{
  int it, it_;
      for(it=0;it<rowsA;it++)
	for(it_=0;it_<cA_rB;it_++)
	  A[it][it_]=num1;
      for(it=0;it<cA_rB;it++)
	for(it_=0;it_<colsB;it_++)
	  B[it][it_]=num2;
}

int main()
{
/*mode_t mode = S_IRUSR | S_IWUSR;
  int i = 0, fd, pid;
  
  //PROCESOS
  unsigned char pi1 = '_', pi2 = '*', pi3 = '/', pin='\n';

  for(i=0;i<2;i++)
    {
      if((pid = fork()) < 0)
	{
	  perror("fork error");
	  exit(1);
	}

      if(pid == 0)
	{
	  if(i==0)
	    {
	      if((fd = open("res", O_WRONLY | O_CREAT | O_APPEND, mode)) < 0)
		{perror("open error");exit(1);}
	      if(write(fd, &pi2, 1) != 1){perror("write error");exit(1);}
	      close(fd);
	    }
	  else if(i==1)
	    {
	      if((fd = open("res", O_WRONLY | O_CREAT | O_APPEND, mode)) < 0)
		{perror("open error");exit(1);}
	      if(write(fd, &pi3, 1) != 1){perror("write error");exit(1);}
	      close(fd);
	    }
	  exit(0);
	  break;
	}
      else
	{
	  if(i==1)
	    {
	      while(wait(NULL)>0)puts("done!");
	      sleep(5);
	      if((fd = open("res", O_WRONLY | O_CREAT | O_APPEND, mode)) < 0)
		{
		  perror("open error");
		  exit(1);
		}
	      puts("parent!");
	      if(write(fd, &pi1, 1) != 1){perror("write error");exit(1);}
	      if(write(fd, &pin, 1) != 1){perror("write error");exit(1);}
	      close(fd);
	    }
	  else continue;
	}
	}
  //COMPOSICIÓN DE UN STRING
  int it,blep[] = {1,2,3,4,5,6};
  fd = open("res", O_WRONLY | O_CREAT | O_APPEND, mode);
  char str[100];
  sprintf(str+strlen(str),"|");
  for(it=0;it<sizeof(blep)/sizeof(blep[0]);it++)
    sprintf(str+strlen(str),"\t[%d]\t",blep[it]);
  sprintf(str+strlen(str),"|\n\0");
  write(fd,str,strlen(str));
  printf("size: %d, %.*s\n",strlen(str),strlen(str),str);*/
  //MATRIZ A STRING
  int ** B = mMemRes(4,2);
  
  return 0;
}

