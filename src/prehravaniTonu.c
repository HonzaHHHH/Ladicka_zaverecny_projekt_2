#include <stdio.h>
#include <stdlib.h>
#include <nastaveni.h>
#include <portaudio.h>
#include <prehravaniTonu.h>
#include <nacitaniStruktur.h>
#include <portaudio.h>
#include <math.h>

void prehraniTonu(void);
struct dataProStream
{
    int frekvence;
    double faze;
};

PaStream * nastaveniPortAudioStreamu(struct hudebniNastroj nastroj);
int pamujcallback(const void *inputBuffer, void *outputBuffer, unsigned long framesPerBuffer, const PaStreamCallbackTimeInfo *timeInfo, PaStreamCallbackFlags statusFlags, void *userData);

void prehraniTonu(void)
{
    setupKytara();
    Pa_Initialize();
    nastaveniPortAudioStreamu(gitara);
}

PaStream * nastaveniPortAudioStreamu(struct hudebniNastroj nastroj)
{
    PaStream * polestreamu = malloc(sizeof(PaStream) * nastroj.pocetTonu);
    struct dataProStream * poleDatProStream = malloc(sizeof(struct dataProStream) * nastroj.pocetTonu);
    for (int i = 0; i < nastroj.pocetTonu; i++)
    {
        poleDatProStream[i].frekvence = nastroj.poleTonu[i];
        poleDatProStream[i].faze = 0;
    }
    for (int i = 0; i < nastroj.pocetTonu; i++)
    {
        Pa_OpenDefaultStream(&polestreamu[i], 0, 1, paFloat32, vzorkovaciFrekvence, vzorkuNaBuffer, &pamujcallback, &nastroj.poleTonu[i]);
    }
}

int pamujcallback(const void *inputBuffer, void *outputBuffer, unsigned long framesPerBuffer, const PaStreamCallbackTimeInfo *timeInfo, PaStreamCallbackFlags statusFlags, void *userData)
{
    float * out = (float*)outputBuffer;
    int * frekvence = (int*)userData;
    double faze;
    for (long int i = 0; i < framesPerBuffer; i++)
    {
        *out++ = sin(faze);
        faze += 2*cisloPi*(*frekvence) / vzorkovaciFrekvence;
        if (faze > 2*cisloPi) faze = 0;
    }
    return 0;

}