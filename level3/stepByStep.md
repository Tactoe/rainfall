level3:
On voit un binaire, on l'ouvre avec un outil de reverse-engineering, on voit qu'il fait appel a la fonction v qui ouvre un shell admin si un int correspond a la bonne valeur (64)
On ouvre gdb, on disass la fonction v(), on met un breakpoint juste avant la comparaison (cmp 0x080484df) sur le mov (b *0x080484da). On lance avec r, une fois arrive au breakpoint on fait stepi, puis set $eax=64, puis c. On entre dans le shell admin et on peut cat le password


PLOT TWIST:
gdb doit être lancé en root pour que ça ouvre un shell admin sinon ça serait un peu facile niveau sécurité haha oups; donc non.

On voit que la comparaison est faite par rapport à une variable globale m qui se trouve à l'adresse 0x0804988c (`info variables` dans gdb ou bien `disass v` et les lignes `0x080484da <+54>:    mov    0x804988c,%eax` `0x080484df <+59>:    cmp    $0x40,%eax`). Si on arrive à override ce qui se trouve à cet endroit on pourra accéder au shell admin pour de vrai.

On a fgets qui prend un input mais qui contrairement a gets est sécurisé et empêche les buffers overflow. Par contre printf est vulnérable aux Format string attack https://owasp.org/www-community/attacks/Format_string_attack. On peut utiliser le flag %n, qui stocke tous les caractères écrits par printf jusque-là dans  pour override la memoire a l'emplacement 0x0804988c. Instead of being replaced by a function argument, %n will cause the number of characters written so far to be stored into the corresponding function argument.




0x080484b6 <+18>:    movl   $0x200,0x4(%esp)
    donc buffer 512
\x8c\x98\x04\x08