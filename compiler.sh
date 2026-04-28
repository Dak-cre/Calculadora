#!/bin/bash
# con la instruccion de arriba se le indica al sistema que use ese archivo bash, en esa ruta 
# para interpretarlo y poder ejecutar los comandos a contniacion, para que los detecte
# se debe ejecutar la instruccion chmod +x ./compiler.sh, para que se ejecute el archivo
echo "Compilando módulos..."
gcc -c  aux.c -o aux.o
gcc -c operaciones.c -o operaciones.o
gcc -c pila.c -o pila.o
gcc -c main.c -o main.o
echo "Ligando archivos..."
gcc main.o operaciones.o pila.o aux.o -o calculadora
echo "¡Listo! Ejecutando..."
./calculadora
