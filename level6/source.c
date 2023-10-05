#include <stdio.h>
#include <stdlib.h>
#include <string.h>

void n(void) {
    system("/bin/cat /home/user/level7/.pass");
}

void m(void) {
    puts("Nope");
}

int main(int ac, char **av) {
    char *a;
    void *b;
    a = malloc(64);
    b = malloc(4);
    *b = (void *)m;
    strcpy(a, av[1]);
    (**b)();
    return 0;
}