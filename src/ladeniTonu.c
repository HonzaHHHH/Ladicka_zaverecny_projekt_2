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
#include <unistd.h>
#include <syslog.h>

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
    int errorPortAudio = Pa_Initialize();
    if (errorPortAudio != paNoError)
    {
        printf("PortAudio error: %s\n", Pa_GetErrorText(errorPortAudio));
        syslog(LOG_ERR, "PortAudio chyba: %s (ladeni)", Pa_GetErrorText(errorPortAudio));
        exit(EXIT_ERRLIBS + EXIT_VLADENI + 1);
    }
    if (&poleHudebnichNastroju[aktualniHudebniNastroj] == NULL)
    {
        printf("Nepodařilo se otevřít nástroj (ladeni)");
        sleep(1);
        Pa_Terminate();
        syslog(LOG_ERR, "Nepodařilo se otevřít nástroj (ladeni)");
        return;
    }
    if (poleHudebnichNastroju[aktualniHudebniNastroj].poleTonu == 0)
    {
        printf("Nebyly zjištěny zádné nástroje, zkuste Nastavení\n");
        sleep(2);
        Pa_Terminate();
        syslog(LOG_ERR, "Nebyly zjištěny zádné nástroje (ladeni)");
        return;
    }
    PaStream **poleStreamu = nastaveniPortAudioStreamuLadeni(poleHudebnichNastroju[aktualniHudebniNastroj]);
    short cisloVNabidce = 0;
    char voliciZnak = 0;
    short konecFunkce = 1;
    while (konecFunkce)
    {
        clearScreen();
        for (int i = 0; i < poleHudebnichNastroju[aktualniHudebniNastroj].pocetTonu; i++)
        {
            if (cisloVNabidce == i)
                printf("\x1b[32m"
                       "\n%s"
                       "\x1b[0m",
                       poleHudebnichNastroju[aktualniHudebniNastroj].nazvyTonu[i]);
            else
                printf("\n%s", poleHudebnichNastroju[aktualniHudebniNastroj].nazvyTonu[i]);
        }
        voliciZnak = getCharNow();
        switch (voliciZnak)
        {
        case 's':
        case 'S':
            if (cisloVNabidce < poleHudebnichNastroju[aktualniHudebniNastroj].pocetTonu - 1)
                cisloVNabidce++;
            break;
        case 'w':
        case 'W':
            if (cisloVNabidce > 0)
                cisloVNabidce--;
            break;
        case '\n':
            clearScreen();
            laditTon(poleStreamu[cisloVNabidce], poleHudebnichNastroju[aktualniHudebniNastroj].poleTonu[cisloVNabidce]);
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
        long _min_index = cilovaFrekvence * velikostFFTbufferu / (8 * vzorkovaciFrekvence);
        long _max_index = 15 * cilovaFrekvence * velikostFFTbufferu / (8 * vzorkovaciFrekvence);
        for (int i = _min_index; i < _max_index; i++)
        {
            double aktual = sqrt(pow((double)komplexniRovinaVystup[i].r, 2) + pow((double)komplexniRovinaVystup[i].i, 2)); // velikost komplexního čísla
            if (aktual > amplitudaMaxima)
            {
                amplitudaMaxima = aktual;
                indexMaxima = i;
            }
        }
        int frekvence = indexMaxima * vzorkovaciFrekvence / velikostFFTbufferu; // portaudio ukládá frekvence podle indexu, takze zde vezmu index, který vynásobím vzorkovací frekvenci a nakonec vydělím velikostí bufferu
        frekvence += posun_frekvence; // pro ladeni frekvence z mikrofonu
        mvprintw(0, 0, "Frekvence je %i HZ                   ", frekvence);
        if ((cilovaFrekvence - frekvence) > 0)
        {
            mvprintw(1, 0, "Je treba naladit vys o %i Hz     ", cilovaFrekvence - frekvence);
        }
        else if ((cilovaFrekvence - frekvence) < 0)
        {
            mvprintw(1, 0, "Je treba naladit niz o %i Hz           ", (cilovaFrekvence - frekvence) * -1);
        }
        else    mvprintw(1, 0, "Spravne naladeno                ");
        refresh();
    }
    Pa_StopStream(ukazatelNaStream);
    endwin();
}

extern struct dataProStreamPrehravani *poleDatProStream;

PaStream **nastaveniPortAudioStreamuLadeni(struct hudebniNastroj nastroj)
{
    PaStream **polestreamu = malloc(sizeof(PaStream *) * nastroj.pocetTonu);
    poleDatProStream = malloc(sizeof(struct dataProStreamPrehravani) * nastroj.pocetTonu);
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
