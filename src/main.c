

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
#include <portaudio.h>
#include <unistd.h>
#include "terminalSettings.h"

int main(int pocetParametru, char **argumenty);
void praceSParametrem(char **parametry);
void vypsatNabidku(int aktivniPolozka);

int main(int pocetArgumentu, char **argumenty)
{

    for (int i = 1; i < pocetArgumentu; i++)
    {
        praceSParametrem(&argumenty[i]);
    }
    short system = 0;
#ifdef __unix__
    system |= (1 << 0);
#endif
#ifdef __linux__
    system |= (1 << 1);
#endif
    if (system == 0)
    {
        printf("Je mi líto, ale tento systém není podporován\n");
    }
    setupTerminalFunctions();
    char volici = 0;
    short cisloVNabidce = 0;
    while (1)
    {
        vypsatNabidku(cisloVNabidce);
        volici = getCharNow();
        if ((volici == 's' || volici == 'S') && cisloVNabidce < 3) cisloVNabidce++;
        if ((volici == 'w' || volici == 'W') && cisloVNabidce > 0) cisloVNabidce--;
        volici = 0;
        if (volici == '\n')
        {
            switch (cisloVNabidce)
            {
            case 0:
                /* code */
                break;
            
            default:
                break;
            }
        }
    }
}

void praceSParametrem(char **parametry) {}
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
        else printf("%s", text[i]);
    }
}