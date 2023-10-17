class N {
  public:
  char annotation[100];
  int n;
  int (N::*operatorFunctionPointer)(N &);
  N(int n)
	{
    this->n = n;
		this->operatorFunctionPointer = &N::operator+;
	}
  int operator-(N &otherN)
  {
    return this->n - otherN.n;
  }
  int operator+(N &otherN)
  {
    return otherN.n + this->n;
  }
  void setAnnotation(char *annotation) {
    memcpy(this->annotation,annotation,strlen(annotation));
  }
}

int main(int ac,char **av)
{
  N *nClass1;
  N *nClass2;
  N *nPointer1;
  N *nPointer2;
  
  if (ac <= 1) {
    _exit(1);
  }
  nClass1 = new N(5);
  nClass2 = new N(6);
  nPointer1 = nClass1;
  nPointer2 = nClass2;
  nClass1->setAnnotation(av[1]);
  return (nPointer2->*nPointer2->operatorFunctionPointer)(*nClass2);
}

