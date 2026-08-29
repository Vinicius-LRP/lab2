
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
    s_substitui(s, -9, 1, s_cria("123"));
    printf("\nDeve escrever [123abácaxi]\n");
    s_imprime(s3);

    Str ss = s_cria_substring(s, 3, 2);
    printf("\nDeve escrever [ic] ");
    s_imprime(ss);
    s_substring(ss, s, 1, 2);
    printf("\nDeve escrever [áb] ");
    s_imprime(ss);
    if (s_tam(ss) != 2) printf("\ntamanho de 'áb' (%d) deveria ser 2\n", s_tam(ss));
    s_substitui(ss, -2, 1, s);
    s_substitui(s, -7, 3, ss);
    printf("Deve escrever [áica] \n");
    s_imprime(s);
    s_destroi(ss);
    s_destroi(s1);
    s_destroi(s);
}
