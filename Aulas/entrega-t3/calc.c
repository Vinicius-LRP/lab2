#include "calc.h"
#include "dicionario.h"

// f auxiliares

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

static bool espaco_tab_fim_de_linha(unichar c)
{
    if (c == ' ' || c == '\n' || c == '\t' || c == '\r'){
        return true;
    }
    return false;
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
        } else if (letra(c)) {
            while (pos < tam) {
                unichar c1 = s_ch(txt, pos);
                if (!letra(c1)) break;
                pos++;
            }
        } else {
            pos++;
        }
        int fim = pos - inicio;
        Str token = s_cria_substring(txt, inicio, fim);
        l_insere_fim(l, token);
    }
    return l;
}
