/* Anderson Cardoso Gonçalves - github.com/ansoncg */

#ifndef LISTA_H
#define LISTA_H

#include <stddef.h>
#include <stdbool.h>
#include "../Comum/definicoes.h"

typedef struct lista Lista;

/* Funcionalidades da lista. */

/* Cria e destroi. */
Erro lista_cria(Lista **l, size_t dadoTam, bool ordenada, bool repeticao, dado_copia funcCopia, dado_libera funcLibera, dado_compara funcCompara); 
Erro lista_destroi(Lista **l); 

/* Inserções. */
Erro lista_insereIni(Lista *l, void *dado); 
Erro lista_insereFim(Lista *l, void *dado); 
Erro lista_insereOrd(Lista *l, void *dado); 

/* Remoções. */
Erro lista_removeIni(Lista *l);
Erro lista_removeFim(Lista *l); 
Erro lista_removeDado(Lista *l, void *dado, bool todos); 

/* Passando pela lista. */
Erro lista_itera(Lista *l, dado_usa funcao); 
Erro lista_iteraInvert(Lista *l, dado_usa funcao); 
Erro lista_iteraAninhado(Lista *l, dado_usa funFora, dado_aninhado funDentro); 

/* Interagindo com o dado. */
Erro lista_contemDado(Lista *l, void *dado, int *pos); 
Erro lista_pegaDadoIni(Lista *l, void **dadoRetorno, bool memoriaNova);
Erro lista_pegaDadoFim(Lista *l, void **dadoRetorno, bool memoriaNova); 
Erro lista_pegaDadoPos(Lista *l, int pos, void **dadoRetorno, bool memoriaNova, char gavetaQual); 
Erro lista_pegaDado(Lista *l, void *dadoEntrada, void **dadoRetorno, char gavetaQual); 

/* Gavetas no sentinela. */
Erro lista_esvaziaGaveta(Lista *l, char gavetaQual);  
Erro lista_rmNoGaveta(Lista *l, char gavetaQual);
Erro lista_andaGaveta(Lista *l, char gavetaQual, bool direcao);
Erro lista_igualaGaveta(Lista *l, char gavetaOrg);
Erro lista_pegaGaveta(Lista *l, void **dadoRetorno, char gavetaQual);
Erro lista_pegaGavetaPos(Lista *l, int *gavetaPos, char gavetaQual); 

/* Getters da lista. */
int lista_pegaTam(Lista *l);
size_t lista_pegaDadoTam(Lista *l);
bool lista_pegaehOrdenada(Lista *l);
bool lista_pegaRepetida(Lista *l);

#endif
