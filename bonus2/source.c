#include <stdio.h>
#include <stdlib.h>
#include <string.h>

int language = 0;

void greetuser(char *user)

{
  char hello [64];
  
  if (language == 1) {
    strcpy(hello, "Hyvää päivää ");
  }
  else if (language == 2) {
    strcpy(hello, "Goedemiddag! ");
  }
  else if (language == 0) {
    strcpy(hello, "Hello ");
  }
  strcat(hello,user);
  puts(hello);
  return;
}

int main(int ac,char **av)
{
  char user[72];
  char *env;
  
  if (ac == 3) {
    memset(user, 0, 72);
    strncpy(user, argv[1], 40);
    strncpy(&user[40], argv[2], 32);
    env = getenv("LANG");
    if (env)
    {
      if (memcmp(env, "fi", 2) == 0)
        language = 1;
      else if (memcmp(env, "nl", 2) == 0)
        language = 2;
    }
    greetuser(user);
    return 0;
  }
  else {
    return 1;
  }
}