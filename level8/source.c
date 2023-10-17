#include <stdio.h>
#include <string.h>
#include <stdlib.h>

char *auth = NULL;
char *service = NULL;

int main(void)

{
  char buffer [128];
  
  do {
    printf("%p, %p \n",auth,service);
    
    if (fgets(buffer,0x80,stdin) == 0) {
      return 0;
    }
    
    if (strncmp(buffer, "auth ", 5) == 0) {
      auth = malloc(4);
     if (strlen(buffer + 5) < 31) {
        strcpy(auth,buffer + 5);
     }
    }
    
    if (strncmp(buffer, "reset", 5) == 0) {
      free(auth);
    }

    if (strncmp(buffer, "service", 6) == 0) {
      service = strdup(buffer + 7);
    }
    
    if (strncmp(buffer, "login", 5); == 0) {
      if (auth[32] == 0) {
        fwrite("Password:\n",1,10,stdout);
      }
      else {
        system("/bin/sh");
      }
    }
  } while( true );
}
