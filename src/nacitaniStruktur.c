#include <stdlib.h>
#include <nacitaniStruktur.h>
#include <string.h>

struct hudebniNastroj gitara;

void setupKytara(void)
{
    gitara.pocetTonu = 6;
    gitara.poleTonu = malloc(sizeof(int) * gitara.pocetTonu);
    gitara.poleTonu[0] = 82;
    gitara.poleTonu[1] = 110;
    gitara.poleTonu[2] = 147;
    gitara.poleTonu[3] = 196;
    gitara.poleTonu[4] = 247;
    gitara.poleTonu[5] = 330;
    strcpy(gitara.nazev, "Kytara 6 strunná");
    gitara.nazvyTonu = malloc(sizeof(char *) * gitara.pocetTonu);
    for (int i = 0; i < gitara.pocetTonu; i++)
    {
        gitara.nazvyTonu[i] = malloc(sizeof(char) * 10);
    }
    strcpy(gitara.nazvyTonu[0], "Struna E2");
    strcpy(gitara.nazvyTonu[1], "Struna A2");
    strcpy(gitara.nazvyTonu[2], "Struna D3");
    strcpy(gitara.nazvyTonu[3], "Struna G3");
    strcpy(gitara.nazvyTonu[4], "Struna B3");
    strcpy(gitara.nazvyTonu[5], "Struna E4");
}