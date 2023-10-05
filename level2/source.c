#include <stdio.h>
#include <stdlib.h>
#include <string.h>

void p(void) {
    unsigned int addressPointer;
    char buffer[64];

    fflush(stdout);
	gets(buffer);
	addressPointer = (unsigned int)__builtin_return_address(0);
    
    if ((addressPointer & 0xb0000000)  == 0xb0000000) {
        printf ("(%p)\n", addressPointer);
        exit (1);
    }
    puts(buffer);
    strdup(buffer);
}

int	main(void)
{
	p();
	return 0;
}