#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <nastaveni.h>
#include <portaudio.h>
#include <prehravaniTonu.h>
#include <math.h>
#include <terminalSettings.h>

short nastaveni = 0;
int vzorkovaciFrekvence = 44100;
int vzorkuNaBuffer = 256;
double cisloPi = 3.14159265358979323846264338327950288419716939937510f; // převzato z Wikipedie: https://cs.wikipedia.org/wiki/P%C3%AD_(%C4%8D%C3%ADslo)

void nastaveniNastaveni();

void vypsatNabidkuNastaveni(short index);

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

PaStream **nastaveniPortAudioStreamu(struct hudebniNastroj nastroj)
{
    PaStream **polestreamu = malloc(sizeof(PaStream *) * nastroj.pocetTonu);
    struct dataProStreamPrehravani *poleDatProStream = malloc(sizeof(struct dataProStreamPrehravani) * nastroj.pocetTonu);
    for (int i = 0; i < nastroj.pocetTonu; i++)
    {
        poleDatProStream[i].frekvence = nastroj.poleTonu[i];
        poleDatProStream[i].faze = 0;
    }
    for (int i = 0; i < nastroj.pocetTonu; i++)
    {
        Pa_OpenDefaultStream(&polestreamu[i], 0, 1, paFloat32, vzorkovaciFrekvence, vzorkuNaBuffer, &PaCallbackPrehravani, &poleDatProStream[i]);
    }
    return polestreamu;
}

int PaCallbackPrehravani(const void *inputBuffer, void *outputBuffer, unsigned long framesPerBuffer, const PaStreamCallbackTimeInfo *timeInfo, PaStreamCallbackFlags statusFlags, void *userData)
{
    float *out = (float *)outputBuffer;
    struct dataProStreamPrehravani *data = (struct dataProStreamPrehravani *)userData;
    double *faze = &data->faze;
    int frekvence = data->frekvence;
    for (long int i = 0; i < framesPerBuffer; i++)
    {
        *out++ = sin(*faze);
        *faze += 2 * cisloPi * frekvence / vzorkovaciFrekvence;
        if (*faze > 2 * cisloPi)
            *faze = 0;
    }
    return 0;
}

void nastaveniNastaveni(void)
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