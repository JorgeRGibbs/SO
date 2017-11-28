#include <pthread.h>
#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <unistd.h>

struct matrixbit{
  int id,ini,fin;
  int **A,**B,**res;
  int colsB,cA_rB,prntrange;
};

int inputD(int*rowsA,int*cA_rB,int*colsB,int*procs)
{
  printf("\n# filas en A: "); scanf("%d",rowsA);
  printf("\n# columnas en A (filas en B): "); scanf("%d",cA_rB);
  printf("\n# columnas en B: "); scanf("%d",colsB);
  printf("\n# de hilos: "); scanf("%d",procs);
  if(*rowsA<=0||*cA_rB<=0||*colsB<=0||*procs<=0) puts("\nAlgún tamaño no es válido.\n");
  if(*rowsA<*procs){puts("\nNo pueden haber más hilos que filas.\nTerminando...");return 1;}
  return 0;
}

void mFill(int rows,int cols,int ** mtx)
{for(int it=0;it<rows;it++)for(int it_=0;it_<cols;it_++)mtx[it][it_]=rand()%9;}

int ** mMemRes(int R,int C)
{
  int ** matrix = malloc(R*sizeof(int *));
  for(int it=0;it<R;it++)matrix[it] = malloc(C*sizeof(int));
  return matrix;
}

void * procCalc(void * mat)
{
  struct matrixbit * mtx = (struct matrixbit *) mat;
  int it1,it2,it3,accum;
  for(mtx->ini,it3=0;mtx->ini<=mtx->fin;mtx->ini++,it3++)
    {for(it1=0;it1<mtx->colsB;it1++){
	for(it2=0,accum=0;it2<mtx->cA_rB;it2++)
	  accum+=mtx->A[it3][it2]*mtx->B[it2][it1];
	mtx->res[it3][it1]=accum;}}
  pthread_exit((void*)mtx);
}

void fillStruct(struct matrixbit * mtx, int ini,int fin, int cA_rB,int colsB)
{
  mtx->ini = ini;
  mtx->fin = fin;
  mtx->cA_rB = cA_rB;
  mtx->colsB = colsB;
  mtx->A = mMemRes(fin-ini+1,cA_rB);
  mtx->res = mMemRes(fin-ini+1,colsB);
  mtx->prntrange= fin-ini;
  mFill(fin-ini+1,cA_rB,mtx->A);
  //for(int i=0;i<fin-ini+1;i++){for(int j=0;j<colsB;j++)printf("[%d]",mtx->A[i][j]);puts("");}
}

int main(int argc, char * argv[])
{
  srand(time(NULL));
  int hilos,rowsA,cA_rB,colsB,**B;
  if(inputD(&rowsA,&cA_rB,&colsB,&hilos))return 0;
  
  struct matrixbit * mtx = malloc(hilos*sizeof(struct matrixbit));
  pthread_t * thread = malloc(hilos*sizeof(pthread_t));
  B = mMemRes(cA_rB,colsB);
  mFill(cA_rB,colsB,B);
  //for(int i=0;i<cA_rB;i++){for(int j=0;j<colsB;j++)printf("<%d>",B[i][j]);puts("");}
  int tasks=rowsA/hilos,residue=rowsA%hilos;
  
  for(int it = 1; it <= hilos; it++)
    {
      int a = tasks*(it-1), b = (tasks*it)-1, c = (tasks*it+residue)-1;
      mtx[it-1].id = it;
      
      if(it==hilos && residue>0) fillStruct(&mtx[it-1],a,c,cA_rB,colsB);
      else fillStruct(&mtx[it-1],a,b,cA_rB,colsB);
      
      mtx[it-1].B = B;
      pthread_create(&thread[it-1],NULL,procCalc, (void *)&mtx[it-1]);
    }
  //Contenedor para los valores de retorno
  struct matrixbit ** mtxres = malloc(hilos*sizeof(struct matrixbit *));;
  //Recuperar valores de retorno
  for(int i = 0; i < hilos; i++) pthread_join(thread[i],(void **)&mtxres[i]);
  //Ordenar valores de retorno para impresión
  for(int i = 0, k=0; i < hilos; i++){
    if(mtxres[k]->id-1==i){for(int it=0;it<mtxres[k]->prntrange+1;it++){
	for(int it1=0;it1<colsB;it1++)printf("%d ",mtxres[k]->res[it][it1]);
	puts("");}k=0;}
    else{i--;k++;}};
  free(mtxres);free(B);
}
