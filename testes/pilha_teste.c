#include <stdio.h>
#include <assert.h>
#include <string.h>
#include "teste.h"
#include "teste_utilidades.h"
#include "../include/pilha.h"

char pilha_assertStr[100] = "";

void pilha_paraString(void *dado) {
    char tmp[100];
    sprintf(tmp, "%d ", *(int *) dado);
    strcat(pilha_assertStr, tmp);
    return;
}

void pilha_teste() {
    Pilha *p = NULL;
    void *retorno = NULL;

    pilha_cria(&p, sizeof(int), true, NULL, NULL, NULL);
    for (int i = 0; i < 5; i++)
        pilha_insere(p, &i);
    pilha_remove(p); 
    pilha_pegaProx(p, &retorno, false);
    assert(*(int *) retorno == 3);
    assert(pilha_pegaTam(p) == 4);
    pilha_itera(p, pilha_paraString);
    assert(0 == strcmp("3 2 1 0 ", pilha_assertStr));
    pilha_destroi(&p);
    printf("Pilha - Passou\n");
    return;
}

