#include "calc.h"
#include "str.h"
#include "lista.h"

#include <stdio.h>

int main(){
    char *e = "entrada.txt";
    char *s = "saida.txt";

    Str cont = s_cria_de_arquivo(e);

    if(s_tam(cont) == 0) {
        printf("Arquivo %s vazio ou não existe", e);
        s_destroi(cont);
        return 1;
    }

    Str quebra_linha = s_cria("\n");
    Lista linhas = l_cria_separando(cont, quebra_linha);
    
    Lista resultados = l_cria();

    int n_linhas = l_tam(linhas);
    for (int i = 0; i < n_linhas; i++) {
        Str linha = l_dado_pos(linhas, i);
        Str resultado = calculadora(linha);
        l_insere_fim(resultados, resultado);
    }

    while (!l_vazia(linhas)) {
        Str l = l_remove_inicio(linhas);
        s_destroi(l);
    }
    l_destroi(linhas);

    Str sep_saida = s_cria("\n");
    Str texto_saida = s_cria_unindo(resultados, sep_saida);
    s_destroi(sep_saida);

    s_grava_arquivo(texto_saida, s);
    s_destroi(texto_saida);

    while (!l_vazia(resultados)) {
        Str r = l_remove_inicio(resultados);
        s_destroi(r);
    }

    l_destroi(resultados);

    return 0;
}