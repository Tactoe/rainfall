Quand on reverse l'exécutable, on voit que main appel la fonction n qui appel la fonction p.


n() appel fgets() qui recupere de maniere safe notre input.
puis dans la fonction notre input est utilisé en tant que format string pour printf
Ce qui en fait une vulnérabilité.
Après la fonction p, une variable à l'adresse 0x8049810 est stockée dans eax puis est comparée à la valeur 0x1025544 (16930116 en décimal).
Si eax est égal a cette valeur, alors `system()` est executé avec comme argument:
`0x8048590 = "/bin/cat /home/user/level5/.pass"`

Il faut donc que l'on arrive à modifier la variable à l'adresse 0x8049810 pour qu'elle contienne 0x1025544
Après quelque test on remarque que 0x8049810 vaut 0.

Grâce à la vulnérabilité du printf on va pouvoir utiliser une format string attack afin de modifier la valeur de m
Pour ce faire nous avons besoin de l'emplacement de notre buffer dans la stack au moment ou printf est exécuté (l'offset)


`python -c "print('AAAA' + ' %p '*20)" | ./level4`

Le flag %p va print l'adresse d'un élément de la stack, en le faisant plusieurs fois on va "remonter" le long de la stack. Il faut juste trouver quelle adresse est égale à 41414141
On retient sa position qui est 12 et on vérifie si le 12e emplacement est bien notre buffer
( '%12\$x' signifie que l'on souhaite utiliser le 12ème argument de printf, donc le 12ème élément sur la stack )

`python -c "print('AAAA' + '%12\$x')" | ./level4`

Maintenant que nous avons l'offset de notre buffer on peut passer a la suite et essayer de changer des valeurs.

On souhaite écrire l'adresse de m dans notre buffer pour ensuite utiliser le format %n afin d'ecrire le nombre de caractere ecrit avant notre format %n dans notre variable m.
---> ("coucou%n", &nb)  --> nb = 6

1ere etape:
python -c "print('\x10\x98\x04\x08' + '%12\$x')" | ./level4
cela devrait écrire notre adresse dans le 12eme argument.
Il manque plus qu'a ecrire X caractere et d'utiliser %n.

m doit avoir la valeur 0x1025544 ce qui correspond a 16930116 en decimal

On pourrait utiliser quelque chose comme ceci:
`python -c "print('\x10\x98\x04\x08' + 'B'*16930116 + '%12\$n')" | ./level4`

Mais la taille de notre buffer nous bloquerait pour écrire autant de B. Ce que nous pouvons utiliser est la width de printf. Si on met un format %d avec une width de 500:

%500d, printf ajoutera bien 500 caractere a notre string.

Donc si on souhaite ecrire 0x1025544 dans notre variable m il suffit d'ecrire %16930116d

Mais attention car on ecrit deja 4byte donc on le soustrait a notre valeur ce qui donne %16930112d

`python -c "print('\x10\x98\x04\x08'+'%16930112d' + '%12\$n')" | ./level4`
