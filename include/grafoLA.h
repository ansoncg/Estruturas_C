/* Anderson Cardoso Gonçalves - github.com/ansoncg */

#ifndef GRAFOLA_H
#define GRAFOLA_H

#include <stddef.h>
#include <stdbool.h>
#include "definicoes.h"

typedef struct grafoLA GrafoLA;

Erro grafoLA_cria(GrafoLA **gLA, int verticesQuant, bool direcionado, bool valorado, bool repeticao, size_t val_dadoTam, dado_copia funcCopia, dado_libera funcLibera, dado_compara funcCompara); 
Erro grafoLA_insereAresta(GrafoLA *gLA, int verticeUm, int verticeDois, void *valor); 
Erro grafoLA_removeAresta(GrafoLA *gLA, int verticeUm, int verticeDois);
Erro grafoLA_destroi(GrafoLA **gLA);
Erro grafoLA_iteraValor(GrafoLA *gLA, dado_usa funcao); 
Erro grafoLA_iteraVertice(GrafoLA *gLA, dado_usa funcao);
Erro grafoLA_imprime(GrafoLA *gLA); 
Erro grafoLA_imprimeVal(GrafoLA *gLA, dado_usa funcao); 

#endif
