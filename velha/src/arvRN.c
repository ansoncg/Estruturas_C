/* Anderson Cardoso Gonçalves - github.com/ansoncg */

#include <stdlib.h>
#include <string.h>
#include "../include/arvRN.h"


typedef enum {VERMELHO, PRETO} Cor_No;

typedef struct arvRN_no {
   void *dado; 
   Cor_No cor;
   struct arvRN_no *esquerda, *direita, *pai;
} ArvRN_No;

struct arvRN {
   ArvRN_No *raiz;
   size_t dadoTam, arvTam; 
   bool repeticao;
   dado_copia funcCopia;
   dado_libera funcLibera;
   dado_compara funcCompara;
}; 

/* Funções auxiliares, internas. */

static void arvRN_iteraPre(ArvRN_No *arn_no, dado_usa funcao) {
   if (arn_no == NULL)
      return;
   (*funcao)(arn_no->dado);
   arvRN_iteraPre(arn_no->esquerda, funcao);
   arvRN_iteraPre(arn_no->direita, funcao);
   return;
} 

static void arvRN_iteraIn(ArvRN_No *arn_noAtual, dado_usa funcao) {
   if (arn_noAtual == NULL)
      return;
   arvRN_iteraPre(arn_noAtual->esquerda, funcao);
   (*funcao)(arn_noAtual->dado);
   arvRN_iteraPre(arn_noAtual->direita, funcao);
   return;
} 

static void arvRN_iteraPos(ArvRN_No *arn_no, dado_usa funcao) {
   if (arn_no == NULL)
      return;
   arvRN_iteraPre(arn_no->esquerda, funcao);
   arvRN_iteraPre(arn_no->direita, funcao);
   (*funcao)(arn_no->dado);
   return;
} 

static void arvRN_dadoCopia(ArvRN_No **arn_no, const void *dado, ArvRN *arn) {
   if (arn->funcCopia == NULL)
      memcpy((*arn_no)->dado, dado, arn->dadoTam);
   else
      arn->funcCopia((*arn_no)->dado, dado);
   return;
}

static int arvRN_dadoCmp(ArvRN *arn, void *dadoUm, void *dadoDois) {
   if (arn->funcCompara == NULL)
      return memcmp(dadoUm, dadoDois, arn->dadoTam);
   else
      return arn->funcCompara(dadoUm, dadoDois);
}

static void arvRN_criaNo(ArvRN_No **arn_no, const void *dado, ArvRN *arn) {
   (*arn_no) = malloc(sizeof(ArvRN_No));
   (*arn_no)->dado = malloc(arn->dadoTam);
   arvRN_dadoCopia(arn_no, dado, arn);   
   (*arn_no)->direita = (*arn_no)->esquerda = (*arn_no)->pai = NULL;
   return;
}


/* Funções externas, funcionalidades da Arvore rubro negra. */

Erro arvRN_cria(ArvRN **arn, size_t dadoTam, bool repeticao, dado_copia funcCopia, dado_libera funcLibera, dado_compara funcCompara) {
   (*arn) = malloc(sizeof(ArvRN_No));
   (*arn)->raiz = NULL;
   (*arn)->arvTam = 0;
   (*arn)->repeticao = repeticao;
   (*arn)->dadoTam = dadoTam;
   (*arn)->funcCopia = funcCopia;
   (*arn)->funcLibera = funcLibera;
   (*arn)->funcCompara = funcCompara;
   return 0;
}

Erro arvRN_itera(ArvRN *arn, dado_usa funcao, Ordem iteracaoOrdem) {
   switch (iteracaoOrdem) {
      case Pre:
         arvRN_iteraPre(arn->raiz, funcao);    
         break;
      case In:
         arvRN_iteraIn(arn->raiz, funcao);    
         break;
      case Pos:
         arvRN_iteraPos(arn->raiz, funcao);    
         break;
      case Largura:
         //TODO
         break;
   }
   return 0;
}

Erro arvRN_isere(ArvRN *arn, const void *dado) {
   ArvRN_No *noNovo = NULL;

   arvRN_criaNo(&noNovo, dado, arn);
   // Insere no
   arn->arvTam++;
   return 0;
}
