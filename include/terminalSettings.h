#ifndef TERMINALSETTINGS_H_
#define TERMINALSETTINGS_H_

void setupTerminalFunctions(void);  // Setup

void vypnoutKanonickyRezim(void);

void zapnoutKanonickyRezim(void);

void clearScreen(void);

char getCharNow(void);

void poziceKurzoru(int osax, int osay);

void spanek(int sekundy);
#endif