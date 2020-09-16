/* Anderson Cardoso Gonçalves - github.com/ansoncg */

#ifndef FILA_H
#define FILA_H

#include <stddef.h>
#include <stdbool.h>
#include "../Comum/definicoes.h"
#include "../Lista/lista.h"

typedef struct fila Fila;

Erro fila_cria(Fila **f, size_t dadoTam, bool repeticao, dado_copia funcCopia, dado_libera funcLibera, dado_compara funcCompara); 
Erro fila_entra(Fila *f, void *dado);
Erro fila_sai(Fila *f);
Erro fila_pegaProx(Fila *f, void **dadoRetorno, bool memoriaNova); 
Erro fila_itera(Fila *f, dado_usa funcao); 
Erro fila_destroi(Fila **f);
Erro fila_pegaTam(Fila *f, int *filaTam);

#endif
