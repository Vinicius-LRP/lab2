
// teste_str.c
// programa com testes do TAD str

#include "str.h"
#include <stdio.h>

int main()
{
    char *a = "Rábica";
    
    Str s = s_cria(a);
    printf("Deve escrever [%s] \n", a);
    s_imprime(s);
    printf("\nTamanho: %d", s_tam(s));

    Str s1 = s_cria_de_arquivo("teste.txt");
    printf("\nCria String de arquivo: \n");
    s_imprime(s1);
    printf("\nTamanho: %d", s_tam(s1));
    s_grava_arquivo(s1, "teste2.txt");
    printf("\nString gravada no arquivo!");
    char *string = s_strc(s1);
    printf("\nString C: %s", string);
    printf("\nIguais? %d ", s_igual(s1, s));

    Str s2 = s_cria("café");
    for (int i = 0; i < s_tam(s2) + 3; i++) {
        unichar c = s_ch(s2, i);
        if(c == UNI_INV){
            printf("\nPosição %d fora dos limites! (UNI_INV)", i);
        } else {
            printf("\nposição %d: código = %u (0x%X)", i, c, c);
        }
    }
    s_destroi(s2);

    Str s3 = s_cria ("abácaxi");
    s_substitui(s3, -9, 1, s_cria("123"));
    printf("\nDeve escrever [123abácaxi] ");
    s_imprime(s3);
    
    Str s4 = s_cria("abc");
    s_insere_c(s4, 1, 'X');
    printf("\nDeve imprimir [aXbc] ");
    s_imprime(s4);

    Str s5 = s_cria("abácaxi");
    Str vogais = s_cria("aeiouáéíóú");

    printf("\nTestando s_busca_c");
    printf("\n%d", s_busca_c(s5, 0, vogais));
    printf("\n%d", s_busca_c(s5, 1, vogais));
    printf("\n%d", s_busca_c(s5, 3, vogais));
    printf("\n%d", s_busca_c(s5, 7, vogais));
    printf("\n%d", s_busca_c(s5, -100, vogais));
    s_destroi(s5);
    s_destroi(vogais);

    Str s6 = s_cria("teste 1");
    Str espacos = s_cria(" .");
    printf("\nTestando s_apara\n");
    s_apara(s6, espacos);
    s_imprime(s6);
    printf("\n");
    Str s7 = s_cria("... teste 2. .");
    s_apara(s7, espacos);
    s_imprime(s7);

    s_destroi(s6);
    s_destroi(s7);
    s_destroi(espacos);

    Str s8 = s_cria("...abc...");
    Str pontos = s_cria(".");
    printf("\nTestando s_busca_nc");
    printf("\n%d", s_busca_nc(s8, 0, pontos));
    printf("\n%d", s_busca_nc(s8, 3, pontos));
    printf("\n%d", s_busca_nc(s8, 4, pontos));
    printf("\n%d", s_busca_nc(s8, -100, pontos));

    Str s9 = s_cria("......");
    printf("\n%d", s_busca_nc(s9, 0, pontos));

    s_destroi(s8);
    s_destroi(s9);
    s_destroi(pontos);

    Str s10 = s_cria("abácaxi");
    Str v = s_cria("aeiouáéíóú");
    printf("\nTestando s_busca_rc");
    printf("\n%d", s_busca_rc(s10, 7, v));
    printf("\n%d", s_busca_rc(s10, 4, v));
    printf("\n%d", s_busca_rc(s10, 1, v));
    printf("\n%d", s_busca_rc(s10, 0, v));
    printf("\n%d", s_busca_rc(s10, -1, v));
    s_destroi(s10);
    s_destroi(v);

    Str s11 = s_cria("abácaxi");
    Str c = s_cria("bcx");
    printf("\nTestando s_busca_rnc");
    printf("\n%d", s_busca_rnc(s11, 7, c));
    printf("\n%d", s_busca_rnc(s11, 3, c));
    printf("\n%d", s_busca_rnc(s11, 1, c));
    printf("\n%d", s_busca_rnc(s11, 0, c));

    s_destroi(s11);
    s_destroi(c); 

    Str s12 = s_cria("abacaxi legal");
    Str buscada = s_cria("aca");
    printf("\nTestando s_busca_s");
    printf("\n%d", s_busca_s(s12, 0, buscada));
    printf("\n%d", s_busca_s(s12, 3, buscada));

    Str vazia = s_cria("");
    printf("\n%d", s_busca_s(s12, 5, vazia));
    printf("\n%d", s_busca_s(s12, -1, vazia));

    s_destroi(s12);
    s_destroi(buscada);
    s_destroi(vazia);

    Str ss = s_cria_substring(s, 3, 2);
    printf("\nDeve escrever [ic] ");
    s_imprime(ss);
    s_substring(ss, s, 1, 2);
    printf("\nDeve escrever [áb] ");
    s_imprime(ss);
    if (s_tam(ss) != 2) printf("\ntamanho de 'áb' (%d) deveria ser 2\n", s_tam(ss));
    s_substitui(ss, -2, 1, NULL);
    s_substitui(s, -7, 3, ss);
    printf("\nDeve escrever [áica] ");
    s_imprime(s);
    
    s_destroi(ss);
    s_destroi(s1);
    s_destroi(s);
}
