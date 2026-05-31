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
#include <nacitaniStruktur.h>
#include <syslog.h>

/**
 * Proměnná, kam se ukládají nastavení přes bitové masky
 */
short nastaveni = 0;

const int vzorkovaciFrekvence = 44100;
int vzorkuNaBuffer = 256;
double cisloPi = (double)3.14159265358979323846264338327950288419716939937510f; // převzato z Wikipedie: https://cs.wikipedia.org/wiki/P%C3%AD_(%C4%8D%C3%ADslo)
int velikostFFTbufferu = 32768;

// opet zase pomoc od gemini
#define NA_STRING_HELPER(x) #x
#define NA_STRING(x) NA_STRING_HELPER(x)

int posun_frekvence = 0;

/**
 * Spouští nabídku nastavení
 */
void nastaveniNastaveniMain();

void vypsatNabidkuNastaveni(short index);

/**
 * Dostane parametry a podle toho zapne přepínače
 */
void praceSParametrem(char *parametry);

void posunTonu(void);

void novyNastroj(void);

/**
 * Vrátí ukazatele na streamy podle struktury hudebninastroj
 */
PaStream **nastaveniPortAudioStreamuPrehravani(struct hudebniNastroj nastroj);

void vybratAktualniNastroj(void);

//-----------------------------------------------------------------------------------------------------------------------------

void vycistitBuffer(void)
{
    int c;
    // Bude číst znaky z bufferu tak dlouho, dokud nenarazí na Enter (\n) nebo konec souboru (EOF)
    while ((c = getchar()) != '\n' && c != EOF)
        ;
}

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
        syslog(LOG_ERR, "Struktura s hudebním nástrojem je špatná");
        return NULL;
    }
    PaStream **polestreamu = malloc(sizeof(PaStream *) * nastroj.pocetTonu);
    if (polestreamu == NULL)
    {
        printf("Chyba: nepodarilo se alokovat poleStreamu\n");
        syslog(LOG_ERR, "Nepodařilo se alokovat pole streamů");
        return NULL;
    }
    poleDatProStream = malloc(sizeof(struct dataProStreamPrehravani) * nastroj.pocetTonu);
    if (poleDatProStream == NULL)
    {
        printf("Chyba: nepodarilo se alokovat poleDatProStream\n");
        free(polestreamu);
        syslog(LOG_ERR, "Nepodařilo se alokovat pole dat pro stream");
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
            syslog(LOG_ERR, "Chyba port audio: %s", Pa_GetErrorText(paErr));
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
                break;

            case 1:
            {
                posunTonu();
                break;
            }
            case 2:
            {
                novyNastroj();
                break;
            }
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
        for (int i = 0; i < pocetHudebnichNastroju; i++)
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
            syslog(LOG_INFO, "Uživatel změnil aktuální hudební nástroj");
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

void novyNastroj()
{
    char nazevNovehoNastroje[MAXIMALNI_DELKA_NAZVU_NASTROJE];
    printf("Zadejte název nového nástroje: ");
    fgets(nazevNovehoNastroje, sizeof(nazevNovehoNastroje), stdin);
    for (int __cistici__ = 0; __cistici__ < MAXIMALNI_DELKA_NAZVU_NASTROJE; __cistici__++)
    {
        if (nazevNovehoNastroje[__cistici__] == '\n')
            nazevNovehoNastroje[__cistici__] = '\0';
    }
    int novyPocetTonu = 0;
    while (novyPocetTonu <= 0)
    {
        printf("Zadejte počet tónů (musí být větší jak 0): ");
        short _kontrola_poctu_tonu = scanf("%i", &novyPocetTonu);
        vycistitBuffer();
        if (_kontrola_poctu_tonu != 1 || novyPocetTonu < 1)
        {
            novyPocetTonu = 0;
            syslog(LOG_WARNING, "Uživatel je debil a neumí napsat číslo větší než 0");
        }
    }
    int frekvence[novyPocetTonu];
    memset(frekvence, 0, sizeof(frekvence));
    char **noveNazvyTonu = malloc(novyPocetTonu * sizeof(char *));
    for (int __index_v_charu = 0; __index_v_charu < novyPocetTonu; __index_v_charu++)
    {
        noveNazvyTonu[__index_v_charu] = malloc(MAXIMALNI_DELKA_NAZVU_NASTROJE * sizeof(char));
        memset(noveNazvyTonu[__index_v_charu], 0, sizeof(MAXIMALNI_DELKA_NAZVU_TONU * sizeof(char)));
    }
    for (int __index_naplnovani = 0; __index_naplnovani < novyPocetTonu; __index_naplnovani++)
    {
        printf("Zadejte název %i. tónu: ", __index_naplnovani + 1);
        fgets(noveNazvyTonu[__index_naplnovani], MAXIMALNI_DELKA_NAZVU_NASTROJE, stdin);
        for (int __cistici__ = 0; __cistici__ < MAXIMALNI_DELKA_NAZVU_NASTROJE; __cistici__++)
        {
            if (noveNazvyTonu[__index_naplnovani][__cistici__] == '\n')
                noveNazvyTonu[__index_naplnovani][__cistici__] = '\0';
        }
        while (frekvence[__index_naplnovani] <= 0)
        {
            printf("Zadejte frekvenci tohoto tónu: ");
            short ___kontrola_nacteni_frekvence = scanf("%i", &frekvence[__index_naplnovani]);
            vycistitBuffer();
            if (___kontrola_nacteni_frekvence != 1 || frekvence[__index_naplnovani] < 0)
            {
                frekvence[__index_naplnovani] = 0;
            }
        }
    }
    char nazevSouboru[MAXIMALNI_DELKA_NAZVU_NASTROJE];
    memset(nazevSouboru, 0, sizeof(nazevSouboru));
    printf("Zadejte název souboru, kam se data uloží (i s příponou): ");
    fgets(nazevSouboru, MAXIMALNI_DELKA_NAZVU_NASTROJE, stdin);
    for (int __cistici__ = 0; __cistici__ < MAXIMALNI_DELKA_NAZVU_NASTROJE; __cistici__++)
    {
        if (nazevSouboru[__cistici__] == '\n')
            nazevSouboru[__cistici__] = '\0';
    }
    FILE *souborNaNovyNastroj = fopen(nazevSouboru, "w");
    if (souborNaNovyNastroj == NULL)
    {
        printf("Chyba");
        syslog(LOG_ERR, "Nepodařilo se vytvořit soubor pro zápis nástroje");
        return;
    }
    fprintf(souborNaNovyNastroj, "%i;%s;", novyPocetTonu, nazevNovehoNastroje);
    for (int __index_zapisovani_frekvenci_do_souboru = 0; __index_zapisovani_frekvenci_do_souboru < novyPocetTonu; __index_zapisovani_frekvenci_do_souboru++)
    {
        fprintf(souborNaNovyNastroj, "%i;", frekvence[__index_zapisovani_frekvenci_do_souboru]);
    }
    for (int __index_zapisovani_nazvu_do_souboru = 0; __index_zapisovani_nazvu_do_souboru < novyPocetTonu; __index_zapisovani_nazvu_do_souboru++)
    {
        fprintf(souborNaNovyNastroj, "%s;", noveNazvyTonu[__index_zapisovani_nazvu_do_souboru]);
    }
    fclose(souborNaNovyNastroj);
    FILE *hlavniSoubor = fopen("nastroje.lad", "r+");
    if (hlavniSoubor == NULL)
    {
        printf("Chyba");
        syslog(LOG_ERR, "Nepodařilo se otevřít soubor nastroje.lad pro čtení");
        return;
    }

    rewind(hlavniSoubor);
    int staryPocetNastroju = 0;
    short __kontrola_stareho_poctu = fscanf(hlavniSoubor, "%i;", &staryPocetNastroju);
    if (__kontrola_stareho_poctu != 1 || staryPocetNastroju < 1)
    {
        printf("Chyba v načítání staré konfigurace\n");
        syslog(LOG_ERR, "Chyba v načítání staré konfigurace");
        return;
    }
    char **stare_nazvy = malloc(staryPocetNastroju * sizeof(char *));
    for (int __nacitani_starych_nazvu = 0; __nacitani_starych_nazvu < staryPocetNastroju; __nacitani_starych_nazvu++)
    {
        stare_nazvy[__nacitani_starych_nazvu] = malloc(MAXIMALNI_DELKA_NAZVU_NASTROJE * sizeof(char));
        fscanf(hlavniSoubor, "%" NA_STRING(MAXIMALNI_DELKA_NAZVU_NASTROJE) "[^;];", stare_nazvy[__nacitani_starych_nazvu]); // opet radek od gemini
    }
    fclose(hlavniSoubor);
    hlavniSoubor = fopen("nastroje.lad", "w");
    if (hlavniSoubor == NULL)
    {
        printf("Chyba");
        syslog(LOG_ERR, "Chyba v otevírání souboru pro zápis");
        return;
    }
    rewind(hlavniSoubor);
    fprintf(hlavniSoubor, "%i;", staryPocetNastroju + 1);
    for (int __zapisovani_Souboru = 0; __zapisovani_Souboru < staryPocetNastroju; __zapisovani_Souboru++)
    {
        fprintf(hlavniSoubor, "%s;", stare_nazvy[__zapisovani_Souboru]);
    }
    fprintf(hlavniSoubor, "%s;", nazevSouboru);
    fclose(hlavniSoubor);
    printf("Funkce dokončena, pro načtení nových souboru restartujte aplikaci");
    fflush(stdout);
    sleep(2);

    // úklid
    
    for (int __index_cisteni = 0; __index_cisteni < novyPocetTonu; __index_cisteni++)
    {
        free(noveNazvyTonu[__index_cisteni]);
    }
    free(noveNazvyTonu);
    noveNazvyTonu = NULL;
    
    for (int __cisteni_starych_nazvu = 0; __cisteni_starych_nazvu < staryPocetNastroju; __cisteni_starych_nazvu++)
    {
        free(stare_nazvy[__cisteni_starych_nazvu]);
    }
    free(stare_nazvy);
    stare_nazvy = NULL;
}

void posunTonu(void)
{
    printf("Zadejte posunutí frekvence: ");
    int staraFrekvence = posun_frekvence;
    short __kontrola = scanf("%i", &posun_frekvence);
    if (__kontrola != 1)
    {
        posun_frekvence = staraFrekvence;
        printf("Chyba ve vstupu, nic se neuložilo\n");
        return;
    }
    printf("Uloženo");
    sleep(2);
    vycistitBuffer();
    syslog(LOG_INFO, "Změnil se trim frekvence");
    FILE * ulozeni_trim = fopen("trim.lad", "w");
    if (ulozeni_trim == NULL)
    {
        syslog(LOG_ERR, "Nepodařilo se uložit aktuální trim");
        return;
    }
    fprintf(ulozeni_trim, "%i", posun_frekvence);
    fclose(ulozeni_trim);
    syslog(LOG_INFO, "Trim frekvence se úspěšně zapsal");
    return;
}