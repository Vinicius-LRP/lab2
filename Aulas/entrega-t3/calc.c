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
