#include "lista.h"

#include <stdlib.h>
#include <assert.h>

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
    assert(l->sentinela != NULL);

    l->sentinela->prox = l->sentinela;
    l->sentinela->ant = l->sentinela;
    l->sentinela->dado_t = NULL;
    l->tam = 0;
    return l;
}

