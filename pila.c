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
    else if (c == '!')
    {
        t.tipo = OPERADOR;
        t.prioridad = ALTA;
    }
    else if (c == '%')
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
    // una funcion aparte para validar el signo de menos con numeros negativos,
    if (size == 0)
        return 1;

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
    // token temporal para poder guardar cada token en orden
    Token nuevoToken;
    while (c[posicion] != '\0')
    {

        char caracter = c[posicion];
        if (size >= sizeArray)
        {
            sizeArray *= 2;
            tokens = realloc(tokens, sizeArray * sizeof(Token));
        }
        if (caracter == ' ')
        {
            posicion++;
            // hace un salto, hasta la siguiente iteracion, evitando tener que hacer las validaciones
            // sigueintes, es util para evitar que posicion aunmenta accidentalmente durante el proceso de validacion
            continue;
        }

        if (caracter == '-')
        {

            if (esUnario(tokens, size))
            {

                posicion++; // saltar '-'
                // esto validara que efectivamente despues de un signo de menos, que este caso no indica una
                // operacion sino un valor negativo le siga un numero evitando problemas como
                // -*, donde no le sigue un numero, pero si fue procesado como un signo negativo y no de operacion
                int hayNumero = 0;
                /*
                // si se trata un signo negativo de un numeros, formamos el numero,
                while (c[posicion] >= '0' && c[posicion] <= '9')
                {

                    hayNumero = 1;

                    numero = numero * 10 + (c[posicion] - '0');
                    posicion++;
                }
                    */
                float numero = 0;
                int condicion = 1;
                int decimales = 0;
                int dividor = 10;

                while (condicion && c[posicion] >= '0' && c[posicion] <= '9')
                {
                    hayNumero = 1;
                    if (decimales)
                    {
                        numero = numero + (((float)(c[posicion] - '0')) / dividor);
                        dividor *= 10;
                    }
                    else
                    {
                        numero = numero * 10 + (c[posicion] - '0');
                    }

                    posicion++;
                    if (c[posicion] == '.' && decimales == 0)
                    {
                        decimales++;
                        posicion++;
                    }
                    else if (c[posicion] == '.' && decimales >= 1)
                    {
                        condicion = 0;
                        return 0;
                    }
                }

                // si no le sigue un numero, entonces la sintaxis no estara correcta
                if(!hayNumero) return 0;

                Token t;
                t.tipo = NUMERO;
                t.valor = (-1) * numero;
                tokens[size++] = t;
                continue;
            }
            else
            {
                Token t;
                generarTokenOperacion('-', &t);
                tokens[size++] = t;
                posicion++;
                continue;
            }
        }

        // número
        if (caracter >= '0' && caracter <= '9')
        {
            float numero = 0;
            int condicion = 1;
            int decimales = 0;
            int dividor = 10;

            while (condicion && c[posicion] >= '0' && c[posicion] <= '9')
            {
                if (decimales)
                {
                    numero = numero + (((float)(c[posicion] - '0')) / dividor);
                    dividor *= 10;
                }
                else
                {
                    numero = numero * 10 + (c[posicion] - '0');
                }

                posicion++;
                if (c[posicion] == '.' && decimales == 0)
                {
                    decimales++;
                    posicion++;
                }
                else if (c[posicion] == '.' && decimales >= 1)
                {
                    condicion = 0;
                    return 0;
                }
            }

            nuevoToken.tipo = NUMERO;
            nuevoToken.valor = numero;
            tokens[size++] = nuevoToken;
            continue;
        }

        // operador
        if (generarTokenOperacion(caracter, &nuevoToken))
        {
            tokens[size++] = nuevoToken;
            posicion++;
            continue;
        }

        // en caso de no entrar en ninguno de los casos anteriores, el signo ingresado no es valido
        // por lo que no se pude procesar y la expresion estara mal
        free(tokens);
        return 0;
    }
    *t = tokens;
    return size;
}

int convertir_postfija(Pila *p, Token *tokens, int sizeTokens)
{
    int i;
    // aux guardara los operadores
    Pila aux;
    TipoToken anterior = -1; // se inicializa como -1, indicando que no hay aun un valor anterior
                             // lleva el control de los parentesis abiertos, es util en caso de que se cierre un parentesis pero no este
                             // abierto un parentesis anterior, evitando vaciar la pila de aux, y no haya un parentesis de apertura '('
    int parentesis_abiertos = 0;
    inicializar(&aux);

    for (i = 0; i < sizeTokens; i++)
    {

        if (tokens[i].tipo == NUMERO)
        {
            if (anterior == NUMERO || anterior == FACTORIAL)
                return 0;
            push(p, tokens[i]);
            anterior = NUMERO;
        }
        else if (tokens[i].operacion == '!' && anterior != OPERADOR && anterior != FACTORIAL)
        {
            push(p, tokens[i]);
            // Como la operacion modulo ocupa de solo un valor y puedo tener expresiones como
            // 3! * 4; el ! como es un operador puede marcar error al leer el *, por lo cual solo
            // ingreso el valor de !, en la pila de salida, y pongo anterior =NUmero para evitar errores
        }
        else if (tokens[i].tipo == OPERADOR && tokens[i].operacion != '!')
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
        if (tokens[i].operacion == '!')
        {
            anterior = FACTORIAL;
        }
        else
        {
            anterior = tokens[i].tipo;
        }
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

void cleanPila(Pila *p)
{
    while (!vacia(p))
    {
        pop(p);
    }
}
