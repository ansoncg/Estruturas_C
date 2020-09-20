#ifndef MATRIZ_H
#define MATRIZ_H

#include <stddef.h>
#include <stdbool.h>
#include "../Comum/definicoes.h"


typedef struct matriz Matriz;

Erro matriz_cria(Matriz **mat, size_t dadoTam, dado_copia funcCopia, dado_libera funcLibera, dado_compara funcCompara, int dimensoes, ...);
Erro matriz_insere(Matriz *mat, void *dado, int dimensoes, ...);
Erro matriz_retorna(Matriz *mat, void **dadoRetorno, bool memoriaNova, int dimensoes, ...);
Erro matriz_destroi(Matriz **mat);
Erro matriz_inicializa(Matriz *mat, void *dadoInicializador); 

#endif
