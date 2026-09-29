#include "calc.h"
#include "dicionario.h"
#include <stdbool.h>
#include <string.h>
#include <stdlib.h>
#include <math.h>

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

static bool chaves_str_iguais(chave_t a, chave_t b)
{
    return s_igual((Str_c) a, (Str_c) b);
}

static double soma(double a, double b) 
{
    return a + b;
}

static bool chave_str_menor(chave_t a, chave_t b)
{
    Str_c string_a = a;
    Str_c string_b = b;
    char *char_a = s_strc(string_a);
    char *char_b = s_strc(string_b);
    int resultado = strcmp(char_a, char_b);
    if(resultado < 0) {
        return true;
    }
    return false;
}

static void garante_dicionario()
{
    if(dic_variaveis == NULL) {
        dic_variaveis = dic_cria(chave_str_menor, chaves_str_iguais);
    }
}

static double valor_operando(Str_c operando, bool *erro)
{
    *erro = false;
    unichar primeiro_caracter = s_ch(operando, 0);

    if (digito_ou_ponto(primeiro_caracter)) {
        return s_número(operando);
    }
    
    garante_dicionario();
    valor_t v = dic_busca(dic_variaveis, (chave_t) operando);
    if (v == VALOR_NÃO_EXISTE) {
        *erro = true;
        return 0.0;
    }

    Str_c valor_str = (Str_c) v;
    return s_número(valor_str);
}

static bool calcula_dois_operandos(Lista pilha_operandos, double (*operacao)(double, double))
{
    if(l_tam(pilha_operandos) < 2) return false;

    Str string_b = l_desempilha(pilha_operandos);
    Str string_a = l_desempilha(pilha_operandos);

    bool erro_a, erro_b;
    double valor_a = valor_operando(string_a, &erro_a);
    double valor_b = valor_operando(string_b, &erro_b);

    if (erro_a == true || erro_b == true) return false;

    double resultado = operacao(valor_a, valor_b);
    Str s_resultado = s_cria_número(resultado);
    l_empilha(pilha_operandos, s_resultado);
    
    return true;
}

static double soma(double a, double b)
{
    return a + b;
}

static double subtracao(double a, double b)
{
    return a - b;
}

static double multiplicacao(double a, double b)
{
    return a * b;
}

static double divisao(double a, double b)
{
    return a / b;
}

static double potenciacao(double a, double b)
{
    return pow(a, b);
}

static bool opera_soma(Lista p) 
{
    return calcula_dois_operandos(p, soma);
}

static bool opera_subtracao(Lista p) 
{
    return calcula_dois_operandos(p, subtracao);
}

static bool opera_multiplicacao(Lista p) 
{
    return calcula_dois_operandos(p, multiplicacao);
}

static bool opera_divisao(Lista p) 
{
    return calcula_dois_operandos(p, divisao);
}

static bool opera_potencia(Lista p) 
{
    return calcula_dois_operandos(p, potenciacao);
}

static Str valor_operando_str(Str_c operando, bool *erro)
{
    *erro = false;
    unichar primeiro_caractere = s_ch(operando, 0);

    if(digito_ou_ponto(primeiro_caractere)){
        return s_cria_cópia(operando);
    }

    garante_dicionario();
    valor_t v = dic_busca(dic_variaveis, (chave_t) operando);
    if (v == VALOR_NÃO_EXISTE) {
        *erro = true;
        return NULL;
    }
    
    return s_cria_cópia((Str_c) v);
}

static bool opera_atribuicao(Lista pilha_operandos)
{
    if(l_tam(pilha_operandos) < 2) return false;
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
