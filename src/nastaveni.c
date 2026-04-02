#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <nastaveni.h>

short nastaveni = 0;
int vzorkovaciFrekvence = 44100;
int vzorkuNaBuffer = 256;
double cisloPi = 3.14159265358979323846264338327950288419716939937510f; // převzato z Wikipedie: https://cs.wikipedia.org/wiki/P%C3%AD_(%C4%8D%C3%ADslo)

void nastaveniNastaveni();

void praceSParametrem(char *parametry) {
    if (strcmp(parametry, "--ignsys") == 0) // pro ignoraci typu systému
    {
        nastaveni |= (1 << NASTAVENI_IGNSYS);
    }
    if (strcmp(parametry, "--dipp") == 0) // dialog ++
    {
        nastaveni |= (1 << NASTAVENI_DIALOGPLUS);
    }
    
}