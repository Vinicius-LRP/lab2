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

// remove um no da lista e retorna o d
static dado_t remove_no(no *n)
{
    no *anterior = n->ant;
    no *proximo = n->prox;

    anterior->prox = proximo;
    proximo->ant = anterior;

    dado_t d = n->dado;
    return d;
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

dado_t l_dado_pos(Lista l, int pos) 
{
    assert(pos >= 0 && pos < l->tam);
    no *n = no_na_pos(l, pos);
    return n->dado;
}

void l_insere_pos(Lista l, dado_t d, int p)
{
    assert(p >= 0 && p <= l->tam);

    no *ref;
    if(p == l->tam) {
        ref = l->sentinela;
    } else {
        ref = no_na_pos(l, p);
    }

    insere_antes(ref, d);
    l->tam++;
}

dado_t l_remove_inicio(Lista l)
{
    assert(!l_vazia(l));
    l->tam--;
    return remove_no(l->sentinela->prox);
}

dado_t l_remove_fim(Lista l)
{
    assert(!l_vazia(l));
    l->tam--;
    return remove_no(l->sentinela->ant);
}

dado_t l_remove_pos(Lista l, int pos)
{
    assert(pos >= 0 && pos < l->tam);
    l->tam--;
    return l_remove_inicio(no_na_pos(l, pos));
}


void l_destroi(Lista l)
{
    no *n = l->sentinela->prox;
    while(n != l->sentinela) {
        no *proximo = n->prox;
        free(n);
        n = proximo;
    }
    free(l->sentinela);
    free(l);
}
