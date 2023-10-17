char *auth = NULL;
char *service = NULL;

int main(void)

{
  char *pcVar2;
  uint uVar4;
  bool bVar7;
  undefined uVar9;
  bool bVar10;
  uint uVar11;
  char buffer [128];
  
  do {
    printf("%p, %p \n",auth,service);
    pcVar2 = fgets(buffer,0x80,stdin);
    if (pcVar2 == 0) {
      return 0;
    }
// 080485c6 b9 05 00        MOV        ECX,0x5
// 080485cb 89 d6           MOV        ESI,EDX
// 080485cd 89 c7           MOV        EDI,EAX
// 080485cf f3 a6           CMPSB.REPE ES:EDI=>s_auth_08048819,ESI                      = "auth "

// translate into

//   byte bVar12;
//   bVar12 = 0;
//   int iVar3;
//   byte *pbVar5;
//   byte *pbVar6;
    // iVar3 = 5;
    // pbVar5 = (byte *)buffer;
    // pbVar6 = (byte *)"auth ";
    // do {
    //   if (iVar3 == 0) break;
    //   iVar3 = iVar3 + -1;
    //   bVar7 = *pbVar5 < *pbVar6;
    //   bVar10 = *pbVar5 == *pbVar6;
    //   pbVar5 = pbVar5 + (uint)bVar12 * -2 + 1;
    //   pbVar6 = pbVar6 + (uint)bVar12 * -2 + 1;
    // } while (bVar10);

// it's easier to say

    uvar11 = strncmp(buffer, "auth ", 5);

//   undefined uVar8;
    // uVar8 = 0;
    if (uvar11 == 0) {
      auth = malloc(4);

// 08048616 8b 4c 24 1c     MOV        ECX,dword ptr [ESP + 0x1c]=>local_94
// 0804861a 89 d7           MOV        EDI,EDX
// 0804861c f2 ae           SCASB.RE   ES:EDI
// 0804861e 89 c8           MOV        EAX,ECX
// 08048620 f7 d0           NOT        EAX
// 08048622 83 e8 01        SUB        EAX,0x1
// 08048625 83 f8 1e        CMP        EAX,0x1e
// 08048628 77 18           JA         LAB_08048642
// 0804862a 8d 44 24 20     LEA        EAX=>buffer,[ESP + 0x20]
// 0804862e 8d 50 05        LEA        EDX=>buffer[5],[EAX + 0x5]
// 08048631 a1 ac 9a        MOV        EAX,[auth]
// 08048636 89 54 24 04     MOV        dword ptr [ESP + 0x4]=>local_ac,EDX
// 0804863a 89 04 24        MOV        dword ptr [ESP]=>local_b0,EAX
// 0804863d e8 1e fe        CALL       <EXTERNAL>::strcpy                               char * strcpy(char * __dest, cha

//translates into

//   char cVar1;
    //   uVar4 = 0xffffffff;
    //   pcVar2 = buffer + 5;
    //   do {
    //     if (uVar4 == 0) break;
    //     uVar4 = uVar4 - 1;
    //     cVar1 = *pcVar2;
    //     pcVar2 = pcVar2 + (uint)bVar12 * -2 + 1;
    //   } while (cVar1 != '\0');
    //   uVar4 = ~uVar4 - 1;
    //   uVar8 = uVar4 < 0x1e;
    //   uVar11 = uVar4 == 0x1e;
    //   if (uVar4 < 0x1f) {
    //     strcpy((char *)auth,buffer + 5);
    //   }


// it's easier to say
     if (strlen(buffer + 5) < 31) {
        strcpy(auth,buffer + 5);
     }
    }

    // iVar3 = 5;
    // pbVar5 = (byte *)buffer;
    // pbVar6 = (byte *)"reset";
    // do {
    //   if (iVar3 == 0) break;
    //   iVar3 = iVar3 + -1;
    //   uVar8 = *pbVar5 < *pbVar6;
    //   uVar11 = *pbVar5 == *pbVar6;
    //   pbVar5 = pbVar5 + (uint)bVar12 * -2 + 1;
    //   pbVar6 = pbVar6 + (uint)bVar12 * -2 + 1;
    // // } while ((bool)uVar11);
    // uVar9 = 0;
    // uVar8 = (!(bool)uVar8 && !(bool)uVar11) == (bool)uVar8;
    
    // easier to say
    
    uvar8 = strncmp(buffer, "reset", 5);
    if (uVar8 == 0) {
      free(auth);
    }

    // iVar3 = 6;
    // pbVar5 = (byte *)buffer;
    // pbVar6 = (byte *)"service";
    // do {
    //   if (iVar3 == 0) break;
    //   iVar3 = iVar3 + -1;
    //   uVar9 = *pbVar5 < *pbVar6;
    //   uVar8 = *pbVar5 == *pbVar6;
    //   pbVar5 = pbVar5 + (uint)bVar12 * -2 + 1;
    //   pbVar6 = pbVar6 + (uint)bVar12 * -2 + 1;
    // } while ((bool)uVar8);
    // uVar11 = 0;
    // uVar8 = (!(bool)uVar9 && !(bool)uVar8) == (bool)uVar9;

    // easier to say
    uvar8 = strncmp(buffer, "service", 6);
    if (uvar8 == 0) {
    //   uVar11 = (char *)0xfffffff8 < buffer;
    //   uVar8 = buffer + 7 == (char *)0x0;
      service = strdup(buffer + 7);
    }
    // iVar3 = 5;
    // pbVar5 = (byte *)buffer;
    // pbVar6 = (byte *)"login";
    // do {
    //   if (iVar3 == 0) break;
    //   iVar3 = iVar3 + -1;
    //   uVar11 = *pbVar5 < *pbVar6;
    //   uVar8 = *pbVar5 == *pbVar6;
    //   pbVar5 = pbVar5 + (uint)bVar12 * -2 + 1;
    //   pbVar6 = pbVar6 + (uint)bVar12 * -2 + 1;
    // } while ((bool)uVar8);
    
    //easier to say

    uvar8 = strncmp(buffer, "login", 5);

    if (uvar8 == 0) {
      if ((*(int *)(auth + 0x20) == 0)) {
        fwrite("Password:\n",1,10,stdout);
      }
      else {
        system("/bin/sh");
      }
    }
  } while( true );
}
