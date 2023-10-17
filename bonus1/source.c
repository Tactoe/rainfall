#include <stdio.h>
#include <stdlib.h>

int main(int ac,char **av)
{
  int i;
  char buffer [40];
  
  i = atoi(av[1]);
  if (i < 10) {
    memcpy(buffer,av[2],i * 4);
    if (i == 0x574f4c46) { // 1464814662
      execl("/bin/sh","sh",0);
    }
    i = 0;
  }
  else {
    i = 1;
  }
  return i;
}