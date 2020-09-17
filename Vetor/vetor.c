#include <stdlib.h>
#include <string.h>
#include "vetor.h"

/* TODO: Botar erros, comparações, comentarios, arrumar caso alguem acesse uma posição alta, criar a opção de iterar até certo dado, Getters.*/

struct vetor {
   void **elementos;
   size_t tamanho, capacidade, dadoTam;
   dado_copia funcCopia;
   dado_libera funcLibera;
   dado_compara funcCompara;
};

int vetor_cabe(Vetor *vet, size_t pos) {
   return vet->capacidade > pos;
}

void vetor_aumentaCap(Vetor *vet) {
   size_t cap = vet->capacidade;
   vet->elementos = realloc(vet->elementos, 2*cap*sizeof(void *));
   if (vet->funcCopia != NULL)
      for (size_t i = cap; i < (2*cap); i++) 
         vet->elementos[i] = calloc(1, vet->dadoTam); // Com dado complexo já começa inicializado para testar com NULL
   else 
      for (size_t i = cap; i < (2*cap); i++) 
         vet->elementos[i] = malloc(vet->dadoTam);
   vet->capacidade *= 2;
   return;
}

/* Funcionalidades. */

Erro vetor_entra(Vetor *vet, const void *dado, size_t pos) {
   if (!vetor_cabe(vet, pos))
      vetor_aumentaCap(vet);
   if (vet->funcCopia == NULL)
      memcpy(vet->elementos[pos], dado, vet->dadoTam);
   else
      vet->funcCopia(vet->elementos[pos], dado);
   if (pos >= vet->tamanho)
      vet->tamanho = pos + 1;
   return 0;
} 

Erro vetor_retorna(Vetor *vet, void **dadoRetorno, size_t pos, bool memoriaNova) {
   if (memoriaNova) {
      (*dadoRetorno) = malloc(vet->dadoTam);
      if (vet->funcCopia == NULL)
         memcpy(*dadoRetorno, (vet->elementos[pos]), vet->dadoTam);
      else
         vet->funcCopia(*dadoRetorno, (vet->elementos[pos]));
   }
   else 
      *dadoRetorno = vet->elementos[pos];
   return 0;
}

Erro vetor_cria(Vetor **vet, size_t dadoTam, dado_copia funcCopia, dado_libera funcLibera, dado_compara funcCompara) {
   *vet = malloc(sizeof(Vetor)); 
   (*vet)->dadoTam = dadoTam;
   (*vet)->funcCopia = funcCopia;
   (*vet)->funcLibera = funcLibera;
   (*vet)->funcCompara = funcCompara;
   (*vet)->tamanho = 0;
   (*vet)->capacidade = 2;
   (*vet)->elementos = malloc(sizeof(void *)*2);
   (*vet)->elementos[0] = calloc(1, (*vet)->dadoTam); 
   (*vet)->elementos[1] = calloc(1, (*vet)->dadoTam);
   return 0;
}

Erro vetor_entraFim(Vetor *vet, void *dado) {
   vetor_entra(vet, dado, vet->tamanho); 
   return 0;
}

Erro vetor_saiFim(Vetor *vet) {
   vet->tamanho--;
   return 0;
}

Erro vetor_esvazia(Vetor *vet) {
   vet->tamanho = 0;
   return 0;
}

// Melhorar
Erro vetor_copia(Vetor *vetDest, Vetor *vetOrig) {
   for (size_t i = 0; i < vetOrig->tamanho; i++)
      vetor_entra(vetDest, vetOrig->elementos[i], i);
   return 0;
}

Erro vetor_destroi(Vetor **vet) {
   for (size_t i = 0; i < (*vet)->capacidade; i++) {
      if ((*vet)->funcLibera != NULL)
         (*vet)->funcLibera((*vet)->elementos[i]);
      free((*vet)->elementos[i]);
   }
   free((*vet)->elementos);
   free(*vet);
   *vet = NULL;
   return 0;
}

Erro vetor_itera(Vetor *vet, dado_usa funcao) {
   for (size_t i = 0; i < vet->tamanho; i++)
      funcao((vet->elementos[i]));
   return 0;
}

Erro vetor_comparaPos(Vetor *vet, size_t posUm, size_t posDois, int *resultado) {
   if (vet->funcCompara == NULL)
      *resultado = strcmp(vet->elementos[posUm], vet->elementos[posDois]);
   else
      *resultado = vet->funcCompara(vet->elementos[posUm], vet->elementos[posDois]);
   return 0;
}

/*
Erro vetor_ordena(Vetor *vet) {
   // Implementar quicksort  
   return 0;
}
*/

Erro vetor_pegaTamanho(Vetor *vet, int *vetTam) {
   *vetTam = vet->tamanho;
   return 0;
}

Erro vetor_pegaDadoTam(Vetor *vet, int *dadoTam) {
   *dadoTam = vet->dadoTam; 
   return 0;
}
