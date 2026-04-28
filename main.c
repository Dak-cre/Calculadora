#include "pila.h"
#include "operaciones.h"
#include "aux.h"
#include <stdio.h>
#include <stdlib.h>

int resolver(Pila *p, float *resultado);
void obtenerOperandos(float *a, float *b, Pila *p);

int main()
{

	printf("    ______      __           __           __                 \n");
	printf("   / ____/___ _/ /______  __/ /___ _____/ /___  _________ _  \n");
	printf("  / /   / __ `/ / ___/ / / / / __ `/ __  / __ \\/ ___/ __ `/  \n");
	printf(" / /___/ /_/ / / /__/ /_/ / / /_/ / /_/ / /_/ / /  / /_/ /   \n");
	printf(" \\____/\\__,_/_/\\___/\\__,_/_/\\__,_/\\__,_/\\____/_/   \\__,_/    \n");

	printf("\n \n");
	// int opc = -1;
	char opc = ' ';
	char entrada[10];
	do
	{
		printf("Selecciones una opcion \n");
		printf("1. Hacer calculo \n");
		printf("0. Salir\n");
		scanf(" %c", &opc);
		char aux = getchar();
		while (aux != '\n')
		{
			aux = getchar();
			opc = ' ';
		}
		switch (opc)
		{

		case '1':
		{
			opc = ' ';
			printf("El sistema acepta numeros enteros positivos y negativos asi como numeros decimales \n\n");
			printf("Ingrese signos validos ( (),+,-,*,^, !, %%) \n\n");
			printf("Si tiene mas dudas consulte el archivo guia.txt\n\n");
			printf("Ingrese la operacion: \n");
			char *org;
			org = leerExpresion();
			if(org == NULL){
				printf("SINTAX ERROR \n");
			}

			char *exp;
			exp = filtrarExpresion(org);
			if(exp == NULL){
				printf("SINTAX ERROR \n");
			}
			free(org);

			Token *t;
			int n = tokenizarExpresion(exp, &t);

			if (n == 0)
			{
				printf("SINTAX ERROR \n");
				free(exp);
				break;
			}

			Pila expresion;
			inicializar(&expresion);
			int estado = convertir_postfija(&expresion, t, n);

			if (estado == 0)
			{
				printf("SINTAX ERROR\n");
				cleanPila(&expresion);
				free(t);
				free(exp);
				break;
			}
			float resultado;

			if (resolver(&expresion, &resultado) == 0)
			{
				printf("SINTAX ERROR\n");
				cleanPila(&expresion);
				free(exp);
				free(t);
				break;
			}

			printf(" = %f \n\n", resultado);
			free(exp);
			free(t);
			cleanPila(&expresion);
			break;
		}

		case '0':
			printf("Saliendo....\n");
			break;

		default:
			printf("Opcion no valida \n");
			break;
		}

	} while (opc != '0');
	return 0;
}

int resolver(Pila *p, float *r)
{
	// La funcion resive una variable flotante para lograr guardar el calculo al igual que la pila
	// de expresiones ya tranformada en su forma postfija, este retorna un entero, el cual indicara que
	// las operaciones se pudieron hacer de manera exitosa, esto en casos como la division entre 0 que no se
	// puede hacer, ademas de manejar mejor los errores.
	Pila invertida;
	inicializar(&invertida);
	invertida = invertirPila(p);
	cleanPila(p);
	Pila numeros;
	// Pila operadores;
	inicializar(&numeros);

	int estado = 1;
	float a = 0;
	float b = 0;
	float resultado = 0;
	// int i;

	while (!vacia(&invertida) && estado)
	{

		if (invertida.cabeza->token.tipo == NUMERO)
		{
			push(&numeros, invertida.cabeza->token);
			pop(&invertida);
		}
		else if (invertida.cabeza->token.operacion == '!' && size(&numeros) > 0)
		{
			float b = numeros.cabeza->token.valor;
			pop(&numeros);
			if (b > 0)
			{
				if ( ((int)b) == b)
				{
					resultado = factorial(b);
					Token t;
					t.tipo = NUMERO;
					t.valor = resultado;
					pop(&invertida);
					push(&numeros, t);
				}else{
					estado = 0;
					break;
				}
			}
			else
			{	
				estado = 0;
				break;
			}
		}
		else
		{

			if (numeros.size > 1)
			{

				switch (invertida.cabeza->token.operacion)
				{
				case '+':
					obtenerOperandos(&a, &b, &numeros);
					resultado = suma(a, b);
					break;
				case '-':
					obtenerOperandos(&a, &b, &numeros);
					resultado = resta(a, b);
					break;
				case '*':
					obtenerOperandos(&a, &b, &numeros);
					resultado = multipli(a, b);
					break;
				case '/':
					obtenerOperandos(&a, &b, &numeros);
					if (b != 0)
					{
						resultado = division(a, b);
					}
					else
					{
						estado = 0;
						printf("No se pude dividir entre 0\n");
					}
					break;
				case '^':
					obtenerOperandos(&a, &b, &numeros);
					if (((int)b) == b)
					{
						resultado = potencia(a, b);
					}
					else
					{
						printf("El programa no permite elevar un numero a un exponente decimal \n");
						estado = 0;
					}

					break;
				case '%':
					obtenerOperandos(&a, &b, &numeros);
					if ( ((int)a) != a && ((int)b) != b)
					{
						estado = 0;
					}
					else if (b <= 0 && a > 0)
					{
						estado = 0;
					}
					else
					{
						resultado = modulo(a, b);
					}

					break;
				}

				Token t;
				t.tipo = NUMERO;
				t.valor = resultado;
				pop(&invertida);
				push(&numeros, t);
			}
		}
	}

	if(estado) *r = numeros.cabeza->token.valor;
	cleanPila(&numeros);
	return estado;
}

void obtenerOperandos(float *a, float *b, Pila *p)
{

	*b = p->cabeza->token.valor;
	pop(p);
	*a = p->cabeza->token.valor;
	pop(p);
}