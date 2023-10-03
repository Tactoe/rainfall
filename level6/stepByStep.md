En regardant les fonctions dans l'executable via gdb et info functions on peut voir qu'il existe n et m en plus de main.
Quand on desassemble main, plusieur call sont effectuer.
2 appel a malloc pour preparer a l'appel de strcpy, puis un appel a eax qui contient ceci:
0x8048468 (<m>:	push   ebp)

la fonction m ecrit sur stdout "Nope".
la fonction n lance un appel system avec pour argument "/bin/cat /home/user/level7/.pass"

L'executable attend un argument en parametres qui est copier dans un buffer.
Le programme segfault si l'argument est gros, donc nous pouvons utiliser un buffer overflow afin d'overwrite EIP pour appeler la fonction n ( qui n'est pas appeler de base )

On commence par trouver l'offset afin d'overwrite EIP.
Grace a des generateur de pattern on trouve que le buffer overflow a partir de 72 caracteres.

Donc si on ecrit 72 caracteres puis l'address de la fonction n alors elle sera executer.
L'addresse de n est 0x08048454

./level6 `python -c "print('A'*72 + '\x54\x84\x04\x08')"`
