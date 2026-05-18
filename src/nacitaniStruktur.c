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
int aktualniHudebniNastroj;


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

void nacistJmenaSouboru(char ***ukazatel_na_soubory, int *pocetSouboru)
{
    FILE *souborSNazvy = fopen("nastroje.lad", "r+");
    if (souborSNazvy == NULL)
    {
        // neco na logy to bude chtit
        return;
    }
    rewind(souborSNazvy);
    short kontrola_poctu = fscanf(souborSNazvy, "%i;", pocetSouboru);
    if (kontrola_poctu != 1)
    {
        *pocetSouboru = 0;
        *ukazatel_na_soubory = NULL;
    }
    **ukazatel_na_soubory = malloc(MAXIMALNI_DELKA_NAZVU_SEZNAMU_NASTROJU * *pocetSouboru);
    char docasny_uloziste_pro_Jmena[MAXIMALNI_DELKA_NAZVU_SEZNAMU_NASTROJU *  (*pocetSouboru)];
    fgets(docasny_uloziste_pro_Jmena, MAXIMALNI_DELKA_NAZVU_SEZNAMU_NASTROJU * (*pocetSouboru), souborSNazvy);
    for (int index_znaku = 0, index_souboru = 0; index_znaku < sizeof(docasny_uloziste_pro_Jmena); index_znaku++)
    {
        if (docasny_uloziste_pro_Jmena[index_znaku] == ';')
        {
            index_souboru++;
            continue;
        }
        *ukazatel_na_soubory[index_souboru][index_znaku] = docasny_uloziste_pro_Jmena[index_znaku];
    }
    fclose(souborSNazvy);
    return;
}

void nacistNastrojeZeSouboru(struct hudebniNastroj * __pole_nastroju, char ** __pole_nazvu_souboru, int __pocet_nastroju)
{
    int _aktualni_index_vyrovanvaci_pameti = 0;
    for (int index_aktualniho_souboru = 0; index_aktualniho_souboru < __pocet_nastroju; index_aktualniho_souboru++)
    {
        FILE * soubor_s_aktual_nastrojem = fopen(__pole_nazvu_souboru[index_aktualniho_souboru], "r");
        if (soubor_s_aktual_nastrojem == NULL)
        {
            memset(__pole_nastroju, 0, sizeof(struct hudebniNastroj));
            continue;
        }
        char _vyrovnavaci_pamet[MAXIMALNI_DELKA_NAZVU_NASTROJE + MAXIMALNI_POCET_TONU * MAXIMALNI_DELKA_FREKVENCE_V_CHARECH + MAXIMALNI_POCET_TONU * MAXIMALNI_DELKA_NAZVU_TONU + MAXIMALNI_DELKA_POCTU_TONU_V_CHARECH];
        memset(_vyrovnavaci_pamet, 0, sizeof(_vyrovnavaci_pamet));
        fgets(_vyrovnavaci_pamet, sizeof(_vyrovnavaci_pamet), soubor_s_aktual_nastrojem);
        for (int _polozka_ve_strukture = 0, _pozice_stringu = 0; _aktualni_index_vyrovanvaci_pameti < sizeof(_vyrovnavaci_pamet); _aktualni_index_vyrovanvaci_pameti++)
        {
            if (_vyrovnavaci_pamet[_aktualni_index_vyrovanvaci_pameti] == ';')
            {
                _polozka_ve_strukture++;
                _pozice_stringu = 0;
                continue;
            }
            switch (_polozka_ve_strukture)
            {
                case 0:
                    __pole_nastroju[index_aktualniho_souboru].nazev[_pozice_stringu] = _vyrovnavaci_pamet[_aktualni_index_vyrovanvaci_pameti];
                    break;
                case 1:
                    fscanf(soubor_s_aktual_nastrojem, "%i", &__pole_nastroju[index_aktualniho_souboru].pocetTonu);
                break;
            }
        }
    }
}

void nacitaniStrukturMain(void)
{
    // ziskame odkud cerpat informace
    char **nazvySouboru = NULL;
    int pocetNazvuSouboru = 0;
    nacistJmenaSouboru(&nazvySouboru, &pocetNazvuSouboru);

    // jdeme cerpat informace
    struct hudebniNastroj * _hudebni_nastroje_pole;
    int _pocet_Nastroju = pocetNazvuSouboru;
    _hudebni_nastroje_pole = malloc(sizeof(struct hudebniNastroj) * _pocet_Nastroju);
    nacistNastrojeZeSouboru(_hudebni_nastroje_pole, nazvySouboru, _pocet_Nastroju);
}