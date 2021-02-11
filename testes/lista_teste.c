#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <assert.h>
#include "teste.h"
#include "teste_utilidades.h"
#include "../include/lista.h"
#include "../include/definicoes.h"
#include "../include/utilidades.h"

#define STR_TAM 30
#define LISTAS_QUANT 10

char assertStr[10000];
int dadoTipo; 

dadoCompara(string_compara) {
    return strcmp((char *) dadoUm, (char *) dadoDois);
}

void paraString(void *dado) {
    char tmp[1000];
    switch (dadoTipo) {
        case INT:
            sprintf(tmp, "%d ", *(int *) dado);
            break;
        case FLOAT:
            sprintf(tmp, "%.2f ", *(float *) dado);
            break;
        case STR:
            sprintf(tmp, "%s ", (char *) dado);
            break;
        case STRUCT:
            pessoa_imprimeS(*(Pessoa *) dado, tmp);
            break;
    }
    strcat(assertStr, tmp);
    return;
}

void resetaAssertStr() {
    assertStr[0] = '\0';
    return;
}

void criaListas(Lista *listas[LISTAS_QUANT]) {
    for (int i = 0; i < LISTAS_QUANT; i++)
        listas[i] = NULL;
    lista_cria(&listas[INT], sizeof(int), true, false, SIMPLES); // Ordenada, sem repetição, simples.
    lista_cria(&listas[FLOAT], sizeof(float), false, true, SIMPLES); // Não odenada, com repetição, simples.
    lista_cria(&listas[STR], STR_TAM, false, false, NULL, NULL, string_compara); // Não ordenada, sem repetição.
    lista_cria(&listas[STRUCT], sizeof(Pessoa), true, true, pessoa_copia, pessoa_libera, pessoa_compara); // Ordenada, com repetição.
    return;
}

void encheListas(Lista *listas[LISTAS_QUANT]) {
    char str[STR_TAM] = "a";
    int inteiro;
    float real;
    Pessoa p;

    for (int i = 0; i < 5; i++) {
        inteiro = 4 - i;
        real = (float) 10.5 - (float) i;
        lista_insereOrd(listas[INT], &inteiro);
        lista_insereFim(listas[STR], str);
        lista_insereIni(listas[FLOAT], &real); 
        strcat(str, "a");
    }

    inteiro = 2;
    lista_removeIni(listas[INT]);
    lista_removeFim(listas[INT]);
    lista_removeDado(listas[INT], &inteiro, false);
    lista_insereOrd(listas[INT], &inteiro);
    lista_insereOrd(listas[INT], &inteiro);
    lista_removeDado(listas[INT], &inteiro, true);

    pessoa_cria(&p, 20, "Lala", 4, 4, 5, 6, 59);
    lista_insereOrd(listas[STRUCT], &p);
    pessoa_destroi(&p);

    pessoa_cria(&p, 15, "Laca", 2, 4, 5);
    lista_insereOrd(listas[STRUCT], &p);
    pessoa_destroi(&p);

    pessoa_cria(&p, 10, "Beco", 1, 1);
    lista_insereOrd(listas[STRUCT], &p);
    pessoa_destroi(&p);

    pessoa_cria(&p, 40, "Quej", 1, 10);
    lista_insereOrd(listas[STRUCT], &p);
    lista_insereOrd(listas[STRUCT], &p);
    lista_removeDado(listas[STRUCT], &p, true);
    pessoa_destroi(&p);

    return;
}

void assertListas(Lista *listas[LISTAS_QUANT]) {
    Lista *vazia = NULL;
    size_t pos;
    int inteiro = 1, idade = 15;
    void *dado;

    /* Lista vazia. */
    lista_cria(&vazia, sizeof(int), false, true, SIMPLES);
    assert(ESTR_VAZIA == lista_removeIni(vazia));
    assert(ESTR_VAZIA == lista_removeFim(vazia));
    assert(ESTR_VAZIA == lista_removeDado(vazia, &inteiro, false));
    assert(ESTR_VAZIA == lista_contemDado(vazia, &inteiro, &pos));
    assert(ESTR_VAZIA == lista_pegaDadoPos(vazia, 2, &dado, false));
    assert(ESTR_VAZIA == lista_pegaDado(vazia, &inteiro, &dado));
    lista_destroi(&vazia);

    /* Lista de inteiro. */
    dadoTipo = INT;
    assert(REPETIDO == lista_insereOrd(listas[INT], &inteiro));
    assert(ORDENADA == lista_insereIni(listas[INT], &inteiro));
    assert(ORDENADA == lista_insereFim(listas[INT], &inteiro));
    lista_contemDado(listas[INT], &inteiro, &pos);
    assert(pos == 0);
    inteiro = 2;
    assert(NAO_ENCONTRADO == lista_contemDado(listas[INT], &inteiro, &pos));
    lista_itera(listas[INT], paraString);
    assert(0 == strcmp("1 3 ", assertStr));   
    resetaAssertStr();

    lista_iteraInvertido(listas[INT], paraString);
    assert(0 == strcmp("3 1 ", assertStr));   
    resetaAssertStr();

    /* Lista de real. */
    dadoTipo = FLOAT;
    lista_pegaDadoPos(listas[FLOAT], 2, &dado, false);
    assert(8.50 == *(float *) dado);
    lista_pegaDadoPos(listas[FLOAT], 3, &dado, false);
    assert(9.50 == *(float *) dado);
    lista_pegaDadoIni(listas[FLOAT], &dado, false);
    assert(6.50 == *(float *) dado);
    lista_pegaDadoFim(listas[FLOAT], &dado, false);
    assert(10.50 == *(float *) dado);
    lista_pegaDadoPos(listas[FLOAT], 2, &dado, true);
    assert(8.50 == *(float *) dado);
    free(dado);
    assert(POS_INVALIDA == lista_pegaDadoPos(listas[FLOAT], 20, &dado, true));
    lista_itera(listas[FLOAT], paraString);
    assert(0 == strcmp("6.50 7.50 8.50 9.50 10.50 ", assertStr)); 
    resetaAssertStr();

    /* Lista de string. */
    assert(REPETIDO == lista_insereFim(listas[STR], "a"));
    assert(REPETIDO == lista_insereIni(listas[STR], "a"));
    assert(NAO_ORDENADA == lista_insereOrd(listas[STR], "a"));
    dadoTipo = STR;
    lista_itera(listas[STR], paraString);
    assert(0 == strcmp("a aa aaa aaaa aaaaa ", assertStr));   
    resetaAssertStr();

    /* Lista de struct. */
    dadoTipo = STRUCT;
    lista_pegaDado(listas[STRUCT], &idade, &dado);
    pessoa_imprimeS(*(Pessoa *) dado, assertStr);
    assert(0 == strcmp("Nome:  Laca | Idade: 15 | NSortes:   4   5 |\n", assertStr));
    resetaAssertStr();
    idade = 14;
    assert(NAO_ENCONTRADO == lista_pegaDado(listas[STRUCT], &idade, &dado));
    lista_itera(listas[STRUCT], paraString);
    assert(0 == strcmp("Nome:  Beco | Idade: 10 | NSortes:   1 |\n"
                "Nome:  Laca | Idade: 15 | NSortes:   4   5 |\n"
                "Nome:  Lala | Idade: 20 | NSortes:   4   5   6  59 |\n"
                , assertStr));


    /* Getters. */
    assert(lista_pegaOrdenada(listas[INT]) == true);
    assert(lista_pegaRepetida(listas[INT]) == false);
    assert(lista_pegaTam(listas[INT]) == 2);
    assert(lista_pegaDadoTam(listas[INT]) == sizeof(int));
    return;
}

void destroiListas(Lista *listas[LISTAS_QUANT]) {
    for (int i = 0; i < LISTAS_QUANT; i++)
        if (listas[i] != NULL)
            lista_destroi(&listas[i]);
    return;
}

void lista_teste() {
    Lista *listas[LISTAS_QUANT];

    criaListas(listas);
    encheListas(listas);
    assertListas(listas);
    destroiListas(listas);
    printf("Lista - Passou\n");
    return;
}
