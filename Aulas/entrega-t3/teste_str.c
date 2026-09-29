
// teste_str.c
// programa com testes do TAD str

#include "calc.h"
#include "str.h"
#include "lista.h"
#include <stdio.h>
#include <stdlib.h>

int main() 
{
    Str s = s_cria_número(32);
    s_imprime(s);
    
    double num = s_número(s);

    printf("\n%f\n", num);

    Str txt = s_cria(" 9. 5");
    Lista tokens = tokeniza(txt);
    l_imprime(tokens);
    printf("\n");

    s_destroi(s);

}