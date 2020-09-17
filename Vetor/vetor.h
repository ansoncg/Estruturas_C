/* Anderson Cardoso Gonçalves - github.com/ansoncg */

#ifndef VETOR_H
#define VETOR_H

#include <stddef.h>
#include <stdbool.h>
#include "../Comum/definicoes.h"

typedef struct vetor Vetor;

Erro vetor_insere(Vetor *vet, const void *dado, size_t pos);
Erro vetor_insereFim(Vetor *vet, void *dado); 
Erro vetor_cria(Vetor **vet, size_t dadoTam, dado_copia funcCopia, dado_libera funcLibera, dado_compara funcCompara); 
Erro vetor_destroi(Vetor **vet); 
Erro vetor_retorna(Vetor *vet, void **dadoRetorno, size_t pos, bool memoriaNova); 
Erro vetor_itera(Vetor *vet, dado_usa funcao); 
Erro vetor_removeFim(Vetor *vet); 
Erro vetor_copia(Vetor *vetDest, Vetor *vetOrig); 
Erro vetor_esvazia(Vetor *vet); 
Erro vetor_ordena(Vetor *vet); 
Erro vetor_comparaPos(Vetor *vet, size_t posUm, size_t posDois, int *resultado); 

Erro vetor_pegaTamanho(Vetor *vet, int *vetTam); 
Erro vetor_pegaDadoTam(Vetor *vet, int *dadoTam); 

#endif 
