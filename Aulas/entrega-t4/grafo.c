#include "grafo.h"
#include <stdlib.h>
#include <assert.h>
#include <string.h>

// funções que implementam as operações básicas de um grafo
//
// um grafo é constituido por nós e arestas que os interligam.
// existe um valor associado a cada nó e a cada aresta. Esses valores são
//   armazenados pelo grafo, mas o grafo desconhece seu tipo, só conhece
//   seu tamanho.
// os nós são identificados por um inteiro que corresponde à ordem em que
//   são inseridos no grafo, com 0 correspondendo ao primeiro.
// a identificação de um nó é sempre um número inferior ao número de nós
//   no grafo. A remoção de um nó altera a identificação dos nós com números
//   superiores.
// as arestas são direcionadas, e são identificadas pelo seu nó de origem
//   e de destino.
// os valores das arestas e dos nós são copiados para o grafo. O acesso a
//   esses valores é por meio de ponteiros. O usuário do grafo pode alterar
//   o valor armazenado no grafo após obter o ponteiro (com g_valor_do_nó
//   por exemplo). O usuário pode também alterar o valor usando uma função
//   de alteração (como g_altera_valor_do_nó).

typedef struct no no;
typedef struct aresta aresta;

struct aresta{
    int no_destino;
    void *dado;
    aresta *prox_aresta;
};

struct no{
    void *dado;
    aresta *prim_aresta;
};

struct grafo{
    no *nos;
    int qnt_nos;
    int tam_dado_nos;
    int tam_dado_arestas;
};


// cria um grafo vazio que suporta dados do tamanho fornecido (em bytes)
//   nos nós e nas arestas
Grafo g_cria(int tam_nó, int tam_aresta)
{
    Grafo g;
    g = malloc(sizeof(*g));
    assert(g != NULL);
    g->nos = NULL;
    g->qnt_nos = 0;
    g->tam_dado_nos = tam_nó;
    g->tam_dado_arestas = tam_aresta;
    return g;
}

// libera a memória ocupada por um grafo
void g_destrói(Grafo self);

// Nós

// insere um nó no grafo, com o dado apontado por p_dado
// retorna o número do novo nó (que é o número de nós que
//   existia antes da inserção)
int g_insere_nó(Grafo self, void *p_dado)
{
    int indice_novo_no = self->qnt_nos;

    no *n = realloc(self->nos, (indice_novo_no + 1) * sizeof(*n));
    assert(n != NULL);

    self->nos = n;
    no *novo_no = &self->nos[indice_novo_no];

    novo_no->dado = malloc(self->tam_dado_nos);
    assert(novo_no->dado != NULL);

    memcpy(novo_no->dado, p_dado, self->tam_dado_nos);

    novo_no->prim_aresta= NULL;
    self->qnt_nos++;

    return indice_novo_no;
}

// remove um nó do grafo e as arestas incidentes nesse nó
// a identificação dos nós remanescentes é alterada, como se esse nó nunca
//   tivesse existido
void g_remove_nó(Grafo self, int nó);

// altera o valor associado a um nó (copia o valor apontado por p_dado para
//   uma memória gerenciada pelo grafo)
void g_altera_valor_do_nó(Grafo self, int nó, void *p_dado)
{
    assert(nó >= 0 && nó < self->qnt_nos);
    no *n = &self->nos[nó];
    memcpy(n->dado,p_dado, self->tam_dado_nos);
}

// retorna um ponteiro para o valor associado a um nó que está na memória
//   gerenciada pelo grafo.
void *g_valor_do_nó(Grafo self, int nó)
{
    assert(nó >= 0 && nó < self->qnt_nos);
    no *n = &self->nos[nó];
    return n->dado;
}

// retorna o número de nós do grafo
int g_num_nós(Grafo self)
{
    return self->qnt_nos;
}

// Arestas

// altera o valor da aresta que interliga o nó origem ao nó destino
//   (copia de *p_dado para uma memória gerenciada pelo grafo)
// caso a aresta não exista, deve ser criada
// caso p_dado seja NULL, a aresta deve ser removida
void g_altera_valor_da_aresta(Grafo self, int nó_origem, int nó_destino, void *p_dado)
{
    assert(nó_origem >= 0 && nó_origem < self->qnt_nos);
    assert(nó_destino >= 0 && nó_destino < self->qnt_nos);

    no *n_origem = &self->nos[nó_origem];
    aresta *atual = n_origem->prim_aresta;
    aresta *anterior = NULL;

    while(atual != NULL && atual->no_destino != nó_destino) {
        anterior = atual;
        atual = atual->prox_aresta;
    }
}

// retorna um ponteiro para o valor associado à aresta entre nó_origem e 
//   nó_destino, localizado em memória gerenciada pelo grafo.
// retorna NULL se tal aresta não existir
void *g_valor_da_aresta(Grafo self, int nó_origem, int nó_destino)
{
    assert(nó_origem >= 0 && nó_origem < self->qnt_nos);
    assert(nó_destino >= 0 && nó_destino < self->qnt_nos);

    no *n_origem = &self->nos[nó_origem];
    no *n_destino = &self->nos[nó_destino];
    aresta *aresta_teste = n_origem->prim_aresta;

    while(aresta_teste != NULL) {
        no *n_destino_teste = &self->nos[aresta_teste->no_destino];
        if(n_destino_teste == n_destino){
            return aresta_teste->dado;
        } else {
            aresta_teste = aresta_teste->prox_aresta;
        }
    }

    return NULL;
}

// inicia uma consulta a arestas que partem do nó origem
// as próximas chamadas a 'g_próxima_aresta' devem retornar os valores
//   correspondentes a cada aresta que parte desse nó
void g_arestas_que_partem(Grafo self, int nó_origem);

// inicia uma consulta a arestas que chegam ao nó destino
// as próximas chamadas a 'g_próxima_aresta' devem retornar os valores
//   correspondentes a cada aresta que chega nesse nó
void g_arestas_que_chegam(Grafo self, int nó_destino);

// retorna um ponteiro para o dado associado à próxima aresta, de acordo
//   com a última consulta iniciada por 'g_arestas_que_partem' ou 
//   'g_arestas_que_chegam'
// o valor do nó vizinho ao nó da consulta deve ser colocado em '*p_nó_vizinho'
//   (se não for NULL)
// caso não exista mais aresta que satisfaça a consulta, retorna NULL.
void *g_próxima_aresta(Grafo self, int *p_nó_vizinho);

// Algoritmos

// retorna true se grafo é cíclico, false caso contrário
bool g_tem_ciclo(Grafo self);

// retorna uma fila de inteiros contendo os números dos nós do grafo em
//   uma ordem em que, se o nó 'a' antecede 'b', não existe uma aresta
//   de 'b' para 'a' no grafo
// deve retornar uma fila vazia caso tal ordem não exista
// quem chama esta função é responsável por destruir a fila.
Fila g_ordem_topológica(Grafo self);
