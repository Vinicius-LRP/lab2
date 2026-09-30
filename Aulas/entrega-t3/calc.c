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

typedef enum {
    VAZIA,
    SOMA_SUBTRACAO,
    MULT_DIV,
    POTENCIACAO,
    ABRE_CON,
    FECHA_CON,
    ATRIBUI,
    FIM
} operadores;

typedef enum {
    TERMINA, 
    EMPILHA,
    OPERA,
    DESCARTA,
    ERRO
} acao;

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

static bool nome_de_variavel(Str_c s)
{
    if(s_tam(s) == 0) return false;
    return letra_ou_cifrao(s_ch(s, 0));
}

static bool opera_atribuicao(Lista pilha_operandos)
{
    if(l_tam(pilha_operandos) < 2) return false;

    Str primeiro = l_desempilha(pilha_operandos);
    Str segundo = l_desempilha(pilha_operandos);

    if(!nome_de_variavel(segundo)){
        return false;
    }

    bool erro;
    Str valor_str = valor_operando_str(primeiro, &erro);

    if (erro == true) {
        return false;
    }

    garante_dicionario();
    valor_t antigo = dic_insere(dic_variaveis, (chave_t) segundo, (valor_t) valor_str);

    if (antigo != VALOR_NÃO_EXISTE){
        s_destroi((Str) antigo);
    }

    l_empilha(pilha_operandos, s_cria_cópia(valor_str));

    return true;
}

static operadores grupo(unichar c)
{
    if(c == '+' || c == '-') return SOMA_SUBTRACAO;
    if(c == '*' || c == '/') return MULT_DIV;
    if(c == '^') return POTENCIACAO;
    if (c == '(') return ABRE_CON;
    if(c == ')') return FECHA_CON;
    if(c == '=') return ATRIBUI;
}

static const acao tabela[7][7] = {
    {TERMINA, EMPILHA, EMPILHA, EMPILHA, EMPILHA, ERRO,     EMPILHA},
    {OPERA,   OPERA,   EMPILHA, EMPILHA, EMPILHA, OPERA,    EMPILHA},
    {OPERA,   OPERA,   OPERA,   EMPILHA, EMPILHA, OPERA,    EMPILHA},
    {OPERA,   OPERA,   OPERA,   EMPILHA, EMPILHA, OPERA,    EMPILHA},
    {ERRO,    EMPILHA, EMPILHA, EMPILHA, EMPILHA, DESCARTA, EMPILHA, },
    {0},
    {OPERA,   EMPILHA, EMPILHA, EMPILHA, EMPILHA, OPERA,    EMPILHA,}
};

static acao decide_acao(Lista pilha_operadores, tipo_token tipo_entrada, unichar op_entrada)
{
    int linha;
    if (l_vazia(pilha_operadores)) {
        linha = VAZIA;
    } else {
        Str topo = l_topo(pilha_operadores);
        linha = grupo(s_ch(topo, 0));
    }

    int coluna;
    if (tipo_entrada == ERRO || (tipo_entrada != TOKEN_OPERADOR)) {
        coluna = FIM;
    } else {
        coluna = grupo(op_entrada);
    }
    return tabela[linha][coluna];
}

static void destroi_lista_de_str(Lista l)
{
    while(!l_vazia(l)) {
        Str s = l_remove_inicio(l);
        s_destroi(s);
    }
    l_destroi(l);
}

static bool exercuta_operador(unichar op, Lista pilha_operandos)
{
    if(op == '+') return opera_soma(pilha_operandos);
    if(op == '-') return opera_subtracao(pilha_operandos);
    if(op == '*') return opera_multiplicacao(pilha_operandos);
    if(op == '/') return opera_divisao(pilha_operandos);
    if(op == '^') return opera_potencia(pilha_operandos);
    if(op == '=') return opera_atribuicao(pilha_operandos);
    return false;
}

static Str cria_erro(char const *msg)
{
    Str s = s_cria("#ERRO ");
    Str m = s_cria(msg);
    s_anexa(s, m);
    s_destroi(m);
    return s;
}

// -----------------

Str calculadora(Str expressão)
{
    Lista tokens = tokeniza(expressão);
    Lista pilha_operandos = l_cria();
    Lista pilha_operadores = l_cria();

    while(true) {
        int linha;
        if (l_vazia(pilha_operadores)) {
            linha = VAZIA;
        } else {
            Str topo = l_topo(pilha_operadores);
            linha = grupo(s_ch(topo, 0));
        }
    
        bool fim_da_entrada = l_vazia(tokens);
        Str token_atual = NULL;
        tipo_token tipo = TOKEN_OPERADOR;

        if (!fim_da_entrada) {
            token_atual = l_primeiro(tokens);
            tipo = classifica_token(token_atual);

            if(tipo == ERRO) {
                destroi_lista_de_str(tokens);
                destroi_lista_de_str(pilha_operandos);
                destroi_lista_de_str(pilha_operadores);
                return cria_erro("Token Invalido");
            }
            if (tipo == TOKEN_OPERANDO) {
                Str t = l_remove_inicio(tokens);
                l_empilha(pilha_operadores, t);
                continue;
            }
        }
        
        int coluna;

        if (fim_da_entrada) {
            coluna = FIM;
        } else {
            coluna = grupo(s_ch(token_atual, 0));
        }

        acao a = tabela[linha][coluna];

        if(a == TERMINA) {
            break;
        } else if (a == ERRO) {
            destroi_lista_de_str(tokens);
            destroi_lista_de_str(pilha_operandos);
            destroi_lista_de_str(pilha_operadores);
            return cria_erro("sintaxe");
        } else if (a == EMPILHA) {
            Str t = l_remove_inicio(tokens);
            l_empilha(pilha_operadores, t);
        } else if (a == DESCARTA) {
            Str abre = l_desempilha(pilha_operadores);
            s_destroi(abre);
            Str fecha = l_remove_inicio(tokens);
            s_destroi(fecha);
        } else if (a == OPERA) {
            Str op = l_desempilha(pilha_operadores);
            unichar c = s_ch(op, 0);
            bool ok = exercuta_operador(c, pilha_operandos);
            s_destroi(op);

            if (!ok) {
                if(!fim_da_entrada) {
                    destroi_lista_de_str(tokens);
                destroi_lista_de_str(pilha_operandos);
                destroi_lista_de_str(pilha_operadores);
                return cria_erro("Operação invalida!");
                }
            }
        }
    }

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
