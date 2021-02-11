#include <stdio.h>
#include <assert.h>
#include <string.h>
#include "teste.h"
#include "teste_utilidades.h"
#include "../include/fila.h"

char fila_assertStr[100] = "";

void fila_paraString(void *dado) {
    char tmp[100];
    sprintf(tmp, "%d ", *(int *) dado);
    strcat(fila_assertStr, tmp);
    return;
}

void fila_teste() {
    Fila *f = NULL;
    void *retorno = NULL;

    fila_cria(&f, sizeof(int), true, NULL, NULL, NULL);
    for (int i = 0; i < 5; i++)
        fila_insere(f, &i);
    fila_remove(f); 
    fila_pegaProx(f, &retorno, false);
    assert(*(int *) retorno == 1);
    assert(fila_pegaTam(f) == 4);
    fila_itera(f, fila_paraString);
    assert(0 == strcmp("1 2 3 4 ", fila_assertStr));
    fila_destroi(&f);
    printf("Fila - Passou\n");
    return;
}

