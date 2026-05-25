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
#include <unistd.h>
#include <string.h>

#define MAXIMALNI_DELKA_NAZVU_SEZNAMU_NASTROJU 20
#define MAXIMALNI_DELKA_NAZVU_TONU 20
#define MAXIMALNI_DELKA_FREKVENCE_V_CHARECH 5
#define MAXIMALNI_DELKA_POCTU_TONU_V_CHARECH 4  // počítám i s varhany
#define MAXIMALNI_POCET_TONU 300
#define MAXIMALNI_DELKA_NAZVU_NASTROJE 50

struct hudebniNastroj *poleHudebnichNastroju;
int pocetHudebnichNastroju;
int aktualniHudebniNastroj = 0;
char **nazvySouboru;

struct hudebniNastroj gitara;

void setupKytara(void);


// --------------------------------------------------------------------

void setupKytara(void)
{
    strcpy(gitara.nazev, "Kytara 6 strunná");
    gitara.pocetTonu = 6;
    gitara.poleTonu = malloc(sizeof(int) * gitara.pocetTonu);
    gitara.poleTonu[0] = 82;
    gitara.poleTonu[1] = 110;
    gitara.poleTonu[2] = 147;
    gitara.poleTonu[3] = 196;
    gitara.poleTonu[4] = 247;
    gitara.poleTonu[5] = 330;
    
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

void nacistJmenaSouboru(void)
{
    FILE *souborSNazvy = fopen("nastroje.lad", "r+");
    if (souborSNazvy == NULL)
    {
        // neco na logy to bude chtit
        return;
    }
    rewind(souborSNazvy);
    short kontrola_poctu = fscanf(souborSNazvy, "%i;", &pocetHudebnichNastroju); // TADY TAKY RADSI FGETS
    if (kontrola_poctu != 1)
    {
        pocetHudebnichNastroju = 0;
        nazvySouboru = NULL;
        return;
    }
    nazvySouboru = malloc(sizeof(char *) * pocetHudebnichNastroju);
    for (int indexStringu = 0; indexStringu < pocetHudebnichNastroju; indexStringu++)
    {
        nazvySouboru[indexStringu] = malloc(sizeof(char) * MAXIMALNI_DELKA_NAZVU_NASTROJE);
    }
    for (int _nazev_souboru_index_pro_nulovani = 0; _nazev_souboru_index_pro_nulovani < pocetHudebnichNastroju; _nazev_souboru_index_pro_nulovani++)
    {
        memset(nazvySouboru[_nazev_souboru_index_pro_nulovani], '\0', sizeof(char) * MAXIMALNI_DELKA_NAZVU_SEZNAMU_NASTROJU);
    }
    char docasny_uloziste_pro_Jmena[MAXIMALNI_DELKA_NAZVU_SEZNAMU_NASTROJU *  (pocetHudebnichNastroju)];
    fgets(docasny_uloziste_pro_Jmena, MAXIMALNI_DELKA_NAZVU_SEZNAMU_NASTROJU * (pocetHudebnichNastroju), souborSNazvy);
    for (int index_znaku = 0, index_souboru = 0, index_zacatku_nazvu = 0; index_znaku < strlen(docasny_uloziste_pro_Jmena); index_znaku++)
    {
        if (docasny_uloziste_pro_Jmena[index_znaku] == ';')
        {
            index_souboru++;
            index_zacatku_nazvu = 0;
            continue;
        }
        nazvySouboru[index_souboru][index_zacatku_nazvu] = docasny_uloziste_pro_Jmena[index_znaku];
        index_zacatku_nazvu++;
    }
    fclose(souborSNazvy);
    return;
}

void nacistNastrojeZeSouboru()
{
    poleHudebnichNastroju = malloc(sizeof(struct hudebniNastroj) * pocetHudebnichNastroju);
    if (poleHudebnichNastroju == NULL)
    {
        return;
    }
    memset(poleHudebnichNastroju, 0, sizeof(struct hudebniNastroj) * pocetHudebnichNastroju);
    for (int index_aktualniho_souboru = 0; index_aktualniho_souboru < pocetHudebnichNastroju; index_aktualniho_souboru++)
    {
        FILE * soubor_s_aktual_nastrojem = fopen(nazvySouboru[index_aktualniho_souboru], "r");
        if (soubor_s_aktual_nastrojem == NULL)
        {
            continue;
        }
        
        fclose(soubor_s_aktual_nastrojem);
    }
}

void nacitaniStrukturMain(void)
{
    // ziskame odkud cerpat informace

    nacistJmenaSouboru();
    nacistNastrojeZeSouboru();
}