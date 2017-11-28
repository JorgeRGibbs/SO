#include <stdio.h>
#include <pthread.h>
#include <fcntl.h>
#include <sys/stat.h>
#include <semaphore.h>
#include <string.h>
#include <stdlib.h>
#include <unistd.h>
#define TH_SZ 5

//--GLOBALES--
char zc[4][3];   //Zona crítica
sem_t * semP[4];   //Semáforos para productores
sem_t * semC[4];   //Semáforos para consumidores
//------------

//------PROTOTIPOS------
void * productor(void*);     //Función del productor
void * consumidor(void *);   //Función del consumidor
//----------------------

//--------------------FUNCIONES AUXILIARES--------------------
char * nP(int i){char*buf=malloc(6);sprintf(buf,"semP%d\0",i);return buf;}
char * nC(int i){char*buf=malloc(6);sprintf(buf,"semC%d\0",i);return buf;}
char getChar(int id){if(id==1)return'A';if(id==2)return'L';if(id==3)return'P';}
char getCont(int id){if(id==0)return'A';if(id==1)return'M';if(id==2)return'T';}
//------------------------------------------------------------

//---------------MAIN---------------
int main(int argc, char * argv[])
{
  //Inicializar semáforos
  for(int i=0;i<4;i++)
    {
      semP[i]=sem_open(nP(i),O_CREAT,00666,1);
      semC[i]=sem_open(nP(i),O_CREAT,00666,0);
    }
  for(int i=0;i<4;i++)
    {
      if(semP[i]==SEM_FAILED||semC[i]==SEM_FAILED)
	{
	  sem_close(semP[i]);sem_close(semC[i]);
	  puts("Error en la creación de semáforos.");
	  exit(1);
	}
    }
  //Inicializar hilos
  pthread_t threads[TH_SZ];int x=1,y=2,z=3;
  pthread_create(&threads[0],NULL,productor,(void *)&x);   //Productor
  pthread_create(&threads[1],NULL,productor,(void *)&y);   //Productor
  pthread_create(&threads[2],NULL,productor,(void *)&z);   //Productor
  pthread_create(&threads[3],NULL,consumidor,NULL);        //Consumidor
  pthread_create(&threads[4],NULL,consumidor,NULL);        //Consumidor
  //Esperar hilos
  for(int i=0;i<TH_SZ;i++)pthread_join(threads[i],NULL);
  //Cerrar semáforos
  for(int i=0;i<4;i++){sem_close(semP[i]);sem_close(semC[i]);}
  //Desligar semáforos
  for(int i=0;i<4;i++){sem_unlink(nP(i));sem_unlink(nC(i));}

  return 0;
}
//----------------------------------

//----------PRODUCTOR----------
void * productor(void * argv)
{
  int cont,*id=(int *)argv,val,self=pthread_self();
  char d = getChar(*id),msg[3][3] = {{d,'M','A'},{d,'M','T'},{d,'M','M'}};
  for(int i=0,cont=0;i<2;i++)
    {
      for(int j=0;j<4;j++)
	{
	  if(sem_getvalue(semP[j],&val)){perror("Error (P)");}
	  else if(val>0)
	    {
	      //sleep(1);
	      sem_wait(semP[j]);
	      zc[j][0]=getChar(*id);zc[j][1]='M';zc[j][2]=getCont(cont);
	      printf("%d - %d -  %c%c%c\n",self,j,zc[j][0],zc[j][1],zc[j][2]);
	      cont++;
	      sem_post(semC[j]);
	    }
	  if(cont>2){break;}/*if(cont>2&&j>3)j=0;/**/
	}if(cont<2)i--;else if(cont>2) cont=0; /**/}}
//-----------------------------

//---------CONSUMIDOR---------
void * consumidor(void * argv)
{
  int val, bool=0;
  for(int i=0;i<3;i++)
    {
      for(int j=0;j<4;j++)
	{
	  if(sem_getvalue(semC[j],&val)){perror("Error (C).");}
	  else if(val>0)
	    {
	      //sleep(1);
	      sem_wait(semC[j]);
	      printf("[%d] %c%c%c\n",j,zc[j][0],zc[j][1],zc[j][2]);
	      bool+=1;
	      sem_post(semP[j]);
	    }
	}if(bool<4)i--;}}
//----------------------------
