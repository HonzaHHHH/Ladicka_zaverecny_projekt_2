/*
    Tato část kódu složí k načítání struktur hudebních nástrojů ze souborů
    !!!! POZOR !!!!!
    Tato část obsahuje velké zbytky po prvním testování, kód je psán velmi jednoduše, abych se mohl soustředit na hlavní problematiku souboru a ne na blbý struktury
*/

#include <stdlib.h>
#include <nacitaniStruktur.h>
#include <stdio.h>
#include <string.h>

#define MAXIMALNI_DELKA_SEZNAMU_NASTROJU 20

struct hudebniNastroj gitara;
void odstranitPodtrzitkaZNastroje(struct hudebniNastroj *nastroj);
void tvorbaPoleUlozenychNastroju(char **nazvy, int *pocet);

void setupKytara(void)
{
    gitara.pocetTonu = 6;
    gitara.poleTonu = malloc(sizeof(int) * gitara.pocetTonu);
    gitara.poleTonu[0] = 82;
    gitara.poleTonu[1] = 110;
    gitara.poleTonu[2] = 147;
    gitara.poleTonu[3] = 196;
    gitara.poleTonu[4] = 247;
    gitara.poleTonu[5] = 330;
    strcpy(gitara.nazev, "Kytara 6 strunná");
    gitara.nazvyTonu = malloc(sizeof(char *) * gitara.pocetTonu);
    for (int i = 0; i < gitara.pocetTonu; i++)
    {
        gitara.nazvyTonu[i] = malloc(sizeof(char) * 10);
    }
    strcpy(gitara.nazvyTonu[0], "Struna E2");
    strcpy(gitara.nazvyTonu[1], "Struna A2");
    strcpy(gitara.nazvyTonu[2], "Struna D3");
    strcpy(gitara.nazvyTonu[3], "Struna G3");
    strcpy(gitara.nazvyTonu[4], "Struna B3");
    strcpy(gitara.nazvyTonu[5], "Struna E4");
}

void setupHudebniNastroje() {

}

void tvorbaPoleUlozenychNastroju(char **nazvy, int *pocet)
{ // souor seznam nastroju bude obsahovat pocet hudebnich nastroju a nazvy jednotlivych souboru s daty nastroje
    FILE *seznamNastroju = fopen("seznamNatroju.lad", "r+");
    if (seznamNastroju == NULL)
    {
        nazvy = NULL;
        pocet = 0;
        return;
    }
    rewind(seznamNastroju);

    fscanf(seznamNastroju, "%i\n", pocet);
    nazvy = malloc(*pocet * MAXIMALNI_DELKA_SEZNAMU_NASTROJU * sizeof(char));
    fclose(seznamNastroju);
}

void setupHudebniNastroj(struct hudebniNastroj *nastroj, char *nazevSouboru)
{
    FILE *souborNastroje = fopen(nazevSouboru, "r+");
    if (souborNastroje == NULL)
    {
        souborNastroje = NULL;
        return;
    }
    rewind(souborNastroje);
    int pocetTonu;
    fscanf(souborNastroje, "%i\n", &pocetTonu);
    nastroj->pocetTonu = pocetTonu;
    nastroj->poleTonu = malloc(sizeof(int) * pocetTonu);
    for (int i = 0; i < nastroj->pocetTonu; i++)
    {
        fscanf(souborNastroje, "%i\n", &nastroj->poleTonu[i]);
    }
    nastroj->nazvyTonu = malloc(sizeof(char *) * nastroj->pocetTonu);
    for (int i = 0; i < nastroj->pocetTonu; i++)
    {
        fscanf(souborNastroje, "%s\n", nastroj->nazvyTonu[i]);

    }
    fscanf(souborNastroje, "%s\n", nastroj->nazev);
    odstranitPodtrzitkaZNastroje(nastroj);
    fclose(souborNastroje);
}

void odstranitPodtrzitkaZNastroje(struct hudebniNastroj *nastroj) {

}