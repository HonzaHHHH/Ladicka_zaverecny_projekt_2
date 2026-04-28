/*
    Tato část kódu obsahuje kód pro přehrávání tónu, podle kterého si uživatel může ladit nástroj
*/

#include <stdio.h>
#include <stdlib.h>
#include <nastaveni.h>
#include <prehravaniTonu.h>
#include <nacitaniStruktur.h>
#include <portaudio.h>
#include <terminalSettings.h>
#include <math.h>

void PrehravaniTonuMain(void);
struct dataProStreamPrehravani *poleDatProStream;
void hratTon(PaStream *ukazatelNaStream);
int PaCallbackPrehravani(const void *inputBuffer, void *outputBuffer, unsigned long framesPerBuffer, const PaStreamCallbackTimeInfo *timeInfo, PaStreamCallbackFlags statusFlags, void *userData);

/**
 * Nabídka pro přehrávání tónu
 */
void PrehravaniTonuMain(void)
{
    setupKytara();
    int errorPortAudio = Pa_Initialize();
    if (errorPortAudio != paNoError)
    {
        printf("PortAudio error: %s\n", Pa_GetErrorText(errorPortAudio));
        exit(EXIT_ERRLIBS + EXIT_VPLAY + 1);
    }
    PaStream **poleStreamu = nastaveniPortAudioStreamuPrehravani(gitara);
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

/**
 * Začne hrát tón ze streamu
 */
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

/**
 * Callback pro portaudio
 */
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