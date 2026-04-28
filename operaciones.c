#include <stdio.h>
#include "operaciones.h"

float suma(float x, float y)
{
    return x + y;
}

float resta(float x, float y)
{
    return x - y;
}

float multipli(float x, float y)
{
    return x * y;
}

float division(float x, float y)
{
    return x / y;
}

float potencia(float base, float n)
{

    if (n == 0)
        return 1;

    int i;
    float res = 1;
    int potencia = n;
    if( n<0 ) potencia *= -1;
    for (i = 0; i < potencia; i++)
    {
        res *= base;
    }


    return (n>0) ? res : (1.0/res);
}

float factorial(float n)
{

    float x = 1;
    int i;
    for (i = n; i > 0; i--)
    {

        x = x * i;
    }
    return x;
}

float modulo(float a, float b)
{

    float n = a / b;
    return a - (((int)n) * b);
}


float raiz(float a){

    float x = a;
    float tolerancia = 0.000001;
    float anterior;

    do{
        anterior = x;
        x = 0.5 * (x + a / x);
    }while( (  (anterior-x) > tolerancia ) || ( (x-anterior) > tolerancia ) );
    
    return x;
}

