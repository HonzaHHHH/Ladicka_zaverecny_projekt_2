#include "terminalSettings.h"
#include <stdlib.h>
#include <unistd.h>
#include <stdio.h>

#ifdef __linux__
#include <termios.h>
struct termios staryTerminal, NovyTerminal;
#endif
void vypnoutKanonickyRezim(void)
{
// Pro vypnutí kanonického režimu

// vypnutí
#ifdef __linux__
    tcsetattr(STDIN_FILENO, TCSANOW, &NovyTerminal);
#endif
}

void zapnoutKanonickyRezim(void)
{
#ifdef __linux__
    tcsetattr(STDIN_FILENO, TCSANOW, &staryTerminal);
#endif
}

void setupTerminalFunctions(void)
{
#ifdef __linux__
    tcgetattr(STDIN_FILENO, &staryTerminal);
    tcgetattr(STDIN_FILENO, &NovyTerminal);
    NovyTerminal.c_lflag &= ~(ICANON | ECHO);
#endif
}

#ifdef _WIN32

#include <conio.h>
#endif

void clearScreen(void)
{
#ifdef __linux__
    system("clear");
#else
    system("cls");
#endif
}
char getCharNow(void)
{
    char c;
#ifdef __linux__
    vypnoutKanonickyRezim();
    c = getchar();
    zapnoutKanonickyRezim();
#endif
#ifdef _WIN32
    c = getch();
#endif
    return c;
}
void poziceKurzoru(int OsaX, int OsaY)
{
    printf("\033[%i;%iH", OsaY, OsaX);
    return;
}

void spanek(int sekundy)
{
#ifdef Win32
#include <windows.h>
    Sleep(sekundy ·1000);
#endif
#ifdef __linux__
#include <unistd.h>
    sleep(sekundy);
#endif
}