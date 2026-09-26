#include "lista.h"

struct no{
    dado_t dado_t;
    struct no *prox;
    struct no *ant;
};

struct lista{
    struct no *sentinela;
    int tam;
};



