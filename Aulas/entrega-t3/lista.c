#include "lista.h"

#include <stdlib.h>
#include <assert.h>

typedef struct no no;

struct no{
    dado_t dado;
    no *prox;
    no *ant;
};

struct lista{
    no *sentinela;
    int tam;
};


//funcoes auxiliares

//insere um no nv antes de um no de ref (inicio ref sent prox/fim ref sent)
static void insere_antes(no *ref, dado_t d)
{
    no *novo = malloc(sizeof(*novo));
    assert(novo != NULL);
    novo->dado = d;
     
    no *anterior = ref->ant;

    novo->prox = ref;
    novo->ant = anterior;
    anterior->prox = novo;
    ref->ant = novo;
}

// retorna um ponteiro para no na posi passada 
static no *no_na_pos(Lista l, int pos)
{
    no *n = l->sentinela->prox;

    for (int i = 0; i < pos; i++){
        n = n->prox;
    }
    return n;
}

//--------------------------------------

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
    return false;
}

bool l_cheia(Lista l)
{
    return false;
}

dado_t l_dado_inicio(Lista l)
{
    assert(!l_vazia(l));
    return l->sentinela->prox->dado;

}

dado_t l_dado_fim(Lista l)
{
    assert(!l_vazia(l));
    return l->sentinela->ant->dado;
}

