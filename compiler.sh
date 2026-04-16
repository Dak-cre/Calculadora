#!/bin/bash
echo "Compilando módulos..."
gcc -c operaciones.c -o operaciones.o
gcc -c pila.c -o pila.o
gcc -c main.c -o main.o
echo "Ligando archivos..."
gcc main.o operaciones.o pila.o -o calculadora
echo "¡Listo! Ejecutando..."
./calculadora
