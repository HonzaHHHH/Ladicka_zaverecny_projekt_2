/*
    Tato část kódu slouží k načítání struktur hudebních nástrojů ze souborů
*/

#include <stdlib.h>
#include <nacitaniStruktur.h>
#include <stdio.h>
#include <unistd.h>
#include <string.h>

#define MAXIMALNI_DELKA_NAZVU_SEZNAMU_NASTROJU 20
#define MAXIMALNI_DELKA_NAZVU_TONU 20
#define MAXIMALNI_DELKA_FREKVENCE_V_CHARECH 5
#define MAXIMALNI_DELKA_POCTU_TONU_V_CHARECH 4
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
    strcpy(gitara.nazev, "Kytara 6 strunna");

    gitara.pocetTonu = 6;

    gitara.poleTonu = malloc(sizeof(int) * gitara.pocetTonu);

    if (gitara.poleTonu == NULL)
    {
        return;
    }

    gitara.poleTonu[0] = 82;
    gitara.poleTonu[1] = 110;
    gitara.poleTonu[2] = 147;
    gitara.poleTonu[3] = 196;
    gitara.poleTonu[4] = 247;
    gitara.poleTonu[5] = 330;

    gitara.nazvyTonu = malloc(sizeof(char *) * gitara.pocetTonu);

    if (gitara.nazvyTonu == NULL)
    {
        free(gitara.poleTonu);
        return;
    }

    for (int i = 0; i < gitara.pocetTonu; i++)
    {
        gitara.nazvyTonu[i] = malloc(sizeof(char) * MAXIMALNI_DELKA_NAZVU_TONU);

        if (gitara.nazvyTonu[i] == NULL)
        {
            return;
        }

        memset(gitara.nazvyTonu[i], '\0', MAXIMALNI_DELKA_NAZVU_TONU);
    }

    strncpy(gitara.nazvyTonu[0], "Struna E2", MAXIMALNI_DELKA_NAZVU_TONU - 1);
    strncpy(gitara.nazvyTonu[1], "Struna A2", MAXIMALNI_DELKA_NAZVU_TONU - 1);
    strncpy(gitara.nazvyTonu[2], "Struna D3", MAXIMALNI_DELKA_NAZVU_TONU - 1);
    strncpy(gitara.nazvyTonu[3], "Struna G3", MAXIMALNI_DELKA_NAZVU_TONU - 1);
    strncpy(gitara.nazvyTonu[4], "Struna B3", MAXIMALNI_DELKA_NAZVU_TONU - 1);
    strncpy(gitara.nazvyTonu[5], "Struna E4", MAXIMALNI_DELKA_NAZVU_TONU - 1);
}

// --------------------------------------------------------------------

void nacistJmenaSouboru(void)
{
    FILE *souborSNazvy = fopen("nastroje.lad", "r");

    if (souborSNazvy == NULL)
    {
        return;
    }

    rewind(souborSNazvy);

    short kontrola_poctu = fscanf(souborSNazvy, "%i;", &pocetHudebnichNastroju);

    if (kontrola_poctu != 1)
    {
        pocetHudebnichNastroju = 0;
        nazvySouboru = NULL;

        fclose(souborSNazvy);

        return;
    }

    nazvySouboru = malloc(sizeof(char *) * pocetHudebnichNastroju);

    if (nazvySouboru == NULL)
    {
        fclose(souborSNazvy);
        return;
    }

    for (int indexStringu = 0; indexStringu < pocetHudebnichNastroju; indexStringu++)
    {
        nazvySouboru[indexStringu] = malloc(sizeof(char) * MAXIMALNI_DELKA_NAZVU_NASTROJE);

        if (nazvySouboru[indexStringu] == NULL)
        {
            fclose(souborSNazvy);
            return;
        }

        memset(
            nazvySouboru[indexStringu],
            '\0',
            MAXIMALNI_DELKA_NAZVU_NASTROJE
        );
    }

    char docasneUloziste[
        (MAXIMALNI_DELKA_NAZVU_SEZNAMU_NASTROJU + 2)
        * pocetHudebnichNastroju
        + 2
    ];

    memset(docasneUloziste, '\0', sizeof(docasneUloziste));

    fgets(
        docasneUloziste,
        sizeof(docasneUloziste),
        souborSNazvy
    );

    int delka = strlen(docasneUloziste);

    for (
        int indexZnaku = 0,
            indexSouboru = 0,
            indexZnakuVNazvu = 0;

        indexZnaku < delka;

        indexZnaku++
    )
    {
        if (docasneUloziste[indexZnaku] == ';')
        {
            if (indexSouboru < pocetHudebnichNastroju)
            {
                nazvySouboru[indexSouboru][indexZnakuVNazvu] = '\0';
            }

            indexSouboru++;
            indexZnakuVNazvu = 0;

            continue;
        }

        if (docasneUloziste[indexZnaku] == '\n')
        {
            continue;
        }

        if (
            indexSouboru < pocetHudebnichNastroju &&
            indexZnakuVNazvu < MAXIMALNI_DELKA_NAZVU_NASTROJE - 1
        )
        {
            nazvySouboru[indexSouboru][indexZnakuVNazvu] =
                docasneUloziste[indexZnaku];

            indexZnakuVNazvu++;
        }
    }

    fclose(souborSNazvy);
}

// --------------------------------------------------------------------

void nacistNastrojeZeSouboru(void)
{
    poleHudebnichNastroju = malloc(
        sizeof(struct hudebniNastroj)
        * pocetHudebnichNastroju
    );

    if (poleHudebnichNastroju == NULL)
    {
        return;
    }

    memset(
        poleHudebnichNastroju,
        0,
        sizeof(struct hudebniNastroj)
        * pocetHudebnichNastroju
    );

    for (
        int indexAktualnihoSouboru = 0;

        indexAktualnihoSouboru < pocetHudebnichNastroju;

        indexAktualnihoSouboru++
    )
    {
        FILE *souborSNastrojem = fopen(
            nazvySouboru[indexAktualnihoSouboru],
            "r"
        );

        if (souborSNastrojem == NULL)
        {
            continue;
        }

        char vyrovnavaciPamet[
            MAXIMALNI_DELKA_NAZVU_NASTROJE
            + MAXIMALNI_POCET_TONU
            * MAXIMALNI_DELKA_FREKVENCE_V_CHARECH
            + MAXIMALNI_POCET_TONU
            * MAXIMALNI_DELKA_NAZVU_TONU
            + MAXIMALNI_DELKA_POCTU_TONU_V_CHARECH
            + 100
        ];

        memset(
            vyrovnavaciPamet,
            '\0',
            sizeof(vyrovnavaciPamet)
        );

        fgets(
            vyrovnavaciPamet,
            sizeof(vyrovnavaciPamet),
            souborSNastrojem
        );

        int delka = strlen(vyrovnavaciPamet);

        int polozkaVeStrukture = 0;
        int poziceStringu = 0;

        char docasneCislo[20];

        memset(docasneCislo, '\0', sizeof(docasneCislo));

        for (
            int indexPameti = 0;

            indexPameti < delka;

            indexPameti++
        )
        {
            if (vyrovnavaciPamet[indexPameti] == ';')
            {
                if (polozkaVeStrukture == 1)
                {
                    poleHudebnichNastroju[indexAktualnihoSouboru].pocetTonu =
                        atoi(docasneCislo);
                }

                polozkaVeStrukture++;
                poziceStringu = 0;

                memset(docasneCislo, '\0', sizeof(docasneCislo));

                continue;
            }

            if (vyrovnavaciPamet[indexPameti] == '\n')
            {
                continue;
            }

            switch (polozkaVeStrukture)
            {
                case 0:

                    if (
                        poziceStringu
                        < MAXIMALNI_DELKA_NAZVU_NASTROJE - 1
                    )
                    {
                        poleHudebnichNastroju
                            [indexAktualnihoSouboru]
                            .nazev[poziceStringu]
                            = vyrovnavaciPamet[indexPameti];

                        poziceStringu++;

                        poleHudebnichNastroju
                            [indexAktualnihoSouboru]
                            .nazev[poziceStringu]
                            = '\0';
                    }

                    break;

                case 1:

                    if (poziceStringu < sizeof(docasneCislo) - 1)
                    {
                        docasneCislo[poziceStringu] =
                            vyrovnavaciPamet[indexPameti];

                        poziceStringu++;
                    }

                    break;
            }
        }

        fclose(souborSNastrojem);
    }
}

// --------------------------------------------------------------------

void nacitaniStrukturMain(void)
{
    nacistJmenaSouboru();
    nacistNastrojeZeSouboru();
}