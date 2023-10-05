level1:
le dossier contient un fichier binaire, en l'analysant avec un outil de reverse-engineering on voit qu'il utilise gets() qui est une fonction unsafe. Il y a également une fonction run() qui exécute un shell admin. On voit que le binaire crashe au bout d'un certain nombre de caracteres (76). Avec ce buffer overflow, il est possible d'override EIP (qui pointe sur le set suivant d'instructions à exécuter une fois au bous de la fonction) avec l'adresse de run() pour lancer un shell.

On ouvre le binaire avec gdb, `info functions` nous indique que l'adresse mémoire de run est 0x08048444. Donc si on donne une string de 76 caractères au programme, avec juste après l'adresse mémoire de run() en little endian (\x44\x84\x04\x08) le programme va l'exécuter. Le seul problème est que cette string peut uniquement être envoyée en pipe, et qu'un shell ouvert dans un pipe n'est pas interactif (il s'ouvre, lit stdin et voit EOF puisque notre input a été "mangé")

Si on utilise la commande 
`(python -c "print 'A' * 76 + '\x44\x84\x04\x08'"; echo 'cat /home/user/level2/.pass') | ./level1`
La deuxième ligne est lue par le shell admin une fois active, et va nous print le pass du level2

ANNEXE: pour savoir la taille du buffer, on peut utiliser gdb et analyser main: d'abord on utilise disass main. On voit deux lignes intéressantes :
0x08048486 <+6>:     sub    $0x50,%esp
0x08048489 <+9>:     lea    0x10(%esp),%eax
On a 0x50 bytes (80 en décimal) alloués pour le buffer. Ensuite, l'adresse esp+0x10 est stockée dans eax pour que gets() l'utilise. Donc la taille du buffer est 0x50 - 0X10 = 0x40 soit 64