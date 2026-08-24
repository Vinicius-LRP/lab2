// includes, constantes e declarações {{{1
#include "str.h"

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <assert.h>

#define MIN_ALLOC 8    // alocação mínima

struct str {
    int numBytes;
    int alocado;
    byte *dados;
};

// A memória para conter os bytes de uma string deve ser alocada e/ou
//   realocada conforme a necessidade, cuidando para que a quantidade
//   de memória alocada seja sempre:
//   - nula (não alocada) se a string for vazia, ou
//   - não inferior ao necessário para armazenar os bytes da codificação utf8;
//   - não inferior à alocação mínima;
//   - não superior ao triplo do número de bytes necessários
//     (exceto quando for o mínimo);
//   - uma potência de 2.

// funções auxiliares {{{1

void s_realoca(Str s, int nBytNecess)
{
    if (s->alocado == 0 && s->alocado < nBytNecess) s->alocado = MIN_ALLOC;
    while (s->alocado < nBytNecess) {
        s->alocado *= 2;
    }
    if (nBytNecess == 0) { 
        free(s->dados);
        s->dados = NULL;
        s->alocado = 0;
        return;
    } else {
        while (nBytNecess <= s->alocado / 2 && s->alocado / 2 >= MIN_ALLOC) {
            s->alocado /= 2;
        } 
    }
    byte *novo = realloc(s->dados, s->alocado);
    if (novo == NULL) {
        printf("Erro em allocar memoria!");
        exit(1);
    }
    s->dados = novo;
}


// verifica se a string cad está de acordo com a especificação
// aborta o programa se não tiver
static void s_ok(Str_c s)
{
    assert(s != NULL);
    assert(s->numBytes >= 0);
    assert(s->alocado >= 0);
    assert(s->numBytes <= s->alocado);
    if (s->alocado == 0){
        assert(s->numBytes == 0);
        assert(s->dados == NULL);
        return;
    } else {
        assert(s->dados != NULL);
        assert(s->alocado >= MIN_ALLOC);
        int a = s->alocado;
        while (a > 1) {
            assert(a % 2 == 0);
            a /= 2;
        }    
    }
    if (s->numBytes * 3 < MIN_ALLOC) {
        assert(s->alocado == MIN_ALLOC);
    } else {
        assert(s->alocado <= s->numBytes * 3);
    }
}

//...

// operações de criação e destruição {{{1

Str s_cria(char const *strC)
{
    Str s = malloc(sizeof(*s));
    assert(s != NULL);
    s->alocado = 0;
    s->dados = NULL;
    s->numBytes = 0;

    if (strC == NULL) return s;

    int nBytes = strlen(strC);
    if(nBytes == 0) return s;
    
    if(u8_conta_unichar_nos_bytes(nBytes, (byte *) strC) == -1){
        return s;
    }
    s->numBytes = nBytes;
    s_realoca(s, s->numBytes);
    memcpy(s->dados, strC, s->numBytes);

    s_ok(s);
    return s;
}

void s_destroi(Str s)
{
    s_ok(s);
    free(s->dados);
    free(s);
}

Str s_cria_substring(Str_c s, int pos, int tam)
{
   Str nova = s_cria("");
   s_substring(nova, s, pos, tam);
   return nova;
}

Str s_cria_cópia(Str_c s)
{
   return s_cria_substring(s, 0, -1);
}

// Retorna uma nova string com o conteúdo do arquivo chamado nome.
// Retorna uma string vazia em caso de erro.
Str s_cria_de_arquivo(char *nome)
{
    Str s = s_cria("");
    FILE *arq = fopen(nome, "rb");
    
    if(arq == NULL){
      return s;
    }
    
    byte buffer[1024];

    size_t lidos;

    while ((lidos = fread(buffer,1 , sizeof(buffer), arq)) > 0)

    return s;
}

// operações de acesso {{{1

int s_tam(Str_c s)
{
  s_ok(s);
  //...
  return 0;
}

char *s_strc(Str_c s)
{
  s_ok(s);
  //...
  return NULL;
}

unichar s_ch(Str_c s, int pos)
{
  s_ok(s);
  //...
  return UNI_INV;
}


// operações de busca e comparação {{{1

bool s_igual(Str_c s, Str_c sb)
{
  s_ok(s);
  s_ok(sb);
  //...
  return false;
}

int s_busca_c(Str_c s, int pos, Str_c sb)
{
  s_ok(s);
  s_ok(sb);
  //...
  return -1;
}

int s_busca_nc(Str_c s, int pos, Str_c sb)
{
  s_ok(s);
  s_ok(sb);
  //...
  return -1;
}

int s_busca_rc(Str_c s, int pos, Str_c sb)
{
  s_ok(s);
  s_ok(sb);
  //...
  return -1;
}

int s_busca_rnc(Str_c s, int pos, Str_c sb)
{
  s_ok(s);
  s_ok(sb);
  //...
  return -1;
}

int s_busca_s(Str_c s, int pos, Str_c buscada)
{
  s_ok(s);
  s_ok(buscada);
  //...
  return -1;
}


// operações de alteração {{{1

void s_substitui(Str s, int pos, int tam, Str_c sb)
{
  s_ok(s);
  s_ok(sb);
  //...
}

void s_substring(Str s, Str_c sb, int pos, int tam)
{
  s_ok(s);
  s_ok(sb);
  //...
}

void s_copia(Str s, Str_c sb)
{
  s_substring(s, sb, 0, -1);
}

void s_insere(Str s, int pos, Str_c sb)
{
  s_substitui(s, pos, 0, sb);
}

void s_insere_c(Str s, int pos, unichar c)
{
  s_ok(s);
  //...
}

void s_anexa(Str s, Str_c sb)
{
  s_substitui(s, -1, 0, sb);
}

void s_anexa_c(Str s, unichar c)
{
  s_insere_c(s, -1, c);
}

void s_remove(Str s, int pos, int tam)
{
  s_substitui(s, pos, tam, NULL);
}

void s_apara(Str s, Str_c sobras)
{
  s_ok(s);
  s_ok(sobras);
  //...
}

// operações de E/S {{{1

void s_imprime(Str_c s)
{
  s_ok(s);
  //...
}

void s_grava_arquivo(Str_c s, char *nome)
{
  s_ok(s);
  //...
}


// vim: foldmethod=marker shiftwidth=2

