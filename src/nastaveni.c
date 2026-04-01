#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <nastaveni.h>

void nastaveniNastaveni();

void praceSParametrem(char *parametry) {
    if (strcmp(parametry, "--ignsys") == 0) // pro ignoraci typu systému
    {
        nastaveni |= (1 << 1);
    }
    
}