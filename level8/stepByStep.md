L'exécutable écoute sur le stdin et affiche (nil), (nil).
Si on l'observe avec gdb on remarque qu'il y a une boucle qui printf 2 adresse de 2 variable ( auth et service ).

Ensuite fgets est utilisé pour écouter sur le stdin puis le retour est comparé à des strings afin d'exécuter la liste d'instruction associée à la string envoyée en stdin. En regardant les str utilisées pour les comparer à notre input nous en avons 4

- auth :
  - un malloc est créé pour pouvoir ensuite copier la chaine dans notre variable auth via un strcpy.
- reset
  - Va appeler la fonction free sur notre variable auth
- service
  - Va utiliser strdup afin de copier notre chaine pour la stocker en mémoire
- login
  - Va comparer si la variable auth (adresse 0x8049aac) est égale à l'adresse stockée en mémoire pour notre string auth + un offset de 0x20 est = 0 ( donc si notre adresse en memoire est 0x804a008 alors login regardera à l'emplacement 0x804a028 )

Il est important que la variable auth soit initialisée sinon notre exécutable crash. 
Si c'est 0 alors la fonction reboucle sinon un appel system est lancé avec comme argument /bin/sh

Le probleme dans cet exécutable est que si auth est déclaré dans un premier temps et que ensuite service est utilisé derriere on peux voir que les 2 adresses se suivent en mémoire. Sachant que la variable service n'est jamais free on peut créer plusieurs chaine en mémoire grace au strdup.
Donc si on essaie de stocker auth puis de stocker 2 adresses via le strdup de `service` on peux logiquement écrire à l'emplacement auth+0x20
car dans notre cas chaque appel à strdup va écrire 0x10 plus loin que notre premier appel à malloc.

Voici une trace de la mémoire apres un appel à auth avec ("auth test")
```
gdb-peda$ x/16xw 0x804a008
0x804a008:	0x74736574	0x0000000a	0x00000000	0x00020ff1
0x804a018:	0x00000000	0x00000000	0x00000000	0x00000000
0x804a028:	0x00000000	0x00000000	0x00000000	0x00000000
0x804a038:	0x00000000	0x00000000	0x00000000	0x00000000
```

0x804a008 étant l'adresse renvoyée par le printf nous indiquant l'emplacement en mémoire de notre string

Pour rappel login va vérifier si "0x804a028:      0x00000000" vaut 0

Maintenant si on utilisee service avec ("service ok") la mémoire ressemble à
```
gdb-peda$ x/16 0x804a008
0x804a008:	0x74736574	0x0000000a	0x00000000	0x00000011
0x804a018:	0x0a6b6f20	0x00000000	0x00000000	0x00020fe1
0x804a028:	0x00000000	0x00000000	0x00000000	0x00000000
0x804a038:	0x00000000	0x00000000	0x00000000	0x00000000
```

On voit clairement que les chaines se suivent et si on continue une nouvelle fois avec service
```
gdb-peda$ x/16 0x804a008
0x804a008:	0x74736574	0x0000000a	0x00000000	0x00000011
0x804a018:	0x0a6b6f20	0x00000000	0x00000000	0x00000011
0x804a028:	0x0a6b6f20	0x00000000	0x00000000	0x00020fd1
0x804a038:	0x00000000	0x00000000	0x00000000	0x00000000
```
Ici "0x804a028:      0x0a6b6f20" Donc "0x804a008+0x20" ne vaut plus 0 et on peut maintenant essayer d'utiliser login pour avoir notre shell

Pour résumer, il suffit de lancer le binaire et d'entrer les strings suivantes dans l'ordre (le deuxieme argument peut etre n'importe quoi) :
- auth a
- service a
- service a
- login
