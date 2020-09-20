#include <stdio.h>
#include <stdlib.h>
#include <stdarg.h>
#include "matriz.h"
#include "../Vetor/vetor.h"


struct matriz {
   Vetor *vetor;
   int *tamanhos;
   int dimensoes;
   size_t tamanhoTotal;
};

Erro matriz_cria(Matriz **mat, size_t dadoTam, dado_copia funcCopia, dado_libera funcLibera, dado_compara funcCompara, int dimensoes, ...) {
   size_t tamanhoTotal = 1;
   va_list varLista;

   va_start(varLista, dimensoes);
   *mat = malloc(sizeof(Matriz));
   (*mat)->tamanhos = malloc(sizeof(int)*dimensoes);
   (*mat)->dimensoes = dimensoes;
   for (int i = 0; i < dimensoes; i++)  {
      tamanhoTotal *= (*mat)->tamanhos[i] = va_arg(varLista, int); 
   }
   (*mat)->tamanhoTotal = tamanhoTotal;
   vetor_cria(&((*mat)->vetor), dadoTam, funcCopia, funcLibera, funcCompara);  
   va_end(varLista);
   return 0;
}

Erro matriz_insere(Matriz *mat, void *dado, int dimensoes, ...) {
   size_t posVetor = 0;
   int multiplicador = 1;
   va_list varLista; 

   if (dimensoes != mat->dimensoes)
     return 11; 
   va_start(varLista, dimensoes);
   for (int i = 0; i < dimensoes - 1; i++) 
      multiplicador *= mat->tamanhos[i];
   for (int i = 0; i < dimensoes - 1; i++) {
      posVetor += multiplicador*va_arg(varLista, int);
      multiplicador /= mat->tamanhos[i];
   }
   posVetor += va_arg(varLista, int);
   vetor_insere(mat->vetor, dado, posVetor);  
   va_end(varLista);
   return 0;
}

Erro matriz_retorna(Matriz *mat, void **dadoRetorno, bool memoriaNova, int dimensoes, ...) {
   size_t posVetor = 0;
   int multiplicador = 1;
   va_list varLista; 

   if (dimensoes != mat->dimensoes)
     return 11; 
   va_start(varLista, dimensoes);
   for (int i = 0; i < dimensoes - 1; i++) 
      multiplicador *= mat->tamanhos[i];
   for (int i = 0; i < dimensoes - 1; i++) {
      posVetor += multiplicador*va_arg(varLista, int);
      multiplicador /= mat->tamanhos[i];
   }
   posVetor += va_arg(varLista, int); 
   vetor_retorna(mat->vetor, dadoRetorno, posVetor, memoriaNova);
   va_end(varLista);
   return 0;
}

Erro matriz_destroi(Matriz **mat) {
   vetor_destroi(&((*mat)->vetor));
   free((*mat)->tamanhos);
   free(*mat);
   return 0;
}

Erro matriz_inicializa(Matriz *mat, void *dadoInicializador) {
   vetor_inicializa(mat->vetor, dadoInicializador, mat->tamanhoTotal);
   return 0;
}
