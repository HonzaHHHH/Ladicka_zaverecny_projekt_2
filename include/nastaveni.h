/*
Část pro nastavení - zde jsou definována různá makra a proměnné
 */

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


#define VZORKOVACIFREK 44100
extern short nastaveni;
extern const int vzorkovaciFrekvence; // nemá smysl, na kritických místech je třeba používat makro
extern int vzorkuNaBuffer;
extern double cisloPi;
extern int velikostFFTbufferu;

struct dataProStreamPrehravani
{
    int frekvence;
    double faze;
};

void praceSParametrem(char *parametry);

PaStream ** nastaveniPortAudioStreamuPrehravani(struct hudebniNastroj);

void nastaveniNastaveniMain();

#endif