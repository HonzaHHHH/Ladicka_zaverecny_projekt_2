/*
    Tato část kódu složí k načítání struktur hudebních nástrojů ze souborů
    !!!! POZOR !!!!!
    Tato část obsahuje velké zbytky po prvním testování, kód je psán velmi jednoduše, abych se mohl soustředit na hlavní problematiku souboru a ne na blbý struktury
*/

/*

Struktura souboru pro ukladani jednotlivych souboru
UZ FUNGUJJEEEEEEEEE




*/

#include <stdlib.h>
#include <nacitaniStruktur.h>
#include <stdio.h>
#include <unistd.h>
#include <string.h>

#define MAXIMALNI_DELKA_NAZVU_SEZNAMU_NASTROJU 20
#define MAXIMALNI_DELKA_NAZVU_TONU 20
#define MAXIMALNI_DELKA_FREKVENCE_V_CHARECH 5
#define MAXIMALNI_DELKA_POCTU_TONU_V_CHARECH 4 // počítám i s varhany
#define MAXIMALNI_POCET_TONU 3000
#define MAXIMALNI_DELKA_NAZVU_NASTROJE 50

#define DELKA_FORMÁTOVÉ_SPECIFIKACE 7

// tyto 2 makra mi pomohla gemini s vytvorenim a implementaci, protoze bych to sam asi nezvladl
#define NA_STRING_HELPER(x) #x
#define NA_STRING(x) NA_STRING_HELPER(x)


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
    FILE *souborSNazvy = fopen("nastroje.lad", "r"); // Stačí režim "r"
    if (souborSNazvy == NULL) return;

    short kontrola_poctu = fscanf(souborSNazvy, "%i;", &pocetHudebnichNastroju);
    if (kontrola_poctu != 1) {
        pocetHudebnichNastroju = 0;
        nazvySouboru = NULL;
        fclose(souborSNazvy);
        return;
    }

    // Alokace pole ukazatelů
    nazvySouboru = malloc(sizeof(char *) * pocetHudebnichNastroju);
    for (int i = 0; i < pocetHudebnichNastroju; i++) {
        nazvySouboru[i] = malloc(sizeof(char) * MAXIMALNI_DELKA_NAZVU_NASTROJE);
        memset(nazvySouboru[i], '\0', MAXIMALNI_DELKA_NAZVU_NASTROJE);
    }

    // === TADY JE TA ZMĚNA: Žádné fgets, žádné ruční sekání znaků ===
    for (int i = 0; i < pocetHudebnichNastroju; i++) {
        // Mezera před % přeskočí případné neviditelné bílé znaky (odřádkování, mezery)
        fscanf(souborSNazvy, " %" NA_STRING(MAXIMALNI_DELKA_NAZVU_NASTROJE) "[^;];", nazvySouboru[i]);
    }

    fclose(souborSNazvy);
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
        FILE *soubor_s_aktual_nastrojem = fopen(nazvySouboru[index_aktualniho_souboru], "r");
        if (soubor_s_aktual_nastrojem == NULL)
        {
            continue;
        }
        rewind(soubor_s_aktual_nastrojem);
        short _kontrola_nacteni_poctu_frekvenci = fscanf(soubor_s_aktual_nastrojem, "%i;", &poleHudebnichNastroju[index_aktualniho_souboru].pocetTonu);
        if (_kontrola_nacteni_poctu_frekvenci != 1 || poleHudebnichNastroju[index_aktualniho_souboru].pocetTonu < 1)
        {
            poleHudebnichNastroju[index_aktualniho_souboru].pocetTonu = 0;
            continue;
        }
        char _docasne_uloziste_pro_nazev[MAXIMALNI_DELKA_NAZVU_NASTROJE + 1];
        memset(_docasne_uloziste_pro_nazev, 0, sizeof(_docasne_uloziste_pro_nazev));
        fscanf(soubor_s_aktual_nastrojem, "%" NA_STRING(MAXIMALNI_DELKA_NAZVU_NASTROJE) "[^;];", _docasne_uloziste_pro_nazev); // tuto cast kodu vygenerovalo gemini
        strcpy(poleHudebnichNastroju[index_aktualniho_souboru].nazev, _docasne_uloziste_pro_nazev);
        poleHudebnichNastroju[index_aktualniho_souboru].poleTonu = calloc(poleHudebnichNastroju[index_aktualniho_souboru].pocetTonu, sizeof(int));
        poleHudebnichNastroju[index_aktualniho_souboru].nazvyTonu = calloc(poleHudebnichNastroju[index_aktualniho_souboru].pocetTonu, sizeof(char*));
        for (int ___index_pro_alokaci = 0; ___index_pro_alokaci < poleHudebnichNastroju[index_aktualniho_souboru].pocetTonu; ___index_pro_alokaci++)
        {
            poleHudebnichNastroju[index_aktualniho_souboru].nazvyTonu[___index_pro_alokaci] = calloc(MAXIMALNI_DELKA_NAZVU_TONU, sizeof(char));
        }
        for (int _index_nacitani_frekvenci = 0; _index_nacitani_frekvenci <  poleHudebnichNastroju[index_aktualniho_souboru].pocetTonu; _index_nacitani_frekvenci++)
        {
            fscanf(soubor_s_aktual_nastrojem, "%i;", &poleHudebnichNastroju[index_aktualniho_souboru].poleTonu[_index_nacitani_frekvenci]);
        }
        for (int ___index_nacitani_nazvu_tonu = 0; ___index_nacitani_nazvu_tonu < poleHudebnichNastroju[index_aktualniho_souboru].pocetTonu; ___index_nacitani_nazvu_tonu++)
        {
            fscanf(soubor_s_aktual_nastrojem, "%" NA_STRING(MAXIMALNI_DELKA_NAZVU_TONU) "[^;];", poleHudebnichNastroju[index_aktualniho_souboru].nazvyTonu[___index_nacitani_nazvu_tonu]);
        }
        // DODELAT TAM NAHORE TY ZMRDY FGETS
        fclose(soubor_s_aktual_nastrojem);
    }
}

void nacitaniStrukturMain(void)
{
    // ziskame odkud cerpat informace

    nacistJmenaSouboru();
    nacistNastrojeZeSouboru();
}