#include "fila.h"
#include <stdio.h>
#include <stdlib.h>
#include <assert.h>
#include <string.h>

typedef struct no no;

struct no{
    void *dado;
    no *prox;
    no *ant;
};

struct fila{
    no *sentinela;
    int tam_dado;
    int tam;
};


// funções auxiliares

no *cria_no_com_dado(Fila f, void *pdado){
    no *n = malloc(sizeof(*n));
    assert(n != NULL);

    n->dado = malloc(f->tam_dado);
    assert(n->dado != NULL);

    if(pdado != NULL) {
        memcpy(n->dado, pdado,  f->tam_dado);
    }
    return n;
}

//-----------------------------


// cria uma fila vazia que suporta dados do tamanho fornecido (em bytes)
Fila f_cria(int tam_do_dado)
{
    Fila f = malloc(sizeof(*f));
    assert(f != NULL);

    f->sentinela = malloc(sizeof(*f->sentinela));
    assert(f->sentinela != NULL);

    f->sentinela->prox = f->sentinela;
    f->sentinela->ant = f->sentinela;
    f->sentinela->dado = NULL;
    f->tam_dado = tam_do_dado;
    f->tam = 0;

    return f;
}

// libera a memória ocupada pela fila
void f_destrói(Fila self)
{
    no *n = self->sentinela->prox;
    while (n != self->sentinela) {
        no *proximo = n->prox;
        free(n->dado);
        free(n);
        n = proximo;
    }
    free(self->sentinela);
    free(self);
}

// diz se a fila está vazia
bool f_tá_vazia(Fila self)
{
    if(self->tam == 0) return true;
    return false;
}

// remove o dado no início da fila e, se pdado não for NULL, copia o dado
//   removido para *pdado
void f_remove(Fila self, void *pdado)
{
    if (f_tá_vazia(self)) return;

    no *no_para_remover = self->sentinela->prox;
    no *novo_prox = no_para_remover->prox;
    self->sentinela->prox = novo_prox;
    novo_prox->ant = self->sentinela;
    
    if(pdado != NULL) {
        memcpy(pdado, no_para_remover->dado, self->tam_dado);
    }

    self->tam--;

    free(no_para_remover->dado);
    free(no_para_remover);
}

// insere o dado apontado por pdado no final da fila
void f_insere(Fila self, void *pdado)
{
    no *ultimo_fila = self->sentinela->ant;
    no *no_para_inserir = cria_no_com_dado(self, pdado);
    
    ultimo_fila->prox = no_para_inserir;
    no_para_inserir->ant = ultimo_fila;

    no_para_inserir->prox = self->sentinela;
    self->sentinela->ant = no_para_inserir;
    self->tam++;
}

// funções que implementam operações complementares, que permitem acesso
//   a todos os elementos da fila. Durante um percurso, a fila não pode ser
//   alterada.

// inicia um percurso aos elementos da fila, a partir de uma posição inicial
// se a posição for positiva, o percurso vai dessa posição até o fim da fila
// se a posição for negativa, o percurso vai dessa posição até o início
//   0 é a posição do primeiro dado (aquele que está na fila há mais tempo)
//   1 é a posição do segundo dado, etc
//   além disso,
//   -1 é a posição do último dado (o que está na fila há menos tempo)
//   -2 é a posição do penúltimo dado, etc
// cada dado do percurso será acessado por chamadas a f_próximo()
void f_inicia_percurso(Fila self, int pos_inicial)
{
    
}

// caso o percurso tenha terminado, retorna false
// senão, coloca o próximo dado do percurso em *pdado (se pdado não for NULL),
//   e retorna true
bool f_próximo(Fila self, void *pdado)
{

}
