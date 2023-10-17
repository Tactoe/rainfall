#include <stdio.h>
#include <stdlib.h>

int main(int ac,char **av)
{
  int i;
  char buffer [132];
  FILE *filePointer;
  
  filePointer = fopen("/home/user/end/.pass","r");

// 0804851f 8d 5c 24 18     LEA        EBX=>buffer,[ESP + 0x18]
// 08048523 b8 00 00        MOV        EAX,0x0
//          00 00
// 08048528 ba 21 00        MOV        EDX,0x21
//          00 00
// 0804852d 89 df           MOV        EDI,EBX
// 0804852f 89 d1           MOV        ECX,EDX
// 08048531 f3 ab           STOSD.REP  ES:EDI
// https://stackoverflow.com/questions/3818856/what-does-the-rep-stos-x86-assembly-instruction-sequence-do
// For ecx repetitions, stores the contents of eax into where edi points to, incrementing or decrementing edi (depending on the direction flag) by 4 bytes each time.
// So 4 * 0x21 (33) = 132, the array size is 132

// this part of the code is equivalent to a memset. Ghidra translation gives us
//   byte bVar3;
//   undefined4 *puVar2;
//   puVar2 = (undefined4 *)buffer;
//   for (i = 0x21; i != 0; i = i + -1) {
//     *puVar2 = 0;
//     puVar2 = puVar2 + (uint)bVar3 * -2 + 1;
//   }

// but it's simpler to say
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
