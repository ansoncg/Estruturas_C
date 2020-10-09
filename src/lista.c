/* Anderson Cardoso Gonçalves - github.com/ansoncg */

/* Lista com as seguintes caracteristicas:
 * - Dados genericos
 * - Apenas um tipo de dado para lista toda, a não ser que use outro ponteiro que varie dentro do dado
 * - Deep copy dos dados
 * - Duplamente encadeada
 * - Circular
 * - Com sentinela que aponta para nós sob controle do usuario ("gavetas")
 * - Opção de ser ordenada ou não
 * - Opção de aceitar repetições ou não
 
 * Sobre gavetas:
   São ponteiros do sentinela que apontam para nós especificos da lista para facilitar acesso.
   A numeração das gavetas para o uso em funções é: gavetaUm - 1, gavetaDois - 2, ambas - 3.
   Funções de remoção de nós no inicio, fim e ordenada esvaziam as gavetas. A função de remoção
   do nó dentro da gaveta coloca o próximo nó dentro da gaveta depois da remoção. Funções de inserção
   atualizam a nova posição numérica do nó dentro da gaveta se necessário.

 * Funções de copia, liberação e comparação são necessarias apenas para dados complexos, ou seja, uma
   struct com ponteiros dentro. 

 * Erros de retorno da lista:
 * 1 - Dado repetido em lista sem repetição.
 * 2 - Lista é ordenada, função incompativel.
 * 3 - Lista não é ordenada, função incompativel.
 * 5 - Lista vazia, não houve remoção.
 * 6 - Dado não encontrado na lista.
 * 7 - Lista vazia, não houve busca.
 * 8 - Posição fora das possiveis.
 * 9 - Número da gaveta inválido.
  
 * Obs: Cabem 2^(8*sizeof(int)-1) elementos na lista. */

#include <string.h>
#include <stdlib.h>
#include "../include/lista.h"

/* Estrutura do nó. */
typedef struct lista_no {
   void *dado;
   struct lista_no *prox;
   struct lista_no *ant;
} Lista_No;

/* Dado do sentinela com ponteiros para nós.
 * Nós da lista são guardados em 'gavetas' 
 * dentro do sentinela. */
typedef struct lista_sentDado {
   Lista_No *gavetaUm, *gavetaDois; 
   int posUm, posDois;
} Lista_sentDado;


/* Estrutura da lista. Caracteristicas
 * da lista aqui para facilitar acesso.
 * Funções de copia, liberação e comparação são 
 * necessarias apenas em dados que contém ponteiros. */
struct lista {
   Lista_No *sentinela; // Referencia da lista circular.
   size_t dadoTam; // Tamanho do dado dentro da lista.
   int listaTam; // Tamanho atual da lista.
   bool ordenada; // Se é uma lista ordenada.
   bool repeticao; // Se a lista aceita dados repetidos.
   dado_copia funcCopia; // Se não for implementada (NULL) será um memcpy() do dado.
   dado_libera funcLibera; // Se não for implementada (NULL) será apenas um free() do dado.
   dado_compara funcCompara; // Se não for implementada (NULL) será apenas um memcmp() dos dados.
};


/* ### Funções internas à lista. ### */


/* Aloca memória para 'noNovo' e insere 'dadoNovo' nele.
 * Parametros: Novo nó, novo dado, lista.
 * Erros: Nenhum.  */
static void lista_criaNo(Lista_No **noNovo, const void *dadoNovo, Lista *l) {
   (*noNovo) = malloc(sizeof(Lista_No));
   (*noNovo)->dado = malloc(l->dadoTam);
   if (l->funcCopia == NULL)
      memcpy((*noNovo)->dado, dadoNovo, l->dadoTam);
   else
      l->funcCopia((*noNovo)->dado, dadoNovo);
   (*noNovo)->prox = (*noNovo)->ant = NULL;
   return; 
}

/* Cria um sentinela com seu dado especifico.
 * Parametros: Sentinela da lista, tamanho do dado da lista.
 * Erros: Nenhum. */
static void lista_IniciaSentinela(Lista_No **sentinela) {
   (*sentinela) = malloc(sizeof(Lista_No));
   (*sentinela)->dado = malloc(sizeof(Lista_sentDado));
   ((Lista_sentDado *) ((*sentinela)->dado))->gavetaUm = *sentinela;
   ((Lista_sentDado *) ((*sentinela)->dado))->gavetaDois = *sentinela;
   ((Lista_sentDado *) ((*sentinela)->dado))->posUm = -1;
   ((Lista_sentDado *) ((*sentinela)->dado))->posDois = -1;
   (*sentinela)->prox = (*sentinela)->ant = *sentinela;
   return;
}

/* Guarda um nó qualquer nas gavetas. 
 * Parametros: Lista, opção de gaveta, nó que vai entrar.
 * Erros: 9. */
static Erro lista_entraGaveta(Lista *l, char gavetaQual, Lista_No *noEntra) {
   switch (gavetaQual) {
      case 1:
         ((Lista_sentDado *) (l->sentinela->dado))->gavetaUm = noEntra;
         break;
      case 2:
         ((Lista_sentDado *) (l->sentinela->dado))->gavetaDois = noEntra;
         break;
      case 3:
         ((Lista_sentDado *) (l->sentinela->dado))->gavetaUm = noEntra;
         ((Lista_sentDado *) (l->sentinela->dado))->gavetaDois = noEntra;
         break;
      default:
         return 9;
   } 
   return 0;
}

/* Atualiza a posição e ponteiro da gaveta na movimentação proximo ou anteior.
 * Parametros: Lista, ponteiro do nó dentro da gaveta, posição do no dentro da gaveta, direção que andou.
 * Erros: Nenhum. */
static void lista_atualizaGaveta(Lista *l, Lista_No **noGuardado, int *gavetaPos, bool direcao) {
   if (noGuardado == NULL)
      return;
   if (direcao) {
      (*noGuardado) = (*noGuardado)->prox; 
      if ((*gavetaPos) == (l->listaTam - 1))
         *gavetaPos = -1; 
      else
         (*gavetaPos)++;
   }
   else {
      (*noGuardado) = (*noGuardado)->ant; 
      if ((*gavetaPos) == -1)
         *gavetaPos = (l->listaTam - 1);
      else
         (*gavetaPos)--;
   }
   return;
}

/* Corrige o número da posição na gaveta ao ocorrer remoção/inserção.
 * Parametros: Lista, posição da rm/ins, se foi inserção ou remoção.
 * Erros: Nenhum. */
static void lista_arrumaPosGaveta(Lista *l, int pos, bool insercao) {
   int posUm = ((Lista_sentDado *) l->sentinela->dado)->posUm;
   int posDois = ((Lista_sentDado *) l->sentinela->dado)->posDois;
   int atualiza = insercao ? 1 : -1;

   if (posUm >= pos)
      ((Lista_sentDado *) l->sentinela->dado)->posUm += atualiza;
   if (posDois >= pos)
      ((Lista_sentDado *) l->sentinela->dado)->posDois += atualiza;
   return;
}

/* Diz se a fila está vazia.
 * Parametros: Lista.
 * Erros: Nenhum. */
static int lista_estaVazia(Lista *l) {
   return (!(l->listaTam));
}

/* Insere um novo nó contendo 'dado' na lista 'l'.
 * Parametros: Lista, nó anterior ao inserido, no posterior ao inserido, novo dado.
 * Erros: Nenhum. */
static void lista_insereNo(Lista *l, Lista_No *noAntes, Lista_No *noDepois, void *dado) {
   Lista_No *noNovo = NULL;

   lista_criaNo(&noNovo, dado, l);
   noNovo->prox = noAntes->prox;
   noNovo->ant = noDepois->ant;
   noAntes->prox = noNovo;
   noDepois->ant = noNovo;
   (l->listaTam)++; 
   return;
}

/* Remove um nó 'noDeleta' da lista 'l'.
 * Parametros: Lista, nó anterior ao removido, no a ser removido, no posterior ao removido.
 * Erros: Nenhum. */
static void lista_removeNo(Lista *l, Lista_No *noAntes, Lista_No *noDeleta, Lista_No *noDepois) {
   noAntes->prox = noDepois;
   noDepois->ant = noAntes;
   if (l->funcLibera != NULL)
      l->funcLibera(noDeleta->dado);
   free(noDeleta->dado);
   free(noDeleta);
   (l->listaTam)--; 
   return;
}

/* Testa se 'dado' já pertence a lista 'l'.
 * Parametros: Lista, dado generico.
 * Erros: 1. */
static Erro lista_veSeRepete(Lista *l, void *dado) {
   int conta = l->listaTam;
   Lista_No *percorre = l->sentinela->prox;
   
   while (conta--) {
      if (l->funcCompara == NULL) {
         if (!(memcmp(dado, percorre->dado, l->dadoTam)))
            return 1;
      }
      else 
         if (!(l->funcCompara(dado, percorre->dado)))
            return 1;
      percorre = percorre->prox;
   }
   return 0;
}

/* ### Funcionalidades da lista. ### */

/* Cria a lista, precisa ser chamada antes de tudo.
 * Parametros: Lista, tamanho do dado, se será ordenada, se aceitará repetição, funções de copia, liberação e comparação.
 * Erros: Nenhum. */
Erro lista_cria(Lista **l, size_t dadoTam, bool ordenada, bool repeticao, dado_copia funcCopia, dado_libera funcLibera, dado_compara funcCompara) {
   Lista_No *sentinela = NULL;

   (*l) = malloc(sizeof(Lista));
   (*l)->dadoTam = dadoTam;
   (*l)->listaTam = 0;
   (*l)->ordenada = ordenada;
   (*l)->repeticao = repeticao;
   (*l)->funcCopia = funcCopia;
   (*l)->funcLibera = funcLibera;
   (*l)->funcCompara = funcCompara;
   lista_IniciaSentinela(&sentinela);
   (*l)->sentinela = sentinela;
   return 0;
}

/* Insere no inicio da lista. 
 * Parametros: Lista, dado generico.
 * Erros: 1, 2. */
Erro lista_insereIni(Lista *l, void *dado) {
   int erro;

   if (l->ordenada)
      return 2;
   if (!(l->repeticao) && (erro = lista_veSeRepete(l, dado)))
      return erro;
   lista_insereNo(l, l->sentinela, l->sentinela->prox, dado);
   lista_arrumaPosGaveta(l, 0, true);
   return 0;
} 

/* Insere no fim da lista. 
 * Parametros: Lista, dado generico.
 * Erros: 1, 2.  */
Erro lista_insereFim(Lista *l, void *dado) {
   int erro;
  
   if (l->ordenada)
      return 2;
   if (!(l->repeticao) && (erro = lista_veSeRepete(l, dado)))
      return erro;
   lista_insereNo(l, l->sentinela->ant, l->sentinela, dado);
   return 0;
} 

/* Insere na lista ordenada.
 * Parametros: Lista, dado generico.
 * Erros: 1, 3. */ 
Erro lista_insereOrd(Lista *l, void *dado) {
   int conta = l->listaTam, comparacao;
   bool repeticao = l->repeticao;
   Lista_No *percorre = l->sentinela->prox;

   if (!(l->ordenada))
      return 3;
   while (conta--) {
      if (l->funcCompara == NULL)
         comparacao = memcmp(dado, percorre->dado, l->dadoTam);
      else
         comparacao = l->funcCompara(dado, percorre->dado);
      if (!repeticao && !comparacao)
         return 1;
      if (comparacao < 0)
         break;
      percorre = percorre->prox;
   }  
   lista_insereNo(l, percorre->ant, percorre, dado);
   if (conta != -1)
      lista_arrumaPosGaveta(l, l->listaTam - conta - 1, true);
   return 0;
}

/* Remove primeiro elemento da lista.
 * Parametros: Lista. 
 * Erros: 5. */
Erro lista_removeIni(Lista *l) {
   if (!(l->listaTam))  // Checa por lista vazia (só sentinela).
      return 5;
   lista_esvaziaGaveta(l, 3); // Esvazia ambas gavetas.
   lista_removeNo(l, l->sentinela, l->sentinela->prox, l->sentinela->prox->prox);
   return 0;
}

/* Remove ultimo elemento da lista.
 * Parametros: Lista. 
 * Erros: 5. */
Erro lista_removeFim(Lista *l) {
   if (!(l->listaTam))  // Checa por lista vazia (só sentinela).
      return 5;
   lista_esvaziaGaveta(l, 3); // Esvazia ambas gavetas.
   lista_removeNo(l, l->sentinela->ant->ant, l->sentinela->ant, l->sentinela);
   return 0;
}

/* Remove um ou todos os elementos da lista que contem um dado especifico.
 * Parametros: Lista, dado para remover, opção de ser todos.
 * Erros: 5 e 6. */
Erro lista_removeDado(Lista *l, void *dado, bool todos) {
   int conta = l->listaTam, removeu = 6, comparacao;
   Lista_No *percorre = l->sentinela->prox;
   Lista_No *deleta = NULL;

   if (!(l->listaTam)) 
      return 5;
   while (conta--) {
      if (l->funcCompara == NULL)
         comparacao = memcmp(dado, percorre->dado, l->dadoTam);
      else 
         comparacao = l->funcCompara(dado, percorre->dado);
      if (!(comparacao)) {
         deleta = percorre;
         percorre = percorre->prox;
         lista_esvaziaGaveta(l, 3); // Esvazia ambas gavetas.
         lista_removeNo(l, deleta->ant, deleta, deleta->prox);
         removeu = 0;
         if (!todos || !(l->repeticao))
            break;
      }
      else
         percorre = percorre->prox;
   }
   return removeu;
}

/* Busca se um dado está na lista e devolve sua posição em 'pos'.
 * Parametros: Lista, dado a ser buscado, variavel de retorno.
 * Erros: 6 e 7. */ 
Erro lista_contemDado(Lista *l, void *dado, int *pos) {
   int conta = l->listaTam, comparacao;
   bool ordenada = l->ordenada;
   Lista_No *percorre = l->sentinela->prox;
   
   *pos = -1;
   if (lista_estaVazia(l))
      return 7;
   while (conta--) {
      if (l->funcCompara == NULL)
         comparacao = memcmp(dado, percorre->dado, l->dadoTam);
      else
         comparacao = (l->funcCompara (dado, percorre->dado));
      if (!comparacao) {
         *pos = l->listaTam - conta - 1;
         return 0;
      }
      if (ordenada && (comparacao < 0))
         break;
      percorre = percorre->prox;
   }
   return 6;
}

/* Remove da lista o nó guardado na gaveta e guarda o proximo nela.
 * Parametros: Lista, opção de gaveta.
 * Erros: 9. */
Erro lista_rmNoGaveta(Lista *l, char gavetaQual) {
   Lista_No *noDeleta = NULL;
   int posUm = ((Lista_sentDado *) l->sentinela->dado)->posUm;
   int posDois = ((Lista_sentDado *) l->sentinela->dado)->posDois;
   int posRm;
   
   switch (gavetaQual) {
      case 1:
         noDeleta = ((Lista_sentDado *) (l->sentinela->dado))->gavetaUm;
         if (posUm == posDois) // Anda com a segunda gaveta também caso esteja na mesma posição
            lista_andaGaveta(l, 2, Proximo);
         lista_andaGaveta(l, 1, Proximo);
         posRm = posUm;
         break;
      case 2:
         noDeleta = ((Lista_sentDado *) (l->sentinela->dado))->gavetaDois;
         if (posUm == posDois) // Anda com a primeira gaveta também caso esteja na mesma posição
            lista_andaGaveta(l, 1, Proximo);
         lista_andaGaveta(l, 2, Proximo);
         posRm = posDois;
         break;
      default:
         return 9;
   }
   lista_removeNo(l, noDeleta->ant, noDeleta, noDeleta->prox);
   if (!lista_estaVazia(l)) 
      lista_arrumaPosGaveta(l, posRm, false); 
   return 0;
}

/* Faz gavetas apontarem para o sentinela.
 * Parametros: Lista, opção de gavetas.
 * Erros: 9. */
Erro lista_esvaziaGaveta(Lista *l, char gavetaQual) {// 1 - primeira 2 - segunda 3 - ambas 
   switch (gavetaQual) {
      case 1:
         ((Lista_sentDado *) l->sentinela->dado)->gavetaUm = l->sentinela;
         ((Lista_sentDado *) l->sentinela->dado)->posUm = -1;
         break;
      case 2:
         ((Lista_sentDado *) l->sentinela->dado)->gavetaDois = l->sentinela;
         ((Lista_sentDado *) l->sentinela->dado)->posDois = -1;
         break;
      case 3:
         ((Lista_sentDado *) l->sentinela->dado)->gavetaUm = l->sentinela;
         ((Lista_sentDado *) l->sentinela->dado)->posUm = -1;
         ((Lista_sentDado *) l->sentinela->dado)->gavetaDois = l->sentinela;
         ((Lista_sentDado *) l->sentinela->dado)->posDois = -1;
         break;
      default:
         return 9;
   }
   return 0;
}

/* Guarda na gaveta o nó proximo ou anterior do nó atualmente guardado.
 * Parametros: Lista, opção de gaveta, direção do movimento.
 * Erros: 9. */
Erro lista_andaGaveta(Lista *l, char gavetaQual, bool direcao) {
   switch (gavetaQual) {
      case 1:
         lista_atualizaGaveta(l, &((Lista_sentDado *) (l->sentinela->dado))->gavetaUm, &(((Lista_sentDado *) (l->sentinela->dado))->posUm), direcao);
         break;
      case 2:
         lista_atualizaGaveta(l, &((Lista_sentDado *) (l->sentinela->dado))->gavetaDois, &(((Lista_sentDado *) (l->sentinela->dado))->posDois), direcao);
         break;
      case 3:
         lista_atualizaGaveta(l, &((Lista_sentDado *) (l->sentinela->dado))->gavetaUm, &(((Lista_sentDado *) (l->sentinela->dado))->posUm), direcao);
         lista_atualizaGaveta(l, &((Lista_sentDado *) (l->sentinela->dado))->gavetaDois, &(((Lista_sentDado *) (l->sentinela->dado))->posDois), direcao);
         break;
      default:
         return 9;
   }
   return 0;
}

/* Igual o centeudo de ambas gavetas.
 * Parametros: Lista, gaveta com o coteudo de origem.
 * Erros: 9. */
Erro lista_igualaGaveta(Lista *l, char gavetaOrg) {
   switch (gavetaOrg) {
      case 1:
         ((Lista_sentDado *) l->sentinela->dado)->gavetaDois = ((Lista_sentDado *) l->sentinela->dado)->gavetaUm;
         ((Lista_sentDado *) l->sentinela->dado)->posDois = ((Lista_sentDado *) l->sentinela->dado)->posUm;
         break;
      case 2:
         ((Lista_sentDado *) l->sentinela->dado)->gavetaUm = ((Lista_sentDado *) l->sentinela->dado)->gavetaDois;
         ((Lista_sentDado *) l->sentinela->dado)->posUm = ((Lista_sentDado *) l->sentinela->dado)->posDois;
         break;
      default:
         return 9;
   }
   return 0;
}

/* Retorna o dado do nó guardado em uma gaveta.
 * Parametros: Lista, ponteiro de retorno do dado, opção de gaveta.
 * Erros: 9. */
Erro lista_pegaGaveta(Lista *l, void **dadoRetorno, char gavetaQual) {
   if (lista_estaVazia(l))
      return 7;
   switch (gavetaQual) {
      case 1:
         (*dadoRetorno) = ((Lista_sentDado *) l->sentinela->dado)->gavetaUm->dado;
         break;
      case 2:
         (*dadoRetorno) = ((Lista_sentDado *) l->sentinela->dado)->gavetaDois->dado;
         break;
      default:
         return 9;
   }
   return 0;
}

/* Retorna a posição numerica do nó da lista dentro de uma gaveta.
 * Parametros: Lista, inteiro de retorno da posição, opção de gaveta.
 * Erros: 9. */
Erro lista_pegaGavetaPos(Lista *l, int *gavetaPos, char gavetaQual) {
   switch (gavetaQual) {
      case 1:
         *gavetaPos = ((Lista_sentDado *) l->sentinela->dado)->posUm;
         break;
      case 2:
         *gavetaPos = ((Lista_sentDado *) l->sentinela->dado)->posDois;
         break;
      default:
         return 9;
   }
   return 0;
}

/* Retorna o dado que satisfaz a comparação com um dado de entrada, util caso a função de
 * comparação use apenas parte do dado de entrada na comparação e não se sabe o resto. Há
 * opção de guardar o dado encontrado em uma gaveta.
 * Parametros: Lista, dado usada na comparação, ponteiro do dado de retorno, opção de gaveta.
 * Erros: 6 e 7. */
Erro lista_pegaDado(Lista *l, void *dadoEntrada, void **dadoRetorno, char gavetaQual) {
   int conta = l->listaTam, comparacao;
   bool ordenada = l->ordenada;
   Lista_No *percorre = l->sentinela->prox;
   
   if (lista_estaVazia(l))
      return 7;
   while (conta--) {
      if (l->funcCompara == NULL)
         comparacao = memcmp(dadoEntrada, percorre->dado, l->dadoTam);
      else
         comparacao = (l->funcCompara (dadoEntrada, percorre->dado));
      if (!comparacao) {
         *dadoRetorno = percorre->dado;
         lista_entraGaveta(l, gavetaQual, percorre);
         return 0;
      }
      if (ordenada && (comparacao < 0))
         break;
      percorre = percorre->prox;
   }
   return 6;
}

/* Devolve primeiro dado da lista.
 * Parametros: Lista, variavel de retorno, se o dado será novo ou o que está.
 * Erros: 7. */
Erro lista_pegaDadoIni(Lista *l, void **dadoRetorno, bool memoriaNova) {
   if (lista_estaVazia(l)) {
      *dadoRetorno = NULL;
      return 7;
   }
   if (memoriaNova) {
      *dadoRetorno = malloc(sizeof(l->dadoTam));
      if (l->funcCopia == NULL)
         memcpy(*dadoRetorno, l->sentinela->prox->dado, l->dadoTam);
      else
         l->funcCopia(*dadoRetorno, l->sentinela->prox->dado);
   }
   else 
      *dadoRetorno = l->sentinela->prox->dado;
   return 0;
}

/* Devolve ultimo dado da lista.
 * Parametros: Lista, variavel de retorno, se o dado será novo ou o que está.
 * Erros: 7. */
Erro lista_pegaDadoFim(Lista *l, void **dadoRetorno, bool memoriaNova) {
   if (lista_estaVazia(l)) {
      *dadoRetorno = NULL;
      return 7;
   }
   if (memoriaNova) {
      *dadoRetorno = malloc(sizeof(l->dadoTam));
      if (l->funcCopia == NULL)
         memcpy(*dadoRetorno, l->sentinela->ant->dado, l->dadoTam);
      else
         l->funcCopia(*dadoRetorno, l->sentinela->ant->dado);
   }
   else 
      *dadoRetorno = l->sentinela->ant->dado;
   return 0;
}

/* Devolve dado em certa posição da lista.
 * Parametros: Lista, posição do dado, variavel de retorno, se guarda na gaveta.
 * Erros: 7 e 8. */
Erro lista_pegaDadoPos(Lista *l, int pos, void **dadoRetorno, bool memoriaNova, char gavetaQual) {
   Lista_No *percorre = l->sentinela->prox;

   *dadoRetorno = NULL;
   if (lista_estaVazia(l)) 
      return 7;
   if (pos > l->listaTam || pos < 0)
      return 8;
   while (pos--)
      percorre = percorre->prox;
   lista_entraGaveta(l, gavetaQual, percorre);
   if (memoriaNova) {
      *dadoRetorno = malloc(sizeof(l->dadoTam));
      if (l->funcCopia == NULL)
         memcpy(*dadoRetorno, percorre->dado, l->dadoTam);
      else
         l->funcCopia(*dadoRetorno, percorre->dado);
   }
   else 
      *dadoRetorno = percorre->dado;
   return 0;
}

/* Destroi a lista.
 * Parametros: Lista.
 * Erros: Nenhum. */
Erro lista_destroi(Lista **l) {
   int conta = (*l)->listaTam;
   Lista_No *percorre = (*l)->sentinela->prox;
   Lista_No *deleta = NULL;

   // Liberação do sentinela.
   free((*l)->sentinela->dado); 
   free((*l)->sentinela); 
   while (conta--) { // Liberação do resto da lista.
      deleta = percorre;
      percorre = percorre->prox;
      if ((*l)->funcLibera != NULL)
         (*l)->funcLibera(deleta->dado);
      free(deleta->dado);
      free(deleta);
   }
   free(*l);
   *l = NULL;
   return 0; 
}

/* Iterador da lista. Aplica a função a todos os nós.
 * Parametros: Lista, função a ser aplicada.
 * Erros: Nenhum. */ 
Erro lista_itera(Lista *l, dado_usa funcao) {
   int conta = l->listaTam;
   Lista_No *percorre = l->sentinela->prox;
   
   while (conta--) {
      (*funcao)(percorre->dado);
      percorre = percorre->prox;
   }
   return 0;
}

/* Iterador da lista invertida. Aplica a função a todos os nós invertidamente.
 * Parametros: Lista, função a ser aplicada.
 * Erros: Nenhum. */
Erro lista_iteraInvert(Lista *l, dado_usa funcao) {
   int conta = l->listaTam;
   Lista_No *percorre = l->sentinela->ant;
   
   while (conta--) {
      (*funcao)(percorre->dado);
      percorre = percorre->ant;
   }
   return 0;
}

/* Iterador aninhado da lista. Aplica a função de fora que recebe um dado da função de dentro.
 * Parametros: Lista, função de fora, função de dentro.
 * Erros: Nenhum. */
Erro lista_iteraAninhado(Lista *l, dado_usa funFora, dado_aninhado funDentro) {
   int conta = l->listaTam;
   Lista_No *percorre = l->sentinela->prox;
   
   while (conta--) {
      (*funFora)((*funDentro)(percorre->dado));
      percorre = percorre->prox;
   }
   return 0;
} //Obs: Usada para facilitar acesso no grafo.


/* Getters da lista. */

int lista_pegaTam(Lista *l) {
   return l->listaTam;
}

size_t lista_pegaDadoTam(Lista *l) {
   return l->dadoTam;
}

bool lista_pegaehOrdenada(Lista *l) {
   return l->ordenada;
}

bool lista_pegaRepetida(Lista *l) {
   return l->repeticao;
}

