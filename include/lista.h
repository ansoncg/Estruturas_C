/* Anderson Cardoso Gonçalves - github.com/ansoncg */

#ifndef LISTA_H
#define LISTA_H

#include <stddef.h>
#include <stdbool.h>
#include "gaveta.h"

typedef struct lista Lista;

/* Funcionalidades da lista. */

/* Cria e destroi. */
Erro lista_cria(Lista **l, size_t dadoTam, bool ordenada, bool repeticao, dado_copia funcCopia, dado_libera funcLibera, dado_compara funcCompara); 
Erro lista_destroi(Lista **l); 

/* Inserções. */
Erro lista_insereIni(Lista *l, const void *dado); 
Erro lista_insereFim(Lista *l, const void *dado); 
Erro lista_insereOrd(Lista *l, const void *dado); 

/* Remoções. */
Erro lista_removeIni(Lista *l);
Erro lista_removeFim(Lista *l); 
Erro lista_removeDado(Lista *l, const void *dado, bool todos); 

/* Passando pela lista. */
Erro lista_itera(Lista *l, dado_usa funcao); 
Erro lista_iteraInvertido(Lista *l, dado_usa funcao); 

/* Interagindo com o dado. */
Erro lista_contemDado(Lista *l, const void *dado, size_t *pos); 
Erro lista_pegaDadoPos(Lista *l, size_t pos, void **dadoRetorno, bool memoriaNova); 
Erro lista_pegaDadoIni(Lista *l, void **dadoRetorno, bool memoriaNova);
Erro lista_pegaDadoFim(Lista *l, void **dadoRetorno, bool memoriaNova); 
Erro lista_pegaDado(Lista *l, void *dadoEntrada, void **dadoRetorno); 

/* Getters da lista. */
size_t lista_pegaTam(Lista *l);
size_t lista_pegaDadoTam(Lista *l);
bool lista_pegaOrdenada(Lista *l);
bool lista_pegaRepetida(Lista *l);

#endif
