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
    for


}