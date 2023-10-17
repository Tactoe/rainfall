class N {
  public:
  char annotation[100];
  int n;
  void __thiscall N::N(N *this,int i)
  {
    *(undefined ***)this = &PTR_operator+_08048848;
    *(int *)(this + 0x68) = i;
    return;
  }
  int __thiscall N::operator-(N *param_1)
  {
    return *(int *)(this + 0x68) - *(int *)(param_1 + 0x68);
  }
  int __thiscall N::operator+(N *param_1)
  {
    return *(int *)(param_1 + 0x68) + *(int *)(this + 0x68);
  }
  void setAnnotation(char *annotation) {
      __n = strlen(param_1);
    memcpy(this->annotation,annotation,__n);
  }

}

int main(int ac,char **av)
{
  N *nClass1;
  N *nClass2;
  int iVar1;
  
  if (ac < 2) {
    _exit(1);
  }
  nClass1 = (N *)operator.new(0x6c);
  N::N(nClass1,5);
  nClass2 = (N *)operator.new(0x6c);
  N::N(nClass2,6);
  N::setAnnotation(nClass1,av[1]);
// 0804867c 8b 44 24 10     MOV        nClass2,dword ptr [ESP + 0x10]=>local_20
// 08048680 8b 00           MOV        nClass2,dword ptr [nClass2]
// 08048682 8b 10           MOV        EDX,dword ptr [nClass2]
// 08048684 8b 44 24 14     MOV        nClass2,dword ptr [ESP + 0x14]=>local_1c
// 08048688 89 44 24 04     MOV        dword ptr [ESP + 0x4]=>local_2c,nClass2
// 0804868c 8b 44 24 10     MOV        nClass2,dword ptr [ESP + 0x10]=>local_20
// 08048690 89 04 24        MOV        dword ptr [ESP]=>local_30,nClass2
// 08048693 ff d2           CALL       EDX
// 08048695 8b 5d fc        MOV        EBX,dword ptr [EBP + -0x4]=>local_8
// 08048698 c9              LEAVE
// 08048699 c3              RET

  // is translated as

  return (***(code ***)nClass2)(nClass2,nClass1);
}

