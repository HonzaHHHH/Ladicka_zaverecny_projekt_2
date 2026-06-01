#ifndef _NACITANI_STRUKTUR_H
#define _NACITANI_STRUKTUR_H


/**
 * Makra, co se mi hodí
 */

#define MAXIMALNI_DELKA_NAZVU_SEZNAMU_NASTROJU 20
#define MAXIMALNI_DELKA_NAZVU_TONU 20
#define MAXIMALNI_DELKA_FREKVENCE_V_CHARECH 5
#define MAXIMALNI_DELKA_POCTU_TONU_V_CHARECH 4 // počítám i s varhany
#define MAXIMALNI_POCET_TONU 3000
#define MAXIMALNI_DELKA_NAZVU_NASTROJE 50

#define DELKA_FORMÁTOVÉ_SPECIFIKACE 7


/**
 * Struktura Hudební nástroj - v tomto formátu pak jsou veškerá data potřebná pro běh programu
 */
struct hudebniNastroj
{
    int pocetTonu;
    int * poleTonu;
    char nazev[50];
    char ** nazvyTonu;
};

extern struct hudebniNastroj *poleHudebnichNastroju;
extern int pocetHudebnichNastroju;
extern int aktualniHudebniNastroj;

void nacitaniStrukturMain(void);

void uvolnitVsechnoNaKonec(void);
#endif