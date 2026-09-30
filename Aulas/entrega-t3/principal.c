#include "calc.h"
#include "str.h"
#include "lista.h"

#include <stdio.h>

int main(){
    char *entrada = "entrada.txt";
    char *saida = "saida.txt";

    Str conteudo = s_cria_de_arquivo(entrada);

    if(s_tam(conteudo) == 0) {
        printf("Arquivo vazio ou erro ao ler: %s", entrada);
        s_destroi(conteudo);
        return 1;
    }

    Str quebra_linha = s_cria("\n");
    Lista linhas = l_cria_separando(conteudo, quebra_linha);
    
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

    s_grava_arquivo(texto_saida, saida);
    s_destroi(texto_saida);

    while (!l_vazia(resultados)) {
        Str r = l_remove_inicio(resultados);
        s_destroi(r);
    }

    l_destroi(resultados);

    return 0;
}