#include <stdio.h>
#include <stdlib.h>
#include <string.h>

char * p(char *dest, char *header)
{
    char buffer [4096];
    
    puts(header);
    read(0,buffer,0x1000);
    *strchr(buffer, '\n') = 0;
    return strncpy(dest,buffer,0x14);
}

char * pp(char *string)
{
    uint i = 0;
    char a [20];
    char b [20];
    
    p(a," - ");
    p(b," - ");
    strcpy(string,a);
    i = strlen(buffer);
    buffer[i] = ' ';
    buffer[i + 1] = 0;
    return strcat(string,b);
}


int main(void)
{
  char buffer [42];
  
  pp(buffer);
  puts(buffer);
  return 0;
}