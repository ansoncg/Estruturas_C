#ifndef GAVETA_H
#define GAVETA_H

#include "definicoes.h"

typedef enum {
    VAZIO, LISTA, FILA, PILHA, GRAFO, VETOR, MATRIZ, STRING, HEAP, AVL, RN, GAVETA
} Estrutura;

typedef struct gaveta Gaveta;

Erro gaveta_esvazia(Gaveta *gaveta);  
Erro gaveta_insere(Gaveta *gaveta, void *dado, Estrutura tipo); 
void *gaveta_pegaDado(Gaveta *gaveta); 
Estrutura gaveta_pegaTipo(Gaveta *gaveta); 

#endif
