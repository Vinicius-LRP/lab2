#include "lista.h"

#include <stdlib.h>
#include <assert.h>

struct no{
    dado_t dado;
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
    l->sentinela->dado = NULL;
    l->tam = 0;
    return l;
}

//insere um no nv antes de um no de ref (inicio ref sent prox/fim ref sent)
static void insere_antes(struct no *ref, dado_t d)
{
    struct no *novo = malloc(sizeof(*novo));
    assert(novo != NULL);
    novo->dado = d;
     
    struct no *anterior = ref->ant;

    novo->prox = ref;
    novo->ant = anterior;
    anterior->prox = novo;
    ref->ant = novo;
}

void l_insere_inicio(Lista l, dado_t d)
{
    insere_antes(l->sentinela->prox, d);
    l->tam++;
}

void l_insere_fim(Lista l, dado_t d)
{
    insere_antes(l->sentinela, d);
    l->tam++;
}

int l_tam(Lista l)
{
    return l->tam;
}

bool l_vazia(Lista l)
{
    if(l->tam == 0) return true;
}

