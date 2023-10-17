bonus3:

Le binaire lit le password de end, et continue l'exéution uniquement si le password existe et que argv = 2
Il effectue un strcmp entre le buffer créé et av[1], et exécute un shell admin si strcmp == 0 (donc les strings sont identiques)
On remarque qu'il effectue un atoi() sur av[1] pour set `buffer[atoi(av[1])] = '\0'`
Si on entre une string vide:

`./bonus3 ""`

atoi va return 0, on aura donc buffer[0] = '\0' et av[1][0] = '\0'
Comme strcmp s'arrête au premier \0 rencontré, les deux strings sont considérées identiques et on gagne accès au shell admin