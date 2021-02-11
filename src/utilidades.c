/* Anderson Cardoso Gonçalves - github.com/ansoncg */

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "../include/utilidades.h"


/* Le uma string comum de tamanho indeterminado de 
 * uma stream. */
char *leString(FILE *entrada) {
    size_t strTam = 32, i = 0;
    char *string = malloc(strTam), c;

    while (((c = (char) getc(entrada)) != '\n') && c != EOF) {
        if (i == strTam) {
            strTam *= 2;
            string = realloc(string, strTam);
        }
        string[i++] = c;
    }
    string[i] = '\0';
    string = realloc(string, i + 1);
    return string;
}

/* Funções para serem usadas no iterador. */

void imprimeChar(void *dado) {
    printf("%c ", *(char *) dado);
    return;
}

void imprimeInteiro(void *dado) {
    printf("%d ", *(int *) dado);
    return;
}

void imprimeInteiroL(void *dado) {
    printf("%ld ", *(long *) dado);
    return;
}

void imprimeInteiroLL(void *dado) {
    printf("%lld ", *(long long *) dado);
    return;
}

void imprimeReal(void *dado) {
    printf("%lf ", *(float *) dado);
    return;
}

void imprimeRealD(void *dado) {
    printf("%lf ", *(double *) dado);
    return;
}

void imprimeString(void *dado) {
    printf("%s ", (char *) dado);
    return;
}

void imprimeStringV(void *dado) {
    printf("%s\n", (char *) dado);
    return;
}

//--------------------------------------------------------------------------------------------

/* Comparadores. 

   Em vez de memcmp direto pode implementar uma função
   propria para tipos simples do tipo:

   return ((*(char *) dadoUm) - ((*(char *) dadoDois)));
   */

int comparaString(void *dadoUm, void *dadoDois) {
    return strcmp((char *) dadoUm, (char *) dadoDois);
}

//--------------------------------------------------------------------------------------------

/* Joga para o stderr os erros dados pelas estruturas. */
void erro_checa(Erro saidaFunc) {
    switch (saidaFunc) {
        case SEM_ERRO:
            break;
        case REPETIDO:
            fprintf(stderr, "Dado repetido em estrutura sem repeticao\n");
            break;
        case ORDENADA:
            fprintf(stderr, "Estrutura eh ordenada, funcao incompativel\n");
            break;
        case NAO_ORDENADA:
            fprintf(stderr, "Estrutura nao eh ordenada, funcao incompativel\n");
            break;
        case GRAFO_NAO_VALORADO:
            fprintf(stderr, "Grafo nao valorado, funcao incompativel\n");
            break;
        case ESTR_VAZIA:
            fprintf(stderr, "Estrutura vazia, nao houve operacao\n");
            break;
        case NAO_ENCONTRADO:
            fprintf(stderr, "Dado nao encontrado na estrutura\n");
            break;
        case POS_INVALIDA:
            fprintf(stderr, "Posicao fora das possiveis\n");
            break;
        case GAVETA_INV:
            fprintf(stderr, "Numero da gaveta invalido\n");
            break;
        case NAO_INI:
            fprintf(stderr, "Estrutura nao inicializada\n");
            break;
    }
}
