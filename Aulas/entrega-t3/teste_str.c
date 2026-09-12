
// teste_str.c
// programa com testes do TAD str

#include "str.h"
#include <stdio.h>
#include <stdlib.h>

int main() 
{
    Str s = s_cria_número(32);
    s_imprime(s);
    
    double num = s_número(s);

    printf("%f", num);

    s_destroi(s);
    

}