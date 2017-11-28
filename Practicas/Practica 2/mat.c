#include <stdio.h>
#include <unistd.h>
#include <stdlib.h>
#include <sys/types.h>
#include <sys/wait.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <string.h>
#include <time.h>

//Columnas A = Filas B

void inputD(int*rowsA,int*cA_rB,int*colsB,int*procs)
{
  printf("\n# filas en A: "); scanf("%d",rowsA);
  printf("\n# columnas en A (filas en B): "); scanf("%d",cA_rB);
  printf("\n# columnas en B: "); scanf("%d",colsB);
  printf("\n# de procesos: "); scanf("%d",procs);
  if(*rowsA<=0||*cA_rB<=0||*colsB<=0||*procs<=0) puts("\nAlgún tamaño no es válido.\n");
  if(*rowsA<*procs){puts("\nNo pueden haber más procesos que filas.\nTerminando...");exit(0);}
}

int ** mMemRes(int R,int C)
{
  int it,** matrix;
  matrix = malloc(R*sizeof(int *));
  for(it=0;it<C;it++) matrix[it] = malloc(C*sizeof(int));
  return matrix;
}

void mFillPrint(int rowsA,int cA_rB,int colsB,int ** A,int ** B,char index)
{
  int it, it_, r;
  srand(time(NULL));
  switch(index)
    {
    case 'f':
      for(it=0;it<rowsA;it++)
	for(it_=0;it_<cA_rB;it_++)
	  A[it][it_]=rand()%9+1;
      for(it=0;it<cA_rB;it++)
	for(it_=0;it_<colsB;it_++)
	  B[it][it_]=rand()%9+1;
      break;
    case 'p':
      puts("");
      for(it=0;it<rowsA;it++)
	{
	  for(it_=0;it_<cA_rB;it_++);
	  //printf("%d ",A[it][it_]);
	  //puts("");
	}
      puts("");
      for(it=0;it<cA_rB;it++)
	{
	  for(it_=0;it_<colsB;it_++);
	  //printf("%d ",B[it][it_]);
	  //puts("");
	}
      puts("");
      break;
    }
}

void procCalc(int ini,int fin,
	      int cA_rB,int colsB,
	      int ** A,int ** B,int (*fd)[2])
{
  int it1,it2,it3,accum;
  char str[255];
  //printf("ini[%d]\n",ini);
  for(ini;ini<=fin;ini++)
    {
      close(fd[ini][0]);
      str[0]='\0';
      sprintf(str+strlen(str),"%d|",ini);
      for(it1=0;it1<colsB;it1++)
	{
	  for(it2=0,accum=0;it2<cA_rB;it2++) accum+=A[ini][it2]*B[it2][it1];
	  sprintf(str+strlen(str),"\t[%d]\t",accum);
	}
      sprintf(str+strlen(str),"|\n",ini);
      write(fd[ini][1],str,strlen(str)+1);
    }
  exit(0);
}

int main(int argc, char * argv[])
{
  //Declaración de variables
  int **matrixA, **matrixB, (*fd)[2], rowsA, cA_rB, colsB, procs,it,i;
  char recv[255];
  pid_t pid;
  //Recibir del usuario
  inputD(&rowsA,&cA_rB,&colsB,&procs);
  //Reservación de espacio en memoria
  matrixA=mMemRes(rowsA,cA_rB);
  matrixB=mMemRes(cA_rB,colsB);
  fd=malloc(rowsA*sizeof(int[2]));
  //Llenado de matrices
  mFillPrint(rowsA,cA_rB,colsB,matrixA,matrixB,'f');
  //Imprime matrices
  mFillPrint(rowsA,cA_rB,colsB,matrixA,matrixB,'p');
  //Cálculo de procesos
  int tasks=rowsA/procs,residue=rowsA%procs;
  for(it=0;it<rowsA;it++)pipe(fd[it]);
  
  for(it=1;it<=procs;it++)
    {
      pid = fork();
      switch(pid)
	{
	case 0:
	  if(residue>0 && it==procs)
	    procCalc(tasks*(it-1),(tasks*it+residue)-1,cA_rB,colsB,matrixA,matrixB,fd);
	  else
	    procCalc(tasks*(it-1),(tasks*it)-1,cA_rB,colsB,matrixA,matrixB,fd);
	  break;
	default:
	  if(it==procs)
	    {
	      while(wait(NULL)>0);
	      for(i=0;it<rowsA;it++)close(fd[i][1]);
	    }
	  break;
	case -1:
	  break;
	} 
    }
  for(it=0;it<rowsA;it++)
    {
      read(fd[it][0],recv,sizeof(recv));
      printf("%s",recv);
    }
}
