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


extern struct hudebniNastroj * hudebniNastrojePole;
extern int soubory_pocetSouboru;
extern int soubory_aktualniNastroj;

short setupHudebniNastroje();

void setupKytara(void);

#endif