/* Anderson Cardoso Gonçalves - github.com/ansoncg */

#ifndef PILHA_H
#define PILHA_H

#include <stddef.h>
#include <stdbool.h>
#include "definicoes.h"
#include "lista.h"

typedef struct pilha Pilha;

Erro pilha_cria(Pilha **p, size_t dadoTam, bool repeticao, dado_copia funcCopia, dado_libera funcLibera, dado_compara funcCompara);
Erro pilha_insere(Pilha *p, void *dado);
Erro pilha_remove(Pilha *p);
Erro pilha_pegaProx(Pilha *p, void **dadoRetorno, bool memoriaNova); 
Erro pilha_itera(Pilha *p, dado_usa funcao); 
Erro pilha_destroi(Pilha **p);
size_t pilha_pegaTam(Pilha *p); 

#endif
