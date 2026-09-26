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

Lista l_cria(){
    Lista l = malloc(sizeof(*l));
    assert(l != NULL);

    l->sentinela = malloc(sizeof(*l->sentinela));
    assert(l != NULL);


}

