#include <stdio.h>
#include <stdlib.h>

void greetuser(char *user)

{
  char hello [72];
  
  if (language == 1) {
    hello[0] = 'H';
    hello[1] = 'y';
    hello[2] = 'v';
    hello[3] = -0x3d;
    hello[4] = -0x5c;
    hello[5] = -0x3d;
    hello[6] = -0x5c;
    hello[7] = ' ';
    hello[8] = 'p';
    hello[9] = -0x3d;
    hello[10] = -0x5c;
    hello[11] = 'i';
    hello[12] = 'v';
    hello[13] = -0x3d;
    hello[14] = -0x5c;
    hello[15] = -0x3d;
    hello[16] = -0x5c;
    hello[17] = ' ';
    hello[18] = '\0';
  }
  else if (language == 2) {
    hello[0] = 'G';
    hello[1] = 'o';
    hello[2] = 'e';
    hello[3] = 'd';
    hello[4] = 'e';
    hello[5] = 'm';
    hello[6] = 'i';
    hello[7] = 'd';
    hello[8] = 'd';
    hello[9] = 'a';
    hello[10] = 'g';
    hello[11] = '!';
    hello[12] = ' ';
    hello[13] = '\0';
  }
  else if (language == 0) {
    hello[0] = 'H';
    hello[1] = 'e';
    hello[2] = 'l';
    hello[3] = 'l';
    hello[4] = 'o';
    hello[5] = ' ';
    hello[6] = '\0';
  }
  strcat(hello,user);
  puts(hello);
  return;
}

int main(int ac,char **av)

{
  char *pcVar1;
  char *env;
  int tmp1;
  char user [72];
  
  if (ac == 3) {

  //     0804854c b8 00 00        MOV        EAX,0x0
  //          00 00
  // 08048551 ba 13 00        MOV        EDX,0x13
  //          00 00
  // 08048556 89 df           MOV        EDI,EBX
  // 08048558 89 d1           MOV        ECX,EDX
  // 0804855a f3 ab           STOSD.REP  ES:EDI

  // undefined4 *puVar2;
    // puVar2 = (undefined4 *)user;
    // for (tmp1 = 0x13; tmp1 != 0; tmp1 = tmp1 + -1) {
    //   *puVar2 = 0;
    //   puVar2 = puVar2 + 1;
    // }

    // which is equal to memset()
    strncpy(user,av[1],0x28);
    strncpy(user + 0x28,av[2],0x20);
    // pcVar1 = "LANG";
    env = getenv("LANG");
    if (env != (char *)0x0) {
      pcVar1 = env;
      tmp1 = memcmp(env,"fi",2);
      if (tmp1 == 0) {
        language = 1;
      }
      else {
        tmp1 = memcmp(env,"nl",2);
        pcVar1 = env;
        if (tmp1 == 0) {
          language = 2;
        }
      }
    }
  //     0804861a 8d 5c 24 50     LEA        EBX=>user,[ESP + 0x50]
  // 0804861e b8 13 00        MOV        tmp1,0x13
  //          00 00
  // 08048623 89 d7           MOV        EDI,EDX
  // 08048625 89 de           MOV        ESI,EBX
  // 08048627 89 c1           MOV        ECX,tmp1
  // 08048629 f3 a5           MOVSD.REP  ES:EDI,ESI

  // byte bVar4;
  // bVar4 = 0;
  // undefined4 *puVar3;
    // puVar2 = (undefined4 *)user;
    // puVar3 = (undefined4 *)&stack0xffffff50;
    // for (tmp1 = 0x13; tmp1 != 0; tmp1 = tmp1 + -1) {
    //   *puVar3 = *puVar2;
    //   puVar2 = puVar2 + (uint)bVar4 * -2 + 1;
    //   puVar3 = puVar3 + (uint)bVar4 * -2 + 1;
    // }
    
    // some operation to set user in case it's not defined??
    pcVar1 = greetuser(pcVar1);
  }
  else {
    pcVar1 = (char *)0x1;
  }
  return (int)pcVar1;
}