/**
 * Zde se bude ladit nástroj
 */


#include <portaudio.h>
#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include <terminalSettings.h>
#include <nastaveni.h>
#include <nacitaniStruktur.h>
#include <ncurses.h>
#include <kiss_fft.h>

/**
 * spustí nabídku pro ladění
 */
void ladeniTonuMain();

/**
 * Funkce bude ladit, není zde třeba žádny callback pro portaudio, protože jsem objevil funkci readStream, kterou bych mohl vyuižívat pro přehrávání tónu, ale nebyla by to taková zábava, proto ji používám pouze zde
 */
void laditTon(PaStream *ukazatelNaStream, int cilovaFrekvence);

PaStream **nastaveniPortAudioStreamuLadeni(struct hudebniNastroj);

//------------------------------------------------------------------------------------------------------------------------------

void ladeniTonuMain()
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
            laditTon(poleStreamu[cisloVNabidce], gitara.poleTonu[cisloVNabidce]);
            break;
        case 'q':
        case 'Q':
            konecFunkce = 0;

            break;
        }
    }

    Pa_Terminate();
}

void laditTon(PaStream *ukazatelNaStream, int cilovaFrekvence)
{
    float fftbuffer[velikostFFTbufferu];
    Pa_StartStream(ukazatelNaStream);
    char stopZnak = 0;
    kiss_fft_cfg konfig = kiss_fft_alloc(velikostFFTbufferu, 0, NULL, NULL);
    kiss_fft_cpx komplexniRovinaVstup[velikostFFTbufferu];
    kiss_fft_cpx komplexniRovinaVystup[velikostFFTbufferu];
    // inicializace ncurses pro neblkující čtení
    WINDOW *kontextOkna = initscr();
    cbreak();
    nodelay(kontextOkna, true);
    while (stopZnak != 'q' && stopZnak != 'Q')
    {
        /*
            Zde jsem objevil problém, že v mé knihovné terminalSettings má funkce getCharNow blokující čtení, které znemožňuje můj záměr v této části programu.
            Jelikož je program pouze na UNIX, tak jsem využil poměrně jednoduché možnosti ncurses s aplikkováním podle webu: https://sqlpey.com/c/non-blocking-keyboard-input-techniques/
        */
        stopZnak = getch();
        Pa_ReadStream(ukazatelNaStream, fftbuffer, velikostFFTbufferu);

        for (int i = 0; i < velikostFFTbufferu; i++)
        {
            float w = 0.5 * (1 - cos((double)2 * cisloPi * i / (velikostFFTbufferu - 1))); // Zde jsem musel využít ChatGPT pro vyhlazení průběhu pro zpracování přes kissfft
            komplexniRovinaVstup[i].r = fftbuffer[i] * w;
            komplexniRovinaVstup[i].i = 0;
        }
        kiss_fft(konfig, komplexniRovinaVstup, komplexniRovinaVystup);
        // zjisteni nejvetsi velikosti
        int indexMaxima = 0;
        double amplitudaMaxima = 0;
        for (int i = 0; i < velikostFFTbufferu / 2; i++)
        {
            double aktual = sqrt(pow((double)komplexniRovinaVystup[i].r, 2) + pow((double)komplexniRovinaVystup[i].i, 2)); // velikost komplexního čísla
            if (aktual > amplitudaMaxima)
            {
                amplitudaMaxima = aktual;
                indexMaxima = i;
            }
        }
        // DODĚLAT INTERVAL OD 1/8 FREKVENCE DO 15/8 FREKVENCE // na základě mého pozorování 
        int frekvence = indexMaxima * vzorkovaciFrekvence / velikostFFTbufferu; // portaudio ukládá frekvence podle indexu, takze zde vezmu index, který vynásobím vzorkovací frekvenci a nakonec vydělím velikostí bufferu
        mvprintw(0, 0, "Frekvence je %i                 ", frekvence);
        mvprintw(1, 0, "Je treba preladit o %i Hz       ", cilovaFrekvence - frekvence);
        refresh();
    }
    Pa_StopStream(ukazatelNaStream);
    endwin();
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
        Pa_OpenDefaultStream(&polestreamu[i], 1, 0, paFloat32, vzorkovaciFrekvence, velikostFFTbufferu, NULL, NULL);
    }
    return polestreamu;
}
