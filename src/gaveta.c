#include <stddef.h>
#include "../include/gaveta.h"

// Gavetas guardam unidades de alguma estrutura que não são acessiveis diretamente.

struct gaveta {
    void *dado;
    Estrutura tipo; 
};


Erro gaveta_cria(Gaveta **gaveta, gaveta_criador funcCriador) {
    funcCriador(gaveta);
    return SEM_ERRO;
}

Erro gaveta_destroi(Gaveta *gaveta) {
    gaveta->dado = NULL;
    gaveta->tipo = VAZIO;
    return SEM_ERRO;
}

Erro gaveta_insere(Gaveta *gaveta, void *dado, Estrutura tipo) {
    gaveta->dado = dado;
    gaveta->tipo = tipo;
    return SEM_ERRO;
}

// Getters
void *gaveta_pegaDado(Gaveta *gaveta) {
    return gaveta->dado;
}

Estrutura gaveta_pegaTipo(Gaveta *gaveta) {
    return gaveta->tipo;
}
