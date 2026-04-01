#ifndef _NACITANI_STRUKTUR_H
#define _NACITANI_STRUKTUR_H

struct hudebniNastroj
{
    int pocetTonu;
    int * poleTonu;
};

extern struct hudebniNastroj gitara;

void setupKytara(void);

#endif