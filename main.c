#include "pila.h"
#include "operaciones.h"
#include <stdlib.h>
#include <stdio.h>

int resolver(Pila *p, float *resultado)
{
	// La funcion resive una variable flotante para lograr guardar el calculo al igual que la pila
	// de expresiones ya tranformada en su forma postfija, este retorna un entero, el cual indicara que
	// las operaciones se pudieron hacer de manera exitosa, esto en casos como la division entre 0 que no se
	// puede hacer, ademas de manejar mejor los errores.
	Pila invertida;
	inicializar(&invertida);
	invertida = invertirPila(p);
	Pila numeros;
	// Pila operadores;
	inicializar(&numeros);

	int estado = 1;

	int i;

	while (!vacia(&invertida) && estado)
	{

		if (invertida.cabeza->token.tipo == NUMERO)
		{
			push(&numeros, invertida.cabeza->token);
			pop(&invertida);
		}
		else
		{

			if (numeros.size > 1)
			{

				float b = numeros.cabeza->token.valor;
				pop(&numeros);
				float a = numeros.cabeza->token.valor;
				pop(&numeros);
				float resultado = 0;

				switch (invertida.cabeza->token.operacion)
				{
				case '+':
					resultado = suma(a, b);
					break;
				case '-':
					resultado = resta(a, b);
					break;
				case '*':
					resultado = multipli(a, b);
					break;
				case '/':
					if (b != 0)
					{
						resultado = division(a, b);
					}
					else
					{
						estado = 0;
					}
					break;
				case '^':
					if (((int)b) == b)
					{
						resultado = potencia(a, b);
					}
					else
					{
						estado = 0;
					}

					break;
				}

				Token t;
				t.tipo = NUMERO;
				t.valor = resultado;
				pop(&invertida);
				push(&numeros, t);
			}
			else
			{
			}
		}
	}
	*resultado = numeros.cabeza->token.valor;
	return estado;
}
char *leerExpresion()
{
	// Funcion para leer una expresion dinamicamente, permitiendo adpatarse a la cadena
	// de la expresion que el usuario ingrese, evitando usar un peque�o espacio o despediciar espacio
	int cap = 1, len = 0;
	char *ptr = malloc(cap);
	// se lee como entero para obtener el codigo ascii del digito ingresado, permitiendo leer mejor
	// cuando el usuario de enter.
	int c;
	while ((c = getchar()) != '\n' && c != EOF)
	{
		ptr[len] = (char)c;
		len++;
		if (len + 1 >= cap)
		{
			// se multiplica por 2, evitando tener que ampliar el espacio cada vez que ingresa un nuevo
			// caracter el usuario, disminuyendo el numero de veces que se necesita ampliar el espacio
			cap *= 2;
			ptr = realloc(ptr, cap);
		}
	}
	ptr[len] = '\0';
	return ptr;
}

char * filtrarExpresion(char * e){
	// Esta funcion eliminara los espacios en blacos con el objetivo de permitir analizar mejor la expresion
	// evitando que falle en especial cuando hay numeros negativos

	int newSize = 0, cap = 1;
	char * sinEspacios = malloc(cap);
	int i = 0;
	while(e[i] != '\0') {
		if(  newSize + 1 >= cap ){
			cap *=2;
			sinEspacios = realloc(sinEspacios,cap);
		}
		if( e[i]!=' '  ){
			sinEspacios[newSize] = e[i];
			newSize++;
		}
		i++;
	}

	return sinEspacios;

}

int main()
{

	printf("    ______      __           __           __                 \n");
	printf("   / ____/___ _/ /______  __/ /___ _____/ /___  _________ _  \n");
	printf("  / /   / __ `/ / ___/ / / / / __ `/ __  / __ \\/ ___/ __ `/  \n");
	printf(" / /___/ /_/ / / /__/ /_/ / / /_/ / /_/ / /_/ / /  / /_/ /   \n");
	printf(" \\____/\\__,_/_/\\___/\\__,_/_/\\__,_/\\__,_/\\____/_/   \\__,_/    \n");

	printf("\n \n");
	int opc;
	do
	{

		printf("Selecciones una opcion \n");
		printf("1. Hacer calculo \n");
		printf("0. Salir\n");
		scanf("%d", &opc);
		while (getchar() != '\n');
		switch (opc)
		{

		case 1:
		{
			printf("Hola\n");
			printf("Ingrese solo numeros enteros, y signos validos ( (),+,-,*,^ ) \n\n");
			printf("Ingrese la operacion: \n");
			char *exp;
			exp = leerExpresion();
			printf("Sin filtrar %s \n",exp);
			exp = filtrarExpresion(exp);
			printf("Filtrado %s \n",exp);
			Token *t;
			int n = tokenizarExpresion(exp, &t);
			printf("%d \n",n);
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
				free(exp);
				break;
			}
			float resultado;

			if (resolver(&expresion, &resultado) == 0)
			{
				printf("SINTAX ERROR\n");
				free(exp);
				free(t);
				break;
			}

			printf(" = %f \n\n", resultado);
			free(exp);
			break;
		}

		case 0:
			printf("Saliendo....\n");
			break;

		default:
			printf("Opcion no valida \n");
			break;
		}

	} while (opc != 0);
	return 10;
}

/* Funciones para evluar la expresion y que permiten su analisis */
/*
int tokenizarExpresion(char *c, Token **t)
{
	// Esta funcion recibe la cadena de caracteres leidas y el puntero de de un arreglo donde se guardaran
	// estos elementos ya convertidos en tokens, para su conversion a postfija, ademas de retornar el numero
	// de elementos que hay permitiendo ademas un previo analisis para saber si la expresion es valida por ejemplo
	// 8 + 5 * (4+5) es valido pero (8/9*+9) no es valida ya que estan dos signos de operacion seguidos
	int size = 0;	   // variable que indicara la cantidad de tokens obtenidos
	int sizeArray = 1; // variable auxiliar para poder aumentar el tamaño del arreglo de tokens

	Token *tokens;
	tokens = (Token *)malloc(sizeof(tokens));
	int posicion = 0; // variable para recorrer el arreglo de caracteres
	char caracter = c[posicion];
	while(caracter != '\0')
	{
		caracter = *(c + posicion);
		if (size >= sizeArray)
		{
			sizeArray *= 2;
			tokens = realloc(tokens, sizeArray * sizeof(Token));
		}

		if (caracter != ' ')
		{
			Token nuevoToken;
			if (caracter >= '0' && caracter <= '9')
			{
				float numero = 0;
				while (caracter >= '0' && caracter <= '9')
				{
					caracter = *(c + posicion);
					numero = (numero * 10) + (caracter - '0');
					posicion++;

				}

				nuevoToken.tipo = NUMERO;
				nuevoToken.valor = numero;
			}
			else
			{
				nuevoToken = generarTokenOperacion(caracter);
				posicion++;
			}
			*(tokens + size) = nuevoToken;
			size++;
		}
		else
			posicion++;

	}

	*t = tokens;

	return size;
}

Token generarTokenOperacion(char c)
{
	Token t;
	if (c == '+' || c == '-')
	{
		t.tipo = OPERADOR;
		t.prioridad = BAJA;
	}
	else if (c == '*' || c == '/')
	{
		t.tipo = OPERADOR;
		t.prioridad = MEDIA;
	}
	else if (c == '(')
	{
		t.tipo = PARENTESIS_IZQUIERDA;
		t.prioridad = ALTA;
	}
	else if (c == ')')
	{
		t.tipo = PARENTESIS_DERECHA;
		t.prioridad = BAJA;
	}
	else if (c == '^')
	{
		t.tipo = OPERADOR;
		t.prioridad = ALTA;
	}
	t.operacion = c;

	return t;
}
*/

/*
int convertir_postfija(Pila *p, Token *tokens, int sizeTokens)
{
	// Esta funcion regresa un entero para indicar si se pudo convertir a una notacion postfija,
	// con el 1 indicando que se logro con exito por el contrario el 0 indicara un error por ejemplo
	// 23 -/ 56, esta expresion no es valida
	int i;
	Pila aux; // pila para guardar los operadores de manera temporal, y ordenarlos por prioridad
	// permite guardar el token anterior permitiendo detectar si hay signos segudios como 34 /* 5,
	//  o si hay numeros seguidos 25 85, tampoco es valido ya que no indica una operacion
	TipoToken anterior = -1;
	// lleva control de los parentesis que esten abiertos y no se hayan cerrado, esto en caso como
	// 123 + ( 45 * ( 34) , donde no se cerro un parentesis por lo cual no es valido
	int parentesis = 0;
	inicializar(&aux);

	for (i = 0; i < sizeTokens; i++)
	{

		if (tokens[i].tipo == NUMERO && anterior != NUMERO)
		{
			push(p, tokens[i]);
		}
		else if (tokens[i].tipo == OPERADOR && anterior != OPERADOR)
		{

			if (vacia(&aux))
			{
				push(&aux, tokens[i]);
			}
			else
			{
				while (!vacia(&aux))
				{

					if (aux.cabeza->token.prioridad >= tokens[i].prioridad && aux.cabeza->token.tipo != PARENTESIS_IZQUIERDA)
					{
						push(p, aux.cabeza->token);
						pop(&aux);
					}
					else
					{
						break;
					}
				}
				push(&aux, tokens[i]);
			}
		}
		else if (tokens[i].tipo == PARENTESIS_IZQUIERDA)
		{
			push(&aux, tokens[i]);
			parentesis++;
		}
		else if (tokens[i].tipo == PARENTESIS_DERECHA && parentesis > 0)
		{
			// Se compara tambien que haya parentesis de izquierda abiertos, ya que sacar todos los operadores
			// hazte su parentesis que abre y no esta abierto indica que la expresion esta mal y no se pude procesar
			parentesis--;

			while (1)
			{
				if (aux.cabeza->token.tipo == PARENTESIS_IZQUIERDA)
				{
					pop(&aux);
					break;
				}

				push(p, aux.cabeza->token);
				pop(&aux);
			}
		}
		else
		{
			// Si no entro a ninguno de esos casos, significa que tengo un token que no corresponde esto puede ser como un caracter distinto
			// o una letra los cuales no se puedne evaluar, y pues significa que la operacion no es valida, o tenemos casos en donde dos
			// operadores esta seguidos o tengo un parentesis de cierre pero no tengo uno de abertura.
			anterior = -1;
			break;
		}

		anterior = tokens[i].tipo;
	}

	if (aux.size > 0 && anterior != -1)
	{

		while (!vacia(&aux))
		{

			push(p, aux.cabeza->token);
			pop(&aux);
		}
	}

	return anterior != -1 ? 1 : 0;
}
*/
