

/*
    Závěrečný projekt ze 2. ročníku
    ▮           ▮     ▮▮▮▮     ▮   ▮▮▮▮   ▮    ▮▮     ▮
    ▮          ▮ ▮    ▮   ▮    ▮  ▮▮▮▮▮▮  ▮   ▮▮     ▮ ▮
    ▮         ▮   ▮   ▮   ▮    ▮  ▮       ▮▮▮▮      ▮   ▮
    ▮        ▮▮▮▮▮▮▮  ▮   ▮    ▮  ▮       ▮   ▮▮   ▮▮▮▮▮▮▮
    ▮▮▮▮▮▮  ▮       ▮ ▮▮▮▮     ▮  ▮▮▮▮▮▮  ▮    ▮▮ ▮       ▮

    LADIČKA
    V konzoli
*/


#include <stdio.h>
#include <stdlib.h>
#include <portaudio.h> // portaudio-19-dev
#include <unistd.h>
#include <nastaveni.h>
#include <terminalSettings.h> // moje knihovna
#include <prehravaniTonu.h>
#include <ladeniTonuMain.h>

// Funkce pro přeheldnost deklaruji již tady (a taky se mi to více líbí)

int main(int pocetParametru, char **argumenty);
void vypsatNabidku(int aktivniPolozka);
void odejit(void);

int main(int pocetArgumentu, char **argumenty)
{
    // ----------------------------------------------------------
    /*
        První část programu - načtení parametrů z konzole
    */
   // ------------------------------------------------------------
    for (int i = 1; i < pocetArgumentu; i++)
    {
        praceSParametrem(argumenty[i]);
    }
    if (nastaveni & ((1 << NASTAVENI_CHCEPOUZEHELP) | (1 << NASTAVENI_HELPASTART)))
    {
        printf("Ladička\n\nUmožňuje ladění různých hudebních nástrojů v konzoli\n\nAutor: Jan Huml\n2026\n\n\nPoužití:\n\nladicka [PARAMETRY]\n\nSeznam parametrů:\n--help   Zobrazí nápovědu\n--has     Zobrazí nápovědu a poté spustí program\n --dipp    Dialogy++ - nechte se překvapit ;-)\n--ignsys    Ignoruje systém (použiití na vlastní riziko)\nProgram se ovládá klávesami WASD, Q pro krok zpátky a enterem\nPro pokračnování stiskněte jakoukkoliv klávesu\n\n");
        setupTerminalFunctions();
        getCharNow();
        if (nastaveni & (1 << NASTAVENI_CHCEPOUZEHELP))
        {
            exit(0);
        }
    }

    // ----------------------------------------------------------------
    /*
        Operační systém
        Jako nechci diskriminovat windouzy, ale je to zkoušený jen na linuxu a tím pádem to asi ještě rozjede mac, ale s windows fakt nevim
        Toto ukončení se dá preskočit parametrem z konzole
    */
    if (nastaveni & (1 << NASTAVENI_IGNSYS))
    {
        short OpSystem = 0;
#ifdef __unix__
        OpSystem |= (1 << 0);
#endif
#ifdef __linux__
        OpSystem |= (1 << 1);
#endif
        if (system == 0)
        {
            printf("Je mi líto, ale tento systém není podporován\n");
        }
    }


    setupTerminalFunctions(); // moje knihovna z loňského roku

    // -----------------------------------------------------------------
    // První nabídka, opět bez kanonického režimu

    char volici = 0;
    short cisloVNabidce = 0;
    while (1)
    {
        vypsatNabidku(cisloVNabidce);
        volici = getCharNow();
        if ((volici == 's' || volici == 'S') && cisloVNabidce < 3)
            cisloVNabidce++;
        if ((volici == 'w' || volici == 'W') && cisloVNabidce > 0)
            cisloVNabidce--;
        if (volici == '\n')
        {
            switch (cisloVNabidce)
            {
            case 0:
                PrehravaniTonuMain();
                break;
            case 1:
                ladeniTonuMain();
                break;
            case 2:
                nastaveniNastaveniMain();
                break;
            case 3:
                odejit();
                break;
            }
        }
        volici = 0;
    }
}

void vypsatNabidku(int aktivniPolozka)
{
    clearScreen();
    printf("\x1b[32m");
    printf("\n\n\n    ▮           ▮     ▮▮▮▮     ▮   ▮▮▮▮   ▮    ▮▮     ▮\n    ▮          ▮ ▮    ▮   ▮    ▮  ▮▮▮▮▮▮  ▮   ▮▮     ▮ ▮\n    ▮         ▮   ▮   ▮   ▮    ▮  ▮       ▮▮▮▮      ▮   ▮\n    ▮        ▮▮▮▮▮▮▮  ▮   ▮    ▮  ▮       ▮   ▮▮   ▮▮▮▮▮▮▮\n    ▮▮▮▮▮▮  ▮       ▮ ▮▮▮▮     ▮  ▮▮▮▮▮▮  ▮    ▮▮ ▮       ▮");
    printf("\nV konzoli od Jana Humla\n\nNavigujte klávesami W a S");
    printf("\x1b[0m");
    printf("\n\n");
    char *text[] = {"Poslouchat tóny\n", "Ladit přes mikrofon\n", "Nastavení\n", "Odejít\n"};
    for (int i = 0; i < 4; i++)
    {
        if (aktivniPolozka == i)
        {
            printf("\x1b[33m"
                   "%s"
                   "\x1b[0m",
                   text[i]);
        }
        else
            printf("%s", text[i]);
    }
}

void odejit(void)
{
    clearScreen();
    printf("Opravdu chcete odejít? (Budu smutný :-( ) [A/N]");
    char volba = getCharNow();
    switch (volba)
    {
    case 'a':
        clearScreen();
        printf("Program ukončen\n");
        if (nastaveni & (1 << NASTAVENI_DIALOGPLUS)) printf("(Hajzle!)\n");
        exit(EXIT_NORMALNI);
        break;
    case 'A':
        clearScreen();
        printf("Program ukončen\n");
        if (nastaveni & (1 << NASTAVENI_DIALOGPLUS)) printf("(Hajzle!)\n");
        exit(EXIT_NORMALNI);
    case 'n':
        clearScreen();
        return;
    case 'N':
        clearScreen();
        return;
    default:
        printf("Takovou klávesnici já neznám\n");
        sleep(1000);
        return;
        break;
    }
}