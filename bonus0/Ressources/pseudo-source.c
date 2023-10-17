#include <stdio.h>
#include <stdlib.h>

char * p(char *dest, char *header)

{
  char *pcVar1;
  char buffer [4096];
  
  puts(header);
  read(0,buffer,0x1000);
  pcVar1 = strchr(buffer,10);
  *pcVar1 = '\0';
  pcVar1 = strncpy(dest,buffer,0x14);
  return pcVar1;
}

char * pp(char *string)
{
    uint i = 0;
    char a [20];
    char b [20];
    
    p(a," - ");
    p(b," - ");
    strcpy(string,a);
// 08048579 f2 ae           SCASB.RE   ES:EDI
// 0804857b 89 c8           MOV        EAX,ECX
// 0804857d f7 d0           NOT        EAX
// 0804857f 83 e8 01        SUB        EAX,0x1
// 08048582 03 45 08        ADD        EAX,dword ptr [EBP + string]
// 08048585 0f b7 13        MOVZX      EDX,dword ptr [x080486a4] => ' '
// 08048588 66 89 10        MOV        word ptr [EAX],DX

// is translated by ghidra as

//   char cVar1;
//   char *pcVar2;
//   uint uVar3;
//   byte bVar4;
//   bVar4 = 0;
//   uVar3 = 0xffffffff;
//   pcVar2 = string;
//   do {
//     if (uVar3 == 0) break;
//     uVar3 = uVar3 - 1;
//     cVar1 = *pcVar2;
//     pcVar2 = pcVar2 + (uint)bVar4 * -2 + 1;
//   } while (cVar1 != '\0');

// it's simpler to use

    i = strlen(buffer);
    buffer[i] = ' ';
    buffer[i + 1] = 0;
    pcVar2 = strcat(string,b);
    return pcVar2;
}


int main(void)
{
  char buffer [42];
// 080485aa 83 ec 40        SUB        ESP,0x40
// 080485ad 8d 44 24 16     LEA        EAX=>buffer,[ESP + 0x16]
  
  pp(buffer);
  puts(buffer);
  return 0;
}