#include "calc.h"
#include "dicionario.h"

Str calculadora(Str expressão)
{
    
}

Lista tokeniza(Str txt)
{

}

static bool operador(unichar c)
{
    if (c == '+' || c == '-' || c == '*' || c == '/' ||
        c == '^' || c == '(' || c == ')' || c == '=') {
        return true;  
    }
}

static bool digito(unichar c)
{
    if(c >= '0' && c <= '9') {
        return true;
    }
}


