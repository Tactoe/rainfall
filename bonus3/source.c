#include <stdio.h>
#include <stdlib.h>

int main(int ac,char **av)
{
  int i;
  char buffer [132];
  FILE *filePointer;
  
  filePointer = fopen("/home/user/end/.pass","r");
  memset(buffer, 0, 132);

  if ((filePointer == (FILE *)0x0) || (ac != 2)) {
    i = -1;
  }
  else {
    fread(buffer,1,0x42,filePointer);
    buffer[65] = '\0';
    i = atoi(av[1]);
    buffer[i] = '\0';
    fread(buffer + 0x42,1,0x41,filePointer);
    fclose(filePointer);
    i = strcmp(buffer,av[1]);
    if (i == 0) {
      execl("/bin/sh","sh",0);
    }
    else {
      puts(buffer + 0x42);
    }
    i = 0;
  }
  return i;
}
