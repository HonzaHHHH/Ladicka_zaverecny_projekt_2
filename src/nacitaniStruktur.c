/*
    Tato část kódu složí k načítání struktur hudebních nástrojů ze souborů
    !!!! POZOR !!!!!
    Tato část obsahuje velké zbytky po prvním testování, kód je psán velmi jednoduše, abych se mohl soustředit na hlavní problematiku souboru a ne na blbý struktury
*/

/*

Struktura souboru pro ukladani jednotlivych souboru





*/

#include <stdlib.h>
#include <nacitaniStruktur.h>
#include <stdio.h>
#include <string.h>

#define MAXIMALNI_DELKA_SEZNAMU_NASTROJU 20

struct hudebniNastroj * hudebniNastrojePole = NULL;
char ** soubory_nazvySouboru;
int soubory_pocetSouboru;
int soubory_aktualniNastroj = 0;

struct hudebniNastroj gitara;
void odstranitPodtrzitkaZNastroje(struct hudebniNastroj *nastroj);
void tvorbaPoleUlozenychNastroju(char ***nazvy, int *pocet);
void setupKytara(void);
short setupHudebniNastroje(void);
int setupHudebniNastroj(struct hudebniNastroj *nastroj, char *nazevSouboru);


// --------------------------------------------------------------------

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

/**
 * nastaví vsechny hudební nástroje
 */
short setupHudebniNastroje() {
    tvorbaPoleUlozenychNastroju(&soubory_nazvySouboru, &soubory_pocetSouboru); // ziska názvy souborů
    if (soubory_nazvySouboru == NULL || soubory_pocetSouboru <= 0)
    {
        return 1;
    }
    hudebniNastrojePole = malloc(soubory_pocetSouboru * sizeof(struct hudebniNastroj)); // alokuje misto pro soubory
    if (hudebniNastrojePole == NULL)
    {
        return 1;
    }
    for (int i = 0; i < soubory_pocetSouboru; i++)
    {
        if (!setupHudebniNastroj(&hudebniNastrojePole[i], soubory_nazvySouboru[i]))
        {
            for (int j = 0; j < i; j++)
            {
                free(hudebniNastrojePole[j].poleTonu);
                for (int k = 0; k < hudebniNastrojePole[j].pocetTonu; k++)
                    free(hudebniNastrojePole[j].nazvyTonu[k]);
                free(hudebniNastrojePole[j].nazvyTonu);
            }
            free(hudebniNastrojePole);
            hudebniNastrojePole = NULL;
            return 1;
        }
    }
    return 0;
}


/**
 * uvolní paměť
 */
short konecHudebnichNastroju(void)
{
    

}

void tvorbaPoleUlozenychNastroju(char ***nazvy, int *pocet)
{ // souor seznam nastroju bude obsahovat pocet hudebnich nastroju a nazvy jednotlivych souboru s daty nastroje
    FILE *seznamNastroju = fopen("seznamNastroju.lad", "r");
    if (seznamNastroju == NULL)
    {
        *nazvy = NULL;
        *pocet = 0;
        return;
    }
    rewind(seznamNastroju);
    if (fscanf(seznamNastroju, "%i\n", pocet) != 1 || *pocet <= 0)
    {
        *nazvy = NULL;
        *pocet = 0;
        fclose(seznamNastroju);
        return;
    }
    *nazvy = malloc(*pocet * sizeof(char *));
    if (*nazvy == NULL)
    {
        *pocet = 0;
        fclose(seznamNastroju);
        return;
    }
    for (int i = 0; i < *pocet; i++)
    {
        (*nazvy)[i] = malloc(MAXIMALNI_DELKA_SEZNAMU_NASTROJU);
        if ((*nazvy)[i] == NULL || fscanf(seznamNastroju, "%19s\n", (*nazvy)[i]) != 1)
        {
            for (int j = 0; j <= i; j++)
                free((*nazvy)[j]);
            free(*nazvy);
            *nazvy = NULL;
            *pocet = 0;
            fclose(seznamNastroju);
            return;
        }
    }
    fclose(seznamNastroju);
}


/**
 * nastaveni konkretniho nastroje
 * 
*/
int setupHudebniNastroj(struct hudebniNastroj *nastroj, char *nazevSouboru)
{
    FILE *souborNastroje = fopen(nazevSouboru, "r");
    if (souborNastroje == NULL)
    {
        nastroj->pocetTonu = 0;
        nastroj->poleTonu = NULL;
        nastroj->nazvyTonu = NULL;
        nastroj->nazev[0] = '\0';
        return 0;
    }
    rewind(souborNastroje);
    int pocetTonu;
    if (fscanf(souborNastroje, "%i\n", &pocetTonu) != 1 || pocetTonu <= 0)
    {
        nastroj->pocetTonu = 0;
        nastroj->poleTonu = NULL;
        nastroj->nazvyTonu = NULL;
        nastroj->nazev[0] = '\0';
        fclose(souborNastroje);
        return 0;
    }
    nastroj->pocetTonu = pocetTonu;
    nastroj->poleTonu = malloc(sizeof(int) * pocetTonu);
    if (nastroj->poleTonu == NULL)
    {
        fclose(souborNastroje);
        nastroj->pocetTonu = 0;
        nastroj->nazvyTonu = NULL;
        nastroj->nazev[0] = '\0';
        return 0;
    }
    for (int i = 0; i < nastroj->pocetTonu; i++)
    {
        if (fscanf(souborNastroje, "%i\n", &nastroj->poleTonu[i]) != 1)
        {
            free(nastroj->poleTonu);
            nastroj->pocetTonu = 0;
            nastroj->poleTonu = NULL;
            nastroj->nazvyTonu = NULL;
            nastroj->nazev[0] = '\0';
            fclose(souborNastroje);
            return 0;
        }
    }
    nastroj->nazvyTonu = malloc(sizeof(char *) * nastroj->pocetTonu);
    if (nastroj->nazvyTonu == NULL)
    {
        free(nastroj->poleTonu);
        nastroj->pocetTonu = 0;
        nastroj->poleTonu = NULL;
        nastroj->nazev[0] = '\0';
        fclose(souborNastroje);
        return 0;
    }
    for (int i = 0; i < nastroj->pocetTonu; i++)
    {
        nastroj->nazvyTonu[i] = malloc(50);
        if (nastroj->nazvyTonu[i] == NULL || fscanf(souborNastroje, "%49s\n", nastroj->nazvyTonu[i]) != 1)
        {
            for (int j = 0; j <= i; j++)
            {
                free(nastroj->nazvyTonu[j]);
            }
            free(nastroj->nazvyTonu);
            free(nastroj->poleTonu);
            nastroj->pocetTonu = 0;
            nastroj->poleTonu = NULL;
            nastroj->nazvyTonu = NULL;
            nastroj->nazev[0] = '\0';
            fclose(souborNastroje);
            return 0;
        }
    }
    if (fscanf(souborNastroje, "%49s\n", nastroj->nazev) != 1)
    {
        for (int i = 0; i < nastroj->pocetTonu; i++)
            free(nastroj->nazvyTonu[i]);
        free(nastroj->nazvyTonu);
        free(nastroj->poleTonu);
        nastroj->pocetTonu = 0;
        nastroj->poleTonu = NULL;
        nastroj->nazvyTonu = NULL;
        nastroj->nazev[0] = '\0';
        fclose(souborNastroje);
        return 0;
    }
    odstranitPodtrzitkaZNastroje(nastroj);
    fclose(souborNastroje);
    return 1;
}

void odstranitPodtrzitkaZNastroje(struct hudebniNastroj *nastroj) {

}