#ifndef NASTAVENI_H_
#define NASTAVENI_H_

// Nastaveni maker pro proměnnou nastavení

#define NASTAVENI_IGNSYS 1
#define NASTAVENI_DIALOGPLUS 2
#define NASTAVENI_CHCEPOUZEHELP 3
#define NASTAVENI_HELPASTART 4

/*
    Kvůli zlepšení debugování

    Nastavení exit kódu:
    XYZ
    X - způsob ukončení
        0 - normální ukončení ( v tomto případě budou i další číslice 00)
        1 - na základě špatného uživatelského vstupu
        2 - na základě chyby v tomto programu
        3 - na základě chyby v přejatých knihovnách a funkcích
    Y - sekce .c souboru
        0 - main.c
        1 - nastavení.c
        2 - prehravaniTonu.c
        3 - terminalSettings.c
    Z - konkrétní exitKod, je jedinečný pro každé ukončení

        Pro korektní ukončení programu vždy používám 0.
*/
#define EXIT_NORMALNI 0
#define EXIT_CHYBUSER 100
#define EXIT_ERRPROG 200
#define EXIT_ERRLIBS 300
#define EXIT_VMAIN 00
#define EXIT_VNAST 10
#define EXIT_VPLAY 20
#define EXIT_VTERMSETT 30
#define EXIT_VLADENI 40

#include <nacitaniStruktur.h>
#include <portaudio.h>

extern short nastaveni;
extern int vzorkovaciFrekvence;
extern int vzorkuNaBuffer;
extern double cisloPi;

struct dataProStreamPrehravani
{
    int frekvence;
    double faze;
};

struct dataProStreamLadeni
{
    int cilovaFrekvence;
    
};

void praceSParametrem(char *parametry);

PaStream ** nastaveniPortAudioStreamu(struct hudebniNastroj);

void nastaveniNastaveni();

#endif