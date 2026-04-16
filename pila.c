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
Pila invertirPila(Pila *p) {
    Pila aux;
    inicializar(&aux);
    Nodo *actual = p->cabeza;
    while (actual != NULL) {
        push(&aux, actual->token);
        actual = actual->link;
    }
    return aux;
}

void showPila(Pila *p) {
    Nodo *actual = p->cabeza;
    printf("Contenido de la pila: ");
    while (actual != NULL) {
        if (actual->token.tipo == NUMERO) 
            printf("[Num: %.2f] ", actual->token.valor);
        else 
            printf("[Op: %c] ", actual->token.operacion);
        actual = actual->link;
    }
    printf("\n");
}


Token generarTokenOperacion(char c) {
    Token t;
    t.operacion = c;
    t.valor = 0;

    if (c == '(') {
        t.tipo = PARENTESIS_IZQUIERDA;
        t.prioridad = BAJA; // IMPORTANTE: Baja para que no expulse operadores
    } 
    else if (c == ')') {
        t.tipo = PARENTESIS_DERECHA;
        t.prioridad = BAJA;
    } 
    else if (c == '+' || c == '-') {
        t.tipo = OPERADOR;
        t.prioridad = BAJA;
    } 
    else if (c == '*' || c == '/') {
        t.tipo = OPERADOR;
        t.prioridad = MEDIA;
    } 
    else if (c == '^') {
        t.tipo = OPERADOR;
        t.prioridad = ALTA;
    }
    return t;
}

int tokenizarExpresion(char *c, Token **t) {
    int size = 0;
    int sizeArray = 2;
    int posicion = 0;
    Token *tokens = (Token *)malloc(sizeArray * sizeof(Token));

    while (c[posicion] != '\0') {
        char caracter = c[posicion];

        if (caracter == ' ') {
            posicion++;
            continue;
        }

        if (size >= sizeArray) {
            sizeArray *= 2;
            tokens = realloc(tokens, sizeArray * sizeof(Token));
        }

        Token nuevoToken;
        if (caracter >= '0' && caracter <= '9') {
            float numero = 0;
            // CORRECCI�N: Usar c[posicion] directamente para no procesar el s�mbolo extra
            while (c[posicion] >= '0' && c[posicion] <= '9') {
                numero = (numero * 10) + (c[posicion] - '0');
                posicion++;
            }
            nuevoToken.tipo = NUMERO;
            nuevoToken.valor = numero;
        } 
        else {
            nuevoToken = generarTokenOperacion(caracter);
            posicion++;
        }
        tokens[size] = nuevoToken;
        size++;
    }
    *t = tokens;
    return size;
}


int convertir_postfija(Pila *p, Token *tokens, int sizeTokens) {
    int i;
    Pila aux;
    TipoToken anterior = -1;
    int parentesis_abiertos = 0;
    inicializar(&aux);

    for (i = 0; i < sizeTokens; i++) {
        // 1. N�MEROS: V�lidos al inicio o despu�s de Operador o '('
        if (tokens[i].tipo == NUMERO) {
            if (anterior == NUMERO) return 0; // Error: 12 12
            push(p, tokens[i]);
        }
        // 2. OPERADORES: V�lidos despu�s de N�mero o ')'
        else if (tokens[i].tipo == OPERADOR) {
            if (anterior == OPERADOR || anterior == PARENTESIS_IZQUIERDA || anterior == -1) return 0;

            while (!vacia(&aux) && aux.cabeza->token.tipo == OPERADOR) {
                if (aux.cabeza->token.prioridad >= tokens[i].prioridad) {
                    push(p, aux.cabeza->token);
                    pop(&aux);
                } else break;
            }
            push(&aux, tokens[i]);
        }
        // 3. PARENTESIS IZQUIERDO
        else if (tokens[i].tipo == PARENTESIS_IZQUIERDA) {
            push(&aux, tokens[i]);
            parentesis_abiertos++;
        }
        // 4. PARENTESIS DERECHO
        else if (tokens[i].tipo == PARENTESIS_DERECHA) {
            if (parentesis_abiertos <= 0) return 0; // Error: ) sin (
            
            parentesis_abiertos--;
            // Vaciar hasta encontrar '('
            while (!vacia(&aux) && aux.cabeza->token.tipo != PARENTESIS_IZQUIERDA) {
                push(p, aux.cabeza->token);
                pop(&aux);
            }
            if (!vacia(&aux)) pop(&aux); // Sacar el '(' de la pila aux
        }
        else {
            return 0; // Caracter extra�o
        }
        anterior = tokens[i].tipo;
    }

    // Al terminar, vaciar lo que quede en aux
    while (!vacia(&aux)) {
        if (aux.cabeza->token.tipo == PARENTESIS_IZQUIERDA) return 0; // Par�ntesis mal cerrado
        push(p, aux.cabeza->token);
        pop(&aux);
    }

    return (parentesis_abiertos == 0) ? 1 : 0;
}


