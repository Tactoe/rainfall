En regardant les fonctions dans l'exécutable via gdb et info functions on peut voir qu'il existe n et m en plus de main.
Quand on désassemble main, plusieurs call sont effectués.
2 appel à malloc pour préparer à l'appel de strcpy, puis un appel à eax qui contient ceci:
0x8048468 (<m>:	push   ebp)

La fonction m écrit sur stdout "Nope". La fonction n lance un appel système avec pour argument "/bin/cat /home/user/level7/.pass"

L'exécutable attend un argument en paramètres qui est copié dans un buffer.
Le programme segfault si l'argument est gros, donc nous pouvons utiliser un buffer overflow afin d'overwrite EIP pour appeler la fonction n ( qui n'est pas appelée de base )

On commence par trouver l'offset afin d'overwrite EIP.
Grace a des générateur de pattern on trouve que le buffer overflow a partir de 72 caractères.

Donc si on écrit 72 caractères puis l'address de la fonction n alors elle sera exécutée.
L'addresse de n est 0x08048454, on l'encode en little endian et c'est bon.

./level6 `python -c "print('A'*72 + '\x54\x84\x04\x08')"`
