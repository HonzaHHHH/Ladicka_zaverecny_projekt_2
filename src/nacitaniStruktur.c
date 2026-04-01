#include <stdlib.h>
#include <nacitaniStruktur.h>

struct hudebniNastroj gitara;

void setupKytara(void) {
    gitara.pocetTonu = 6;
    gitara.poleTonu = malloc(sizeof(int) * gitara.pocetTonu);
}