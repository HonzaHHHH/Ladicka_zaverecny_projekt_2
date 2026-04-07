#include <portaudio.h>
#include <stdio.h>
#include <stdlib.h>
#include <terminalSettings.h>
#include <nastaveni.h>
#include <nacitaniStruktur.h>


void ladeniTonu();

void ladeniTonu()
{
    setupKytara();
    int errorPortAudio = Pa_Initialize();
    if (errorPortAudio != paNoError)
    {
        printf("PortAudio error: %s\n", Pa_GetErrorText(errorPortAudio));
        exit(EXIT_ERRLIBS + EXIT_VLADENI + 1);
    }
    PaStream ** poleStreamu = nastaveniPortAudioStreamu(gitara);
}