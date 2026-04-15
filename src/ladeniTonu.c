#include <portaudio.h>
#include <stdio.h>
#include <stdlib.h>
#include <terminalSettings.h>
#include <nastaveni.h>
#include <nacitaniStruktur.h>
#include <kiss_fft.h>

void ladeniTonu();
void laditTon(PaStream *ukazatelNaStream);
int PaCallbackLadeni(const void *inputBuffer, void *outputBuffer, unsigned long framesPerBuffer, const PaStreamCallbackTimeInfo *timeInfo, PaStreamCallbackFlags statusFlags, void *userData);
PaStream **nastaveniPortAudioStreamuLadeni(struct hudebniNastroj);
int frekvenceZPole(float * fronta);

void ladeniTonu()
{
    setupKytara();
    int errorPortAudio = Pa_Initialize();
    if (errorPortAudio != paNoError)
    {
        printf("PortAudio error: %s\n", Pa_GetErrorText(errorPortAudio));
        exit(EXIT_ERRLIBS + EXIT_VLADENI + 1);
    }
    PaStream **poleStreamu = nastaveniPortAudioStreamuLadeni(gitara);
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
                           "\n### %s ### Nyní ladíme ###"
                           "\x1b[0m",
                           gitara.nazvyTonu[i]);
                else
                    printf("\n%s", gitara.nazvyTonu[i]);
            }
            laditTon(poleStreamu[cisloVNabidce]);
            break;
        case 'q':
        case 'Q':
            konecFunkce = 0;

            break;
        }
    }

    Pa_Terminate();
}

void laditTon(PaStream *ukazatelNaStream)
{
    Pa_StartStream(ukazatelNaStream);
    char stopZnak = 0;
    while (stopZnak != 'q' && stopZnak != 'Q')
    {
        stopZnak = getCharNow();
    }
    Pa_StopStream(ukazatelNaStream);
}

PaStream **nastaveniPortAudioStreamuLadeni(struct hudebniNastroj nastroj)
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
        Pa_OpenDefaultStream(&polestreamu[i], 1, 0, paFloat32, vzorkovaciFrekvence, vzorkuNaBuffer, &PaCallbackLadeni, &poleDatProStream[i]);
    }
    return polestreamu;
}

int PaCallbackLadeni(const void *inputBuffer, void *outputBuffer, unsigned long framesPerBuffer, const PaStreamCallbackTimeInfo *timeInfo, PaStreamCallbackFlags statusFlags, void *userData)
{
    float *out = (float *)outputBuffer;
    struct dataProStreamLadeni * data = (struct dataProStreamLadeni *)userData;
    if (inputBuffer == NULL)
        return paContinue;

    for (unsigned long int i = 0; i < framesPerBuffer; i++)
    {
        data->ulozisteProVstup[data->indexUloziste];
        if (data->indexUloziste > VZORKOVACIFREK / 4 - 1)
        {
            data->indexUloziste = 0;
            printf("ladi se\n");
            //printf("frekvence %i", frekvenceZPole(&data->ulozisteProVstup));
        }
    }
    return 0;
}

int frekvenceZPole(float * fronta) {
    
}