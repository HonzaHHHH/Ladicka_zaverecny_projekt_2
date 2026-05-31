/*
    Tato část kódu řeší načítání dat ze souboru pro každý hudební nástroj
*/

#include <stdlib.h>
#include <nacitaniStruktur.h>
#include <stdio.h>
#include <unistd.h>
#include <string.h>
#include <syslog.h>

// tyto 2 makra mi pomohla gemini s vytvorenim a implementaci, protoze bych to sam asi nezvladl
#define NA_STRING_HELPER(x) #x
#define NA_STRING(x) NA_STRING_HELPER(x)

/**
 * Pole hudebních nástrojů - pole struktur hudebních nástrojů, ze kterého se pak tvoří streamy
 */
struct hudebniNastroj *poleHudebnichNastroju;

/**
 * Počet hudebních nástrojů
 */
int pocetHudebnichNastroju;

/**
 * Index aktuálního hudebního nástroje, výchozí hodnota je 0
 */
int aktualniHudebniNastroj = 0;

/**
 * Řětězce pro načítání souborů, využití pouze v rámci tohoto .c souboru
 */
char **nazvySouboru;


/**
 * Načte jména souborů
 */
void nacistJmenaSouboru(void);

/**
 * Načte jednotlivé nástroje ze souborů
 */
void nacistNastrojeZeSouboru(void);

/**
 * Spustí načítání struktur
 */
void nacitaniStrukturMain(void);

// --------------------------------------------------------------------


/**
 * Tato funkce jednoduše načte ze souboru nastroje.lad 
 */
void nacistJmenaSouboru(void)
{
    FILE *souborSNazvy = fopen("nastroje.lad", "r"); // Stačí režim "r"
    if (souborSNazvy == NULL) 
    {
        syslog(LOG_WARNING, "Neexistuje soubor s nástroji");
        // řízení pro první spuštění
    }
    short kontrola_poctu = fscanf(souborSNazvy, "%i;", &pocetHudebnichNastroju);
    if (kontrola_poctu != 1) {
        pocetHudebnichNastroju = 0;
        nazvySouboru = NULL;
        fclose(souborSNazvy);
        syslog(LOG_ERR, "Soubor s nástroji nejde přečíst");
        return;
    }

    // Alokace pole ukazatelů
    nazvySouboru = malloc(sizeof(char *) * pocetHudebnichNastroju);
    for (int i = 0; i < pocetHudebnichNastroju; i++) {
        nazvySouboru[i] = malloc(sizeof(char) * MAXIMALNI_DELKA_NAZVU_NASTROJE);
        memset(nazvySouboru[i], '\0', MAXIMALNI_DELKA_NAZVU_NASTROJE);
    }

    for (int i = 0; i < pocetHudebnichNastroju; i++) {
        // Mezera před % přeskočí případné neviditelné bílé znaky (odřádkování, mezery)
        fscanf(souborSNazvy, " %" NA_STRING(MAXIMALNI_DELKA_NAZVU_NASTROJE) "[^;];", nazvySouboru[i]); // toto mi opet poradilo gemini po řadě neúspěchů
    }

    fclose(souborSNazvy);
}

void nacistNastrojeZeSouboru(void)
{
    poleHudebnichNastroju = malloc(sizeof(struct hudebniNastroj) * pocetHudebnichNastroju);
    if (poleHudebnichNastroju == NULL)
    {
        syslog(LOG_ERR, "Nealokovalo se pole hudebních nástrojů");
        return;
    }
    memset(poleHudebnichNastroju, 0, sizeof(struct hudebniNastroj) * pocetHudebnichNastroju);
    for (int index_aktualniho_souboru = 0; index_aktualniho_souboru < pocetHudebnichNastroju; index_aktualniho_souboru++)
    {
        FILE *soubor_s_aktual_nastrojem = fopen(nazvySouboru[index_aktualniho_souboru], "r");
        if (soubor_s_aktual_nastrojem == NULL)
        {
            syslog(LOG_WARNING, "soubor s hudebním nástrojem (index %i) se nepodařilo otevřít, přeskakuji", index_aktualniho_souboru);
            continue;
        }
        rewind(soubor_s_aktual_nastrojem);
        short _kontrola_nacteni_poctu_frekvenci = fscanf(soubor_s_aktual_nastrojem, "%i;", &poleHudebnichNastroju[index_aktualniho_souboru].pocetTonu);
        if (_kontrola_nacteni_poctu_frekvenci != 1 || poleHudebnichNastroju[index_aktualniho_souboru].pocetTonu < 1)
        {
            poleHudebnichNastroju[index_aktualniho_souboru].pocetTonu = 0;
            syslog(LOG_WARNING, "U souboru s hudebním nástrojem (index %i) se nepodařilo načíst počet tónů", index_aktualniho_souboru);
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
        fclose(soubor_s_aktual_nastrojem);
    }
}

void nacitaniStrukturMain(void)
{
    // ziskame odkud cerpat informace

    nacistJmenaSouboru();
    nacistNastrojeZeSouboru();
}