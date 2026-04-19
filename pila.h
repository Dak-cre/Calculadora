#ifndef PILA_H
#define PILA_H


typedef enum
{
	NUMERO,
	OPERADOR,
	PARENTESIS_DERECHA,
	PARENTESIS_IZQUIERDA
} TipoToken;
// Servira para determinar la prioridad de una operacion entendiendo Baja como sumas y restas,
// MEDIA como multiplicacion y division y ALTA a potencias
typedef enum
{
	BAJA,
	MEDIA,
	ALTA
} Prioridad;

typedef struct
{
	TipoToken tipo;
	float valor;
	Prioridad prioridad;
	char operacion;
} Token;
typedef struct Nodo
{
	Token token;
	struct Nodo *link;
} Nodo;

typedef struct
{
	int size;
	Nodo *cabeza;
} Pila;



// Operaciones de  la pila
void inicializar(Pila *p);
void push(Pila *p, Token t);
void pop(Pila *p);
int vacia(Pila *p);
int size(Pila *p);
void showPila(Pila *p);
void cleanPila(Pila*p);

// Operaciones para evaluar las expresiones
int tokenizarExpresion(char *c, Token **t);
int convertir_postfija(Pila *p, Token *tokens, int size);
int generarTokenOperacion(char c, Token*);
Pila invertirPila(Pila *p);

#endif



