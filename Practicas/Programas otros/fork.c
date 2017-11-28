#include <stdio.h>
#include <unistd.h>
#include <stdlib.h>
#include <sys/types.h>
#include <sys/wait.h>

void leftChild(int count,int level)
{
  int it,status;
  pid_t id;
  if(count==level)exit(0);
  for(it=0;it<3;it++)
    {
      id=fork();
      if(id==0)
	{
	  printf("LEVEL[%d] => I am (%d) a left spawn of\t· <- %d -|\n",count,getpid(),getppid());
	  //Se corre la función de leftChild en cada fork, creando 3 hijos nuevos por fork
	  leftChild(count+1,level);//Se pasa el contador con 1 más
	  break;
	}
      else if(id>0)if(it==2)while(wait(NULL)>0);
    }
}

void rightChild(int count,int level)
{
  int it,status;
  pid_t id;
  if(count==level)exit(0);
  for(it=0;it<4;it++)
    {
      id=fork();
      if(id==0)
	{
	  printf("LEVEL[%d] => I am (%d) a right spawn of\t· |- %d ->\n",count,getpid(),getppid());
	  //Se corre la función rightChild en cada fork, creando 4 hijos nuevos por fork
	  rightChild(count+1,level);//Se pasa el contador con 1 más
	  break;
	}
      else if(id>0) if(it==3)while(wait(NULL)>0);
    }
}


int main(int argc, char * argv[])
{
  if(argc != 2)
    {
      printf("\nFallo: Uso incorrecto.\n\nUso del programa:\n\n\t./fork [número de niveles]\n\n");
      return 0;
    }
  int it, status, level=atoi(argv[1]), count=0;
  if(level<=0)
    {
      printf("\nNo se pueden tener 0 cero niveles\n\n");
      return 0;
    }else if(level==1)
    {
      printf("LEVEL[%d] => I am satan [%d], son of [%d]\n",count,getpid(),getppid());
      return 0;
    }
  printf("LEVEL[%d] => I am satan [%d], son of [%d]\n",count,getpid(),getppid());
  pid_t id;
  
  for(it=0;it<2;it++)
    {
      id=fork();
      if(id==0)
	{
	  if(it==0)//0 es el hijo izquierdo
	    {
	      printf("LEVEL[%d] => I am [%d], left son of satan\t· <- %d ·]\n",
		     count+1,getpid(),getppid());
	      leftChild(count+2,level);
	      break;
	    }
	  if(it==1)//1 es el hijo derecho
	    {
	      printf("LEVEL[%d] => I am [%d], right son of satan\t· [· %d ->\n",
		     count+1,getpid(),getppid());
	      rightChild(count+2,level);
	    }
	  break;
	}
      else if(id>0) if(it==1)while(wait(NULL)>0);
    }
  return 0;
}
