#include <stdio.h>
#include "operaciones.h"

float suma (float x,float y){
    return x + y;
}

float resta(float x, float y){
    return x-y;
}

float multipli(float x,float y){
    return x*y;
}

float division(float x, float y){
    return x / y;
}

float potencia(float base,float n){

    if(n==0) return 1;

    int i;
    float res = 1;
    for(i=0; i<n; i++){
        res *= base;
    }

    return res;

}




