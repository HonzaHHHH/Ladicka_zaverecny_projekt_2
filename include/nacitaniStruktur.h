#ifndef _NACITANI_STRUKTUR_H
#define _NACITANI_STRUKTUR_H

struct hudebniNastroj
{
    int pocetTonu;
    int * poleTonu;
    char nazev[50];
    char ** nazvyTonu;
};

extern struct hudebniNastroj gitara;

extern struct hudebniNastroj * poleNastroju;

void setupKytara(void);

#endif