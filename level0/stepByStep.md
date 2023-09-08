level0:
On voit un binaire, on l'ouvre avec un outil de reverse-engineering, on voit qu'il ouvre un shell d'un autre utilisateur si on lui donne la bonne valeur (423)
Donc il suffit de faire ./level0 423 et de cat le .pass de /home/user/level1/ qui est : 1fe8a524fa4bec01ca4ea2a869af2a02260d4a7d5fe7e7c24d8617e6dca12d3a