#include "pila.h"
#include <stdio.h>
#include <stdlib.h>

//***************************************************** */
void inicializar(Pila *p)
{
    p->size = 0;
    p->cabeza = NULL;
    return;
}

void push(Pila *p, Token t)
{
    Nodo *n = (Nodo *)malloc(sizeof(Nodo));
    n->link = p->cabeza;
    n->token = t;
    p->cabeza = n;

    p->size++;
    return;
}
void pop(Pila *p)
{
    Nodo *dele = p->cabeza;
    p->cabeza = dele->link;
    p->size--;
    free(dele);
}

int vacia(Pila *p)
{
    return p->size == 0;
}
int size(Pila *p)
{
    return p->size;
}
Pila invertirPila(Pila *p)
{
    Pila aux;
    inicializar(&aux);
    Nodo *actual = p->cabeza;
    while (actual != NULL)
    {
        push(&aux, actual->token);
        actual = actual->link;
    }
    return aux;
}

void showPila(Pila *p)
{
    Nodo *actual = p->cabeza;
    printf("Contenido de la pila: ");
    while (actual != NULL)
    {
        if (actual->token.tipo == NUMERO)
            printf("[Num: %.2f] ", actual->token.valor);
        else
            printf("[Op: %c] ", actual->token.operacion);
        actual = actual->link;
    }
    printf("\n");
}

int generarTokenOperacion(char c, Token *token)
{
    // retornar un entero (1=true, 0=false) para indicar si el operador ingresado es valido
    // permitiendo hacer un filtrado con operadores, o signo no validos
    Token t;
    t.operacion = c;
    t.valor = 0;

    if (c == '(')
    {
        t.tipo = PARENTESIS_IZQUIERDA;
        t.prioridad = BAJA;
    }
    else if (c == ')')
    {
        t.tipo = PARENTESIS_DERECHA;
        t.prioridad = BAJA;
    }
    else if (c == '+' || c == '-')
    {
        t.tipo = OPERADOR;
        t.prioridad = BAJA;
    }
    else if (c == '*' || c == '/')
    {
        t.tipo = OPERADOR;
        t.prioridad = MEDIA;
    }
    else if (c == '^')
    {
        t.tipo = OPERADOR;
        t.prioridad = ALTA;
    }
    else if (c == '/')
    {
        t.tipo = OPERADOR;
        t.prioridad = ALTA;
    }
    else
    {
        return 0;
    }
    *token = t;
    return 1;
}

int esUnario(Token *tokens, int size)
{
    if (size == 0) return 1;

    Token anterior = tokens[size - 1];

    if (anterior.tipo == OPERADOR ||
        anterior.tipo == PARENTESIS_IZQUIERDA)
        return 1;

    return 0;
}

int tokenizarExpresion(char *c, Token **t)
{
    int size = 0;
    int sizeArray = 2;
    int posicion = 0;
    Token *tokens = (Token *)malloc(sizeArray * sizeof(Token));
    // la siguiente variable permitira leer numeros negativos, en donde si encuentra un signo de menos
    // despues de un operador y seguido de un digito, valdra -1, y se multiplicara por el numero obteneido
    int signo = 1;
    Token nuevoToken;
    while (c[posicion] != '\0') {

    char caracter = c[posicion];
                if (size >= sizeArray)
        {
            sizeArray *= 2;
            tokens = realloc(tokens, sizeArray * sizeof(Token));
        }
    if (caracter == ' ') {
        posicion++;
        continue;
    }

    if (caracter == '-') {

    if (esUnario(tokens, size)) {

        posicion++; // saltar '-'

        float numero = 0;

        while (c[posicion] >= '0' && c[posicion] <= '9') {
            numero = numero * 10 + (c[posicion] - '0');
            posicion++;
        }

        Token t;
        t.tipo = NUMERO;
        t.valor = -numero;

        tokens[size++] = t;
        continue;
    }
    else {
        Token t;
        generarTokenOperacion('-', &t);
        tokens[size++] = t;
        posicion++;
        continue;
    }
}


    // número
    if (caracter >= '0' && caracter <= '9') {
        float numero = 0;

        while (c[posicion] >= '0' && c[posicion] <= '9') {
            numero = numero * 10 + (c[posicion] - '0');
            posicion++;
        }

        nuevoToken.tipo = NUMERO;
        nuevoToken.valor = numero;
        tokens[size++] = nuevoToken;
        continue;
    }

    // operador
    if (generarTokenOperacion(caracter, &nuevoToken)) {
        tokens[size++] = nuevoToken;
        posicion++;
        continue;
    }

    // error real
    free(tokens);
    return 0;
}

/*
    while (c[posicion] != '\0')
    {
        char caracter = c[posicion];

        if (caracter == ' ')
        {
            posicion++;
            continue;
        }



        if (posicion == 0 && caracter == '-' && (c[posicion + 1] >= '0' && c[posicion + 1] <= '9'))
        {
            signo = -1;
            posicion++;
            continue;
        }
        if ((posicion > 0 || tokens[size - 1].tipo == OPERADOR ||
             tokens[size - 1].tipo == PARENTESIS_IZQUIERDA || tokens[size - 1].tipo == PARENTESIS_DERECHA) &&
            (c[posicion + 1] >= '0' && c[posicion + 1] <= '9') && caracter == '-' )
        {

            signo = -1;
            posicion++;
            continue;
        }
        Token nuevoToken;
        if (caracter >= '0' && caracter <= '9')
        {
            float numero = 0;
            // CORRECCI�N: Usar c[posicion] directamente para no procesar el s�mbolo extra
            while (c[posicion] >= '0' && c[posicion] <= '9')
            {
                numero = (numero * 10) + (c[posicion] - '0');
                posicion++;
            }
            nuevoToken.tipo = NUMERO;
            if (signo == -1)
            {
                numero *= signo;
                signo = 1;
            }
            nuevoToken.valor = numero;
            tokens[size] = nuevoToken;
            size++;
        }
        else
        {
            if (generarTokenOperacion(caracter, &nuevoToken))
            {
                tokens[size] = nuevoToken;
                size++;
                posicion++;
                continue;
            }
            // se regresa un 0 indicnaod que se ingreso un caracter no valido y no se puede procesar
            size = 0;
            break;
        }

/*
else if (caracter == '+' || caracter == '-' || caracter == '(' || caracter == ')' || caracter == '*' || caracter == '^' || caracter == '/')
        {
            if (generarTokenOperacion(caracter, &nuevoToken))
            {
                tokens[size] = nuevoToken;
                size++;
                posicion++;
                continue;
            }
            // se regresa un 0 indicnaod que se ingreso un caracter no valido y no se puede procesar
            size = 0;
            break;
        }




    }
    */
    *t = tokens;
    return size;
}

int convertir_postfija(Pila *p, Token *tokens, int sizeTokens)
{
    int i;
    Pila aux;
    TipoToken anterior = -1;
    int parentesis_abiertos = 0;
    inicializar(&aux);

    for (i = 0; i < sizeTokens; i++)
    {
        // 1. N�MEROS: V�lidos al inicio o despu�s de Operador o '('
        if (tokens[i].tipo == NUMERO)
        {
            if (anterior == NUMERO)
                return 0; // Error: 12 12
            push(p, tokens[i]);
        }
        // 2. OPERADORES: V�lidos despu�s de N�mero o ')'
        else if (tokens[i].tipo == OPERADOR)
        {
            if (anterior == OPERADOR || anterior == PARENTESIS_IZQUIERDA || anterior == -1)
                return 0;

            while (!vacia(&aux) && aux.cabeza->token.tipo == OPERADOR)
            {
                if (aux.cabeza->token.prioridad >= tokens[i].prioridad)
                {
                    push(p, aux.cabeza->token);
                    pop(&aux);
                }
                else
                    break;
            }
            push(&aux, tokens[i]);
        }
        // 3. PARENTESIS IZQUIERDO
        else if (tokens[i].tipo == PARENTESIS_IZQUIERDA)
        {
            push(&aux, tokens[i]);
            parentesis_abiertos++;
        }
        // 4. PARENTESIS DERECHO
        else if (tokens[i].tipo == PARENTESIS_DERECHA)
        {
            if (parentesis_abiertos <= 0)
                return 0; // Error: ) sin (

            parentesis_abiertos--;
            // Vaciar hasta encontrar '('
            while (!vacia(&aux) && aux.cabeza->token.tipo != PARENTESIS_IZQUIERDA)
            {
                push(p, aux.cabeza->token);
                pop(&aux);
            }
            if (!vacia(&aux))
                pop(&aux); // Sacar el '(' de la pila aux
        }
        else
        {
            return 0; // Caracter extra�o
        }
        anterior = tokens[i].tipo;
    }

    // Al terminar, vaciar lo que quede en aux
    while (!vacia(&aux))
    {
        if (aux.cabeza->token.tipo == PARENTESIS_IZQUIERDA)
            return 0; // Par�ntesis mal cerrado
        push(p, aux.cabeza->token);
        pop(&aux);
    }

    return (parentesis_abiertos == 0) ? 1 : 0;
}
