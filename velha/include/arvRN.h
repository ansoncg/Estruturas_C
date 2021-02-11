#ifndef ARVRN_H
#define ARVRN_H

#include <stddef.h>
#include <stdbool.h>
#include "definicoes.h"

typedef struct arvRN ArvRN;
typedef enum {Pre, In, Pos, Largura} Ordem;

Erro arvRN_cria(ArvRN **arn, size_t dadoTam, bool repeticao, dado_copia funcCopia, dado_libera funcLibera, dado_compara funcCompara);
Erro arvRN_itera(ArvRN *arn, dado_usa funcao, Ordem iteracaoOrdem); 

#endif
