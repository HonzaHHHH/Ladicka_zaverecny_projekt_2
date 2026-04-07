#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <nastaveni.h>
#include <portaudio.h>
#include <prehravaniTonu.h>
#include <math.h>

short nastaveni = 0;
int vzorkovaciFrekvence = 44100;
int vzorkuNaBuffer = 256;
double cisloPi = 3.14159265358979323846264338327950288419716939937510f; // převzato z Wikipedie: https://cs.wikipedia.org/wiki/P%C3%AD_(%C4%8D%C3%ADslo)

void nastaveniNastaveni();

void praceSParametrem(char *parametry) {
    if (strcmp(parametry, "--ignsys") == 0) // pro ignoraci typu systému
    {
        nastaveni |= (1 << NASTAVENI_IGNSYS);
    }
    if (strcmp(parametry, "--dipp") == 0) // dialog ++
    {
        nastaveni |= (1 << NASTAVENI_DIALOGPLUS);
    }
    
}

PaStream **nastaveniPortAudioStreamu(struct hudebniNastroj nastroj)
{
    PaStream **polestreamu = malloc(sizeof(PaStream *) * nastroj.pocetTonu);
    struct dataProStream * poleDatProStream = malloc(sizeof(struct dataProStream) * nastroj.pocetTonu);
    for (int i = 0; i < nastroj.pocetTonu; i++)
    {
        poleDatProStream[i].frekvence = nastroj.poleTonu[i];
        poleDatProStream[i].faze = 0;
    }
    for (int i = 0; i < nastroj.pocetTonu; i++)
    {
        Pa_OpenDefaultStream(&polestreamu[i], 0, 1, paFloat32, vzorkovaciFrekvence, vzorkuNaBuffer, &pamujcallback, &poleDatProStream[i]);
    }
    return polestreamu;
}

int pamujcallback(const void *inputBuffer, void *outputBuffer, unsigned long framesPerBuffer, const PaStreamCallbackTimeInfo *timeInfo, PaStreamCallbackFlags statusFlags, void *userData)
{
    float *out = (float *)outputBuffer;
    struct dataProStream *data = (struct dataProStream *)userData;
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