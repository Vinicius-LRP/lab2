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

static void s_realoca(Str s, int nBytNecess)
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

static bool s_pertence(unichar c, Str_c s){
    int tam = s_tam(s);
    for (int i = 0; i < tam; i++) {
        if (s_ch(s,i) == c) return true;
    }
    return false;
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
    if(arq == NULL) return s;
    fseek(arq, 0, SEEK_END);
    long tamanho = ftell(arq);
    fseek(arq, 0, SEEK_SET);
    if(tamanho <= 0){
        fclose(arq);
        return s;
    }
    s_realoca(s, tamanho);
    size_t lido = fread(s->dados, 1, (size_t)tamanho, arq);
    fclose(arq);
    if(lido != (size_t) tamanho) {
        s_realoca(s, 0);
        return s;
    }
    int ncar = u8_conta_unichar_nos_bytes(tamanho, s->dados);
    if(ncar == -1){
        s_realoca(s , 0);
        return s;
    }
    s->numBytes = tamanho;
    s_ok(s);
    return s;
}

// operações de acesso {{{1

int s_tam(Str_c s)
{
    s_ok(s);
    int numCaracteres = u8_conta_unichar_nos_bytes(s->numBytes, s->dados);
    return numCaracteres;
}

char *s_strc(Str_c s)
{
    s_ok(s);
    char *string = malloc(s->numBytes + 1);

    if (string == NULL) {
        printf("Erro em alocar memoria para string!");
        return NULL;
    }

    for (int i = 0; i < s->numBytes; i++) {
        string[i] = s->dados[i];
    }

    string[s->numBytes] = '\0';
    
    return string;
}

unichar s_ch(Str_c s, int pos)
{
    s_ok(s);
    int tam = s_tam(s);
    int indice;
    if (pos >= 0) {
        indice = pos;
    } else {
        indice = tam + pos + 1;
    }
    if (indice < 0 || indice >= tam) return UNI_INV;
    byte *p = u8_avanca_unichar(s->dados, indice);
    if (p == NULL) return UNI_INV;
    int bytesRestantes = s->numBytes - (p - s->dados);
    unichar uni;
    int nBytes = u8_unichar_nos_bytes(bytesRestantes, p, &uni);
    if (nBytes == -1) return UNI_INV;
    return uni;
}


// operações de busca e comparação {{{1

bool s_igual(Str_c s, Str_c sb)
{
    s_ok(s);
    s_ok(sb);
    
    if (s->numBytes != sb->numBytes) return false;

    for (int i = 0; i < s->numBytes; i++){
        if(sb->dados[i] != s->dados[i]) return false;
    }

    return true;
}

int s_busca_c(Str_c s, int pos, Str_c sb)
{
    s_ok(s);
    s_ok(sb);
    int tam = s_tam(s);
    int ind;
    if (pos >= 0) {
        ind = pos;
    } else {
        ind = tam + pos + 1;
    }
    if (ind < 0) {
        ind = 0;
    }
    if (ind >= tam) return -1;

    for (int i = ind; i < tam; i++) {
        if (s_pertence(s_ch(s , i), sb)) return i;
    }
    return -1;
}

int s_busca_nc(Str_c s, int pos, Str_c sb)
{
    s_ok(s);
    s_ok(sb);
    int tam = s_tam(s);
    int ind;
    if (pos >= 0) {
        ind = pos;
    } else {
        ind = tam + pos + 1;
    }
    if (ind < 0) {
        ind = 0;
    }
    if (ind >= tam) return -1;

    for (int i = ind; i < tam; i++) {
        if (!s_pertence(s_ch(s, i), sb)) return i;
    }

    return -1;
}

int s_busca_rc(Str_c s, int pos, Str_c sb)
{
    s_ok(s);
    s_ok(sb);

    int tam = s_tam(s);

    int ind;
    if (pos >= 0) {
        ind = pos;
    } else {
        ind = tam + pos + 1;
    }
    if (ind > tam) {
        ind = tam;
    }

    for (int i = ind - 1; i >= 0; i--) {
        if (s_pertence(s_ch(s, i), sb)) return i;
    }    
    return -1;
}

int s_busca_rnc(Str_c s, int pos, Str_c sb)
{
    s_ok(s);
    s_ok(sb);
    int tam = s_tam(s);

    int ind;
    if (pos >= 0) {
        ind = pos;
    } else {
        ind = tam + pos + 1;
    }

    if (ind > tam) {
        ind = tam;
    }

    for (int i = ind - 1; i >= 0; i--) {
        if (!s_pertence(s_ch(s, i), sb)) return i;
    }

    return -1;
}

int s_busca_s(Str_c s, int pos, Str_c buscada)
{
    s_ok(s);
    s_ok(buscada);
    int tamS = s_tam(s);
    int tamB = s_tam(buscada);

    int ind;

    if(pos >= 0) {
        ind = pos;
    } else {
        ind = tamS + pos + 1;
    }
    if (ind < 0) {
        ind = 0;
    }
    if(ind > tamS) {
        ind = tamS;
    }
    if (tamB == 0) return ind;
    for (int i = ind; i + tamB <= tamS; i++) {
        bool igual = true;
        for (int j = 0; j < tamB; j++) {
            if (s_ch(s, i + j) != s_ch(buscada, j)) {
                igual = false;
                break;
            }
        }
        if (igual) return i;
    }
    return -1;
}


// operações de alteração {{{1

void s_substitui(Str s, int pos, int tam, Str_c sb)
{
    s_ok(s);
    if(sb != NULL) s_ok(sb);
    
    int tamS = s_tam(s);
    
    int indiceInicio2;
    if (pos >= 0) {
        indiceInicio2 = pos;
    } else {
        indiceInicio2 = tamS + pos + 1;
    }

    int indiceFim;

    if(tam < 0) {
        indiceFim = tamS;
    } else {
        indiceFim = indiceInicio2 + tam;
    }

    int indiceInicio = indiceInicio2;

    if (indiceInicio < 0) {
        indiceInicio = 0;
    }

    if (indiceInicio > tamS){
        indiceInicio = tamS;
    }

    if (indiceFim < 0) {
        indiceFim = 0;
    }

    if (indiceFim > tamS) {
        indiceFim = tamS;
    }

    if (indiceFim < indiceInicio) {
        indiceFim = indiceInicio;
    }

    byte *inicio = u8_avanca_unichar(s->dados, indiceInicio);
    byte *fim = u8_avanca_unichar(s->dados, indiceFim);

    int byteInicio = inicio - s->dados;
    int byteFim = fim - s->dados;

    int bytesRemovidos = byteFim - byteInicio;

    int bytesInseridos = 0;
    byte *copiaInserida = NULL;

    if (sb != NULL){
        bytesInseridos = sb->numBytes;
        if(bytesInseridos > 0){
            copiaInserida = malloc(bytesInseridos);
            if(copiaInserida == NULL){
                printf("Erro ao criar copia!");
                exit(1);
            }

            memcpy(copiaInserida, sb->dados, bytesInseridos);
        }
    }

    int novoTotal = s->numBytes - bytesRemovidos + bytesInseridos;
    s_realoca(s, novoTotal);

    int bytesDepois = s->numBytes - byteFim;

    if (bytesDepois > 0) {
        memmove(s->dados + byteInicio + bytesInseridos, s->dados + byteFim, bytesDepois);
    }

    if(bytesInseridos > 0) {
        memcpy(s->dados + byteInicio, copiaInserida, bytesInseridos);
    }

    s->numBytes = novoTotal;
    free(copiaInserida);

    s_ok(s);
}

void s_substring(Str s, Str_c sb, int pos, int tam)
{
    s_ok(s);
    s_ok(sb);
    int tamSb = s_tam(sb);
    int indiceInicio2;

    if (pos >= 0) {
        indiceInicio2 = pos;
    } else {
        indiceInicio2 = tamSb + pos + 1;
    }

    int indiceFim;

    if (tam < 0) {
        indiceFim = tamSb;
    } else {
        indiceFim = indiceInicio2 + tam;
    }

    int indiceInicio = indiceInicio2;

    if (indiceInicio < 0) {
        indiceInicio = 0;
    }
    if (indiceInicio > tamSb) {
        indiceInicio = tamSb;
    }

    if (indiceFim < 0) {
        indiceFim = 0;
    }
    if (indiceFim > tamSb) {
        indiceFim = tamSb;
    }
    
    if (indiceFim < indiceInicio) {
        indiceFim = indiceInicio;
    }

    byte *pInicio = u8_avanca_unichar(sb->dados, indiceInicio);
    byte *pFim = u8_avanca_unichar(sb->dados, indiceFim);

    int deslocamentoInicio  = pInicio - sb->dados;
    int deslocamentoFim = pFim - sb->dados;
    int tamanho = deslocamentoFim - deslocamentoInicio;

    byte *t = NULL;

    if (tamanho > 0) {
        t = malloc(tamanho);
        assert(t != NULL);
        memcpy(t, sb->dados + deslocamentoInicio, tamanho);
    }

    s_realoca(s, tamanho);

    if (tamanho > 0) {
        memcpy(s->dados, t,tamanho);
    }
    s->numBytes = tamanho;
    free(t);
    
    s_ok(s);

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
    byte bytesUtf8[8];

    int quantidadeBytes = u8_converte_pra_utf8(c, bytesUtf8);

    if (quantidadeBytes == -1) return;

    struct str caractere = { 
        .dados = bytesUtf8,
        .numBytes = quantidadeBytes,
        .alocado = 8
    };

    s_substitui(s, pos, 0, &caractere);
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
    int tam = s_tam(s);

    int inicio = 0;
    while (inicio < tam && s_pertence(s_ch(s, inicio), sobras)) {
        inicio++;
    }

    int fim = tam;
    while (fim > inicio && s_pertence(s_ch(s, fim-1), sobras)) {
        fim--;
    }

    s_substring(s, s, inicio, fim - inicio);
}

// operações de E/S {{{1

void s_imprime(Str_c s)
{
    s_ok(s);
    fwrite(s->dados, 1, s->numBytes, stdout);
}

void s_grava_arquivo(Str_c s, char *nome)
{
    s_ok(s);
    FILE *arq = fopen(nome, "wb");
    if(arq == NULL) {
        return;
    }
    size_t gravado = fwrite(s->dados, 1, s->numBytes, arq);

    if(gravado != (size_t) s->numBytes){
        printf("\nErro na gravação da string no arquivo!\n");
        fclose(arq);
        return;
    }
    fclose(arq);
}


// vim: foldmethod=marker shiftwidth=2

