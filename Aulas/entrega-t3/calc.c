#include "calc.h"
#include "dicionario.h"
#include <stdbool.h>

typedef enum {
    TOKEN_OPERADOR,
    TOKEN_OPERANDO,
    ERRO
} tipo_token;

// f auxiliares

static Dicionário dic_variaveis = NULL;

static bool operador(unichar c)
{
    if (c == '+' || c == '-' || c == '*' || c == '/' ||
        c == '^' || c == '(' || c == ')' || c == '=') {
        return true;  
    }
    return false;
}

static bool digito(unichar c)
{
    if(c >= '0' && c <= '9') {
        return true;
    }
    return false;
}

static bool digito_ou_ponto(unichar c)
{
    if (c == '.' || digito(c)){
        return true;
    }
    return false;
}

static bool letra(unichar c)
{
    if ((c >= 'a' && c <= 'z') || (c >= 'A' && c <= 'Z')){
        return true;
    }
    return false;
}

static bool letra_ou_cifrao(unichar c)
{
    if(c == '$' || letra(c)){
        return true;
    }
    return false;
}

static bool letra_ou_digito_underline(unichar c)
{
    if(c == '_' || letra(c) || digito(c)){
        return true;
    }
    return false;
}

static bool espaco_tab_fim_de_linha(unichar c)
{
    if (c == ' ' || c == '\n' || c == '\t' || c == '\r'){
        return true;
    }
    return false;
}

static tipo_token classifica_token(Str_c token)
{
    int tam = s_tam(token);

    unichar primeiro_caracter = s_ch(token, 0);

    if(tam == 1 && operador(primeiro_caracter)) {
        return TOKEN_OPERADOR;
    }
    if (digito_ou_ponto(primeiro_caracter) || letra_ou_cifrao(primeiro_caracter)) {
        return TOKEN_OPERANDO;
    }

    return ERRO;
}

bool chaves_são_iguais(chave_t a, chave_t b)
{
  int *pa = a;
  int *pb = b;
  return *pa == *pb;
}

bool chave_é_menor(chave_t a, chave_t b)
{
  int *pa = a;
  int *pb = b;
  return *pa < *pb;
}

static void garante_dicionario()
{
    if(dic_variaveis == NULL) {
        dic_variaveis = dic_cria(chave_é_menor, chaves_são_iguais);
    }
}

static double valor_operando(Str_c operando)
{
    unichar primeiro_caracter = s_ch(operando, 0);
    if (digito_ou_ponto(primeiro_caracter)) {
        return s_número(operando);
    }
    
    valor_t v = dic_busca(dic_variaveis, (chave_t) operando);
    if (v == VALOR_NÃO_EXISTE) {
        return 0.0;
    }

    Str_c valor_str = (Str_c) v;
    return s_número(valor_str);
}

// -----------------

Str calculadora(Str expressão)
{
    return NULL;
}

Lista tokeniza(Str txt)
{
    Lista l = l_cria();
    int tam = s_tam(txt);
    int pos = 0;

    while (pos < tam) {
        unichar c = s_ch(txt, pos);
        if(espaco_tab_fim_de_linha(c)) {
            pos++;
            continue;
        }
        int inicio = pos;
        if(operador(c)){
            pos++;
        } else if (digito_ou_ponto(c)) {
            pos++;
            while (pos < tam) {
                unichar c1 = s_ch(txt, pos);
                if (!digito_ou_ponto(c1)) break;
                pos++;
            }
        } else if (letra_ou_cifrao(c)) {
            pos++;
            while (pos < tam) {
                unichar c1 = s_ch(txt, pos);
                if (!letra_ou_digito_underline(c1)) break;
                pos++;
            }
        } else {
            pos++;
        }
        int tam_token = pos - inicio;
        Str token = s_cria_substring(txt, inicio, tam_token);
        l_insere_fim(l, token);
    }
    return l;
}
