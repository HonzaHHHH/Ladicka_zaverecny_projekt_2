/*
    Nastavení programu
 */

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <nastaveni.h>
#include <portaudio.h>
#include <prehravaniTonu.h>
#include <math.h>
#include <terminalSettings.h>

/**
 * Proměnná, kam se ukládají nastavení přes bitové masky
 */
short nastaveni = 0;

const int vzorkovaciFrekvence = 44100;
int vzorkuNaBuffer = 256;
double cisloPi = (double)3.14159265358979323846264338327950288419716939937510f; // převzato z Wikipedie: https://cs.wikipedia.org/wiki/P%C3%AD_(%C4%8D%C3%ADslo)
int velikostFFTbufferu = 32768;

/**
 * Spouští nabídku nastavení
 */
void nastaveniNastaveniMain();

void vypsatNabidkuNastaveni(short index);

/**
 * Dostane parametry a podle toho zapne přepínače
 */
void praceSParametrem(char *parametry);

/**
 * Vrátí ukazatele na streamy podle struktury hudebninastroj
 */
PaStream **nastaveniPortAudioStreamuPrehravani(struct hudebniNastroj nastroj);

void vybratAktualniNastroj(void);

//-----------------------------------------------------------------------------------------------------------------------------
void praceSParametrem(char *parametry)
{
    if (strcmp(parametry, "--ignsys") == 0) // pro ignoraci typu systému
    {
        nastaveni |= (1 << NASTAVENI_IGNSYS);
    }
    if (strcmp(parametry, "--dipp") == 0) // dialog ++
    {
        nastaveni |= (1 << NASTAVENI_DIALOGPLUS);
    }
    if (strcmp(parametry, "--help") == 0) // pouze zobrazi nápovědu
    {
        nastaveni |= (1 << NASTAVENI_CHCEPOUZEHELP);
    }
    if (strcmp(parametry, "--has") == 0) // helpne a zapne se
    {
        nastaveni |= (1 << NASTAVENI_HELPASTART);
    }
}

extern struct dataProStreamPrehravani *poleDatProStream;

PaStream **nastaveniPortAudioStreamuPrehravani(struct hudebniNastroj nastroj)
{
    if (nastroj.pocetTonu <= 0 || nastroj.poleTonu == NULL)
    {
        printf("Chyba: napadly nastroj ma pocetTonu=%d, poleTonu=%p\n", nastroj.pocetTonu, (void *)nastroj.poleTonu);
        return NULL;
    }
    PaStream **polestreamu = malloc(sizeof(PaStream *) * nastroj.pocetTonu);
    if (polestreamu == NULL)
    {
        printf("Chyba: nepodarilo se alokovat poleStreamu\n");
        return NULL;
    }
    poleDatProStream = malloc(sizeof(struct dataProStreamPrehravani) * nastroj.pocetTonu);
    if (poleDatProStream == NULL)
    {
        printf("Chyba: nepodarilo se alokovat poleDatProStream\n");
        free(polestreamu);
        return NULL;
    }
    for (int i = 0; i < nastroj.pocetTonu; i++)
    {
        poleDatProStream[i].frekvence = nastroj.poleTonu[i];
        poleDatProStream[i].faze = 0;
    }
    for (int i = 0; i < nastroj.pocetTonu; i++)
    {
        int paErr = Pa_OpenDefaultStream(&polestreamu[i], 0, 1, paFloat32, vzorkovaciFrekvence, vzorkuNaBuffer, &PaCallbackPrehravani, &poleDatProStream[i]);
        if (paErr != paNoError)
        {
            printf("PortAudio OpenStream error: %s\n", Pa_GetErrorText(paErr));
            for (int j = 0; j < i; j++)
                Pa_CloseStream(polestreamu[j]);
            free(polestreamu);
            free(poleDatProStream);
            poleDatProStream = NULL;
            return NULL;
        }
    }
    return polestreamu;
}

void nastaveniNastaveniMain(void)
{
    short indexNabidky = 0;
    char volba = 0;
    short konecFunkce = 1;
    while (konecFunkce)
    {
        clearScreen();
        vypsatNabidkuNastaveni(indexNabidky);
        volba = getCharNow();
        switch (volba)
        {
        case 's':
        case 'S':
            if (indexNabidky < 2)
                indexNabidky++;
            break;
        case 'w':
        case 'W':
            if (indexNabidky > 0)
                indexNabidky--;
            break;
        case '\n':
            switch (indexNabidky)
            {
            case 0:
                vybratAktualniNastroj();
            }
            break;
        case 'q':
        case 'Q':
            konecFunkce = 0;
            break;
        default:
            break;
        }
    }
}

void vypsatNabidkuNastaveni(short index)
{
    char *textNabidky[] = {"Vybrat nástroj", "Ladění tónů", "Přidat nástroj"};
    clearScreen();
    for (int i = 0; i < 3; i++)
    {
        if (index == i)
            printf("\x1b[33m"
                   "%s"
                   "\n\x1b[0m",
                   textNabidky[i]);
        else
            printf("%s\n", textNabidky[i]);
    }
}

void vybratAktualniNastroj(void)
{
    short indexNabidky = 0;
    char volba = 0;
    short konecFunkce = 1;
    while (konecFunkce)
    {
        clearScreen();
        printf("Který hudební nástroj chcete ladit?\n\n");
        for (int i = 0; i < 3; i++)
        {
            if (indexNabidky == i)
                printf("\x1b[33m"
                       "%s"
                       "\n\x1b[0m",
                       poleHudebnichNastroju[i].nazev);
            else
                printf("%s\n", poleHudebnichNastroju[i].nazev);
        }
        volba = getCharNow();
        switch (volba)
        {
        case 's':
        case 'S':
            if (indexNabidky < pocetHudebnichNastroju)
                indexNabidky++;
            break;
        case 'w':
        case 'W':
            if (indexNabidky > 0)
                indexNabidky--;
            break;
        case '\n':
            aktualniHudebniNastroj = indexNabidky;
            clearScreen();
            fflush(stdout);
            printf("Aktuální nástroj byl změněn na %s", poleHudebnichNastroju[aktualniHudebniNastroj].nazev);
            fflush(stdout);
            sleep(2);
            clearScreen();
            konecFunkce = 0;
            break;
        case 'q':
        case 'Q':
            konecFunkce = 0;
            break;
        default:
            break;
        }
    }
}