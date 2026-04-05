#include <stdio.h>
#include <stdlib.h>
#include <nastaveni.h>
#include <prehravaniTonu.h>
#include <nacitaniStruktur.h>
#include <portaudio.h>
#include <terminalSettings.h>
#include <math.h>

void prehraniTonu(void);
struct dataProStream
{
    int frekvence;
    double faze;
};

struct dataProStream *poleDatProStream;

void hratTon(PaStream *ukazatelNaStream);
PaStream **nastaveniPortAudioStreamu(struct hudebniNastroj nastroj);
int pamujcallback(const void *inputBuffer, void *outputBuffer, unsigned long framesPerBuffer, const PaStreamCallbackTimeInfo *timeInfo, PaStreamCallbackFlags statusFlags, void *userData);

void prehraniTonu(void)
{
    setupKytara();
    int errorPortAudio = Pa_Initialize();
    if (errorPortAudio != paNoError)
    {
        printf("PortAudio error: %s\n", Pa_GetErrorText(errorPortAudio));
        exit(EXIT_ERRLIBS + EXIT_VPLAY + 1);
    }
    PaStream **poleStreamu = nastaveniPortAudioStreamu(gitara);
    short cisloVNabidce = 0;
    char voliciZnak = 0;
    short konecFunkce = 1;
    while (konecFunkce)
    {
        clearScreen();
        for (int i = 0; i < gitara.pocetTonu; i++)
        {
            if (cisloVNabidce == i)
                printf("\x1b[32m"
                       "\n%s"
                       "\x1b[0m",
                       gitara.nazvyTonu[i]);
            else
                printf("\n%s", gitara.nazvyTonu[i]);
        }
        voliciZnak = getCharNow();
        switch (voliciZnak)
        {
        case 's':
        case 'S':
            if (cisloVNabidce < gitara.pocetTonu - 1)
                cisloVNabidce++;
            break;
        case 'w':
        case 'W':
            if (cisloVNabidce > 0)
                cisloVNabidce--;
            break;
        case '\n':
        clearScreen();
            for (int i = 0; i < gitara.pocetTonu; i++)
            {
                if (cisloVNabidce == i)
                    printf("\x1b[32m"
                           "\n### %s ### Nyní hraje ###"
                           "\x1b[0m",
                           gitara.nazvyTonu[i]);
                else
                    printf("\n%s", gitara.nazvyTonu[i]);
            }
            hratTon(poleStreamu[cisloVNabidce]);
            break;
        case 'q':
        case 'Q':
            konecFunkce = 0;

            break;
        }
    }
    Pa_Terminate();
}

PaStream **nastaveniPortAudioStreamu(struct hudebniNastroj nastroj)
{
    PaStream **polestreamu = malloc(sizeof(PaStream *) * nastroj.pocetTonu);
    poleDatProStream = malloc(sizeof(struct dataProStream) * nastroj.pocetTonu);
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

void hratTon(PaStream *ukazatelNaStream)
{
    Pa_StartStream(ukazatelNaStream);
    char stopZnak = 0;
    while (stopZnak != 'q' && stopZnak != 'Q')
    {
        stopZnak = getCharNow();
    }
    Pa_StopStream(ukazatelNaStream);
}