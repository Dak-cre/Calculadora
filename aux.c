#include <stdio.h>
#include <stdlib.h>
#include "aux.h"



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
            if(ptr == NULL){
                return NULL;
            }
        }
    }
    ptr[len] = '\0';
    return ptr;
}

char *filtrarExpresion(char *e)
{
    int newSize = 0, cap = 1;
    char *sinEspacios = malloc(cap);
    int i = 0;
    char anterior = ' ';

    while (e[i] != '\0')
    {
        // Verificamos si hay espacio para el caracter Y el '\0'
        if (newSize + 1 >= cap)
        {
            cap *= 2;
            sinEspacios = realloc(sinEspacios, cap);
            if(sinEspacios == NULL){
                return NULL;
            }
        }
        if ((anterior >= '0' && anterior <= '9') && e[i] == ' ')
        {
            sinEspacios[newSize] = e[i];
            newSize++;
        }

        if (e[i] != ' ')
        {
            anterior = e[i];
            sinEspacios[newSize] = e[i];
            newSize++;
        }
        i++;
    }

    // Aseguramos espacio final para el nulo por si acaso
    if (newSize >= cap)
    {
        sinEspacios = realloc(sinEspacios, newSize + 1);
    }
    sinEspacios[newSize] = '\0';

    return sinEspacios;
}
