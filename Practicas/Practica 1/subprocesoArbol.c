#include <sys/types.h>
#include <unistd.h>
#include <stdio.h>
#include <sys/wait.h>
#include <stdlib.h>

int creaSubProceso(pid_t proceso, int estatus, int n, int l);

int main(){
  pid_t proceso;
  int estatus;
  int niveles = 3;
  int i,j;
  printf("¿Cuantos niveles desea?\n");
  scanf("%d",&niveles);
  while(niveles<=0){
    printf("Ingrese un numero mayor que cero\n");
    scanf("%d",&niveles);
  }
  i=niveles;
  if(niveles>0){
      printf("Yo soy el padre %d nivel %d\n", getpid(),niveles);
      niveles-=1;
        creaSubProceso(proceso, estatus, niveles, 4);
        creaSubProceso(proceso, estatus, niveles, 3);
        wait(&estatus);
  }
  return 0;
}

int creaSubProceso(pid_t proceso, int estatus,int n, int l){
  int i;
  if(n>=1){

    proceso = fork();
    switch(proceso){
      case -1:
        printf("Error\n");
        break;
      case 0:
        printf("Yo soy el hijo %d y mi padre es %d nivel %d\n",getpid(), getppid(),n);
        for(i=0;i<l;i++){
          creaSubProceso(proceso, estatus, n-1, l);
          wait(&estatus);
        }
        exit(0);
        break;
    }
  }

}
