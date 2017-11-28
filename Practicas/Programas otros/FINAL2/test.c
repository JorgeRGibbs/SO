#include <stdio.h>

void ppp(char * t)
{
  printf("%c%c",t[0],t[1]);
}
int main()
{
  char meh[2][2]={{'a','b'},{'c','d'}};
  ppp(meh[0]);
  int i = -1;
  if(i)puts(":)");
  i = 0;
  if(i)puts(":(");
  return 0;
}

