/* Anderson Cardoso Gonçalves - github.com/ansoncg */

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "../include/lista.h"
#include "../include/utilidades.h"
#include "../include/grafoLA.h"

/* Grafo com as seguintes caracteristicas:
 * Listas de adjacencia
 * Opção de ser valorado ou não
 * Opção de ser direcionado ou não
 * Valor da aresta é um dado generico
 * Permite escolher entre ter repetição de vertices ou não mas não há essa opção para os valores da aresta
 * Em um grafo valorado não direcionado será a mesma memória para os dois ponteiros nas listas, evitando informação duplicada

 * Erros de retorno do grafoLA:
 * 4 - Grafo não valorado, função incompativel.
 * */

//TODO: Colocar erros, comentarios e algoritmos de grafos

typedef struct gLA_valoradoDado {
   int vertice;
   bool *desaloca; // Guarda se o valor da aresta deve ser desalocado, evita double-free em grafos valorados não direcionados.
   void *arestaValor; 
} gLA_valDado;

struct grafoLA {
   Lista **listas;
   int verticesQuant;  
   bool direcionado;
   bool valorado;
   size_t val_dadoTam;
   dado_copia funcCopia;
   dado_libera funcLibera;
   dado_compara funcCompara;
};

/* Funções auxiliares. */

static void gLA_liberaNaLista(void *dado) {
   if (*(((gLA_valDado *) dado)->desaloca)) {
      free(((gLA_valDado *) dado)->arestaValor);
      free(((gLA_valDado *) dado)->desaloca);
   }
   else 
      (*(((gLA_valDado *) dado)->desaloca)) = true;
   return; 
}

static int gLA_comparaNaLista(const void *dadoUm, const void *dadoDois) {
   int vertUm = ((gLA_valDado *) dadoUm)->vertice,
       vertDois = ((gLA_valDado *) dadoDois)->vertice;
   return (vertUm < vertDois) ? -1 : (vertUm > vertDois);
}

static void* grafoLA_pegaDadoImp(void *gLA_val) {
   printf("%d-", ((gLA_valDado *) gLA_val)->vertice);
   return (((gLA_valDado *) gLA_val)->arestaValor);
}

static void* grafoLA_pegaDado(void *gLA_val) {
   return (((gLA_valDado *) gLA_val)->arestaValor);
}

static void grafoLA_novoVal(GrafoLA *gLA, int vertice, void *valor, gLA_valDado *novoVal) {
   (novoVal)->vertice = vertice;
   (novoVal)->arestaValor = malloc(gLA->val_dadoTam); 
   (novoVal)->desaloca = malloc(sizeof(bool));
   *((novoVal)->desaloca) = true;
   if (gLA->funcCopia != NULL) 
      gLA->funcCopia(((novoVal)->arestaValor), valor);
   else
      memcpy(((novoVal)->arestaValor), valor, gLA->val_dadoTam);
   return;
}

/* Funcionalidades. */

Erro grafoLA_cria(GrafoLA **gLA, int verticesQuant, bool direcionado, bool valorado, bool repeticao, size_t val_dadoTam, dado_copia funcCopia, dado_libera funcLibera, dado_compara funcCompara) {
   (*gLA) = malloc(sizeof(GrafoLA));
   (*gLA)->listas = malloc(sizeof(Lista *)*((size_t) verticesQuant));
   (*gLA)->verticesQuant = verticesQuant;
   (*gLA)->direcionado = direcionado; 
   (*gLA)->valorado = valorado;
   (*gLA)->val_dadoTam = val_dadoTam;
   (*gLA)->funcCopia = funcCopia;
   (*gLA)->funcLibera = funcLibera; // Libera dentro do void *arestaValor se precisar
   (*gLA)->funcCompara = funcCompara;
   if (valorado) 
      for (int i = 0; i < verticesQuant; i++) 
         lista_cria(&((*gLA)->listas[i]), sizeof(gLA_valDado), false, repeticao, NULL, gLA_liberaNaLista, gLA_comparaNaLista);
   else 
      for (int i = 0; i < verticesQuant; i++) 
         lista_cria(&((*gLA)->listas[i]), sizeof(int), false, repeticao, NULL, NULL, NULL);
   return 0; 
}

Erro grafoLA_insereAresta(GrafoLA *gLA, int verticeUm, int verticeDois, void *valor) {
   gLA_valDado novoVal;

   if (verticeUm >= gLA->verticesQuant || verticeDois >= gLA->verticesQuant || verticeUm < 0 || verticeDois < 0)
      return 8;   
   if (gLA->valorado) {
      grafoLA_novoVal(gLA, verticeDois, valor, &novoVal);  
      if (lista_insereFim(gLA->listas[verticeUm], &novoVal) == 1) { // Testa por repetição.
         if (gLA->funcLibera != NULL)
            gLA->funcLibera(novoVal.arestaValor);
         free(novoVal.arestaValor);
         free(novoVal.desaloca);
         return 1; 
      }
      if (!(gLA->direcionado)) {
         novoVal.vertice = verticeUm;
         *(novoVal.desaloca) = false; // Muda em ambas as listas pois é um dado só.
         lista_insereFim(gLA->listas[verticeDois], &novoVal); 
      }        
   }
   else {
      lista_insereFim(gLA->listas[verticeUm], &verticeDois); 
      if (!(gLA->direcionado))
         lista_insereFim(gLA->listas[verticeDois], &verticeUm); 
   }
   return 0;
}

Erro grafoLA_removeAresta(GrafoLA *gLA, int verticeUm, int verticeDois) {
   gLA_valDado val;
   void *valRm = NULL;

   if (gLA->valorado) {
      if (!(gLA->direcionado)) { // É apenas um valor para as duas listas, então não precisa remove-lo aqui.
         val.vertice = verticeUm; // Vertice a ser removido
         lista_removeDado(gLA->listas[verticeDois], &val, false);   
      }
      val.vertice = verticeDois; 
      if (gLA->funcLibera != NULL) { // Dado complexo dentro do valor da aresta.
         lista_pegaDado(gLA->listas[verticeUm], &val, &valRm, 1); // val = dado de entrada, valrm = dado de retorno   
         gLA->funcLibera(((gLA_valDado *) valRm)->arestaValor); // Função especifica do usuario para liberar seus ponteiros.
         lista_rmNoGaveta(gLA->listas[verticeUm], 1);
      }
      else
         lista_removeDado(gLA->listas[verticeUm], &val, false);   
   }
   else {
      lista_removeDado(gLA->listas[verticeUm], &verticeDois, false);
      if (!(gLA->direcionado))
         lista_removeDado(gLA->listas[verticeDois], &verticeUm, false);
   }
   return 0;
}

Erro grafoLA_destroi(GrafoLA **gLA) {
   void *valRm = NULL; // Guarda o dado complexo a ser liberado caso exista.
   bool direcionado = (*gLA)->direcionado;
   int listaTam;
   
   // Remove o dado complexo primeiro passando pelo grafo todo.
   if (((*gLA)->valorado) && ((*gLA)->funcLibera != NULL)) { // Dado complexo dentro do valor da aresta.
      for (int i = 0; i < (*gLA)->verticesQuant; i++) {
         listaTam = lista_pegaTam((*gLA)->listas[i]);
         lista_esvaziaGaveta((*gLA)->listas[i], 2);
         while (listaTam--) {
            lista_andaGaveta((*gLA)->listas[i], 2, Proximo); 
            lista_pegaGaveta((*gLA)->listas[i], &valRm, 2); 
            if (*(((gLA_valDado *) valRm)->desaloca)) {
               (*gLA)->funcLibera(((gLA_valDado *) valRm)->arestaValor); // Função especifica do usuario para liberar seus ponteiros.
               if (!direcionado)
                  (*((gLA_valDado *) valRm)->desaloca) = false; // Deixa em falso caso não seja direcionado, para o lista_destroi(). 
            }
            else 
               (*((gLA_valDado *) valRm)->desaloca) = true; 
         }
      }
   }
   for (int i = 0; i < (*gLA)->verticesQuant; i++)
      lista_destroi(&((*gLA)->listas[i]));
   free((*gLA)->listas);
   free(*gLA);
   *gLA = NULL;
   return 0;
}

Erro grafoLA_imprime(GrafoLA *gLA) {
   for (int i = 0; i < gLA->verticesQuant; i++) {
      printf ("%d: ", i);
      lista_itera(gLA->listas[i], imprimeInteiro);
      putchar('\n');
   }
   return 0;
}

Erro grafoLA_imprimeVal(GrafoLA *gLA, dado_usa funcao) {
   if (!(gLA->valorado))
      return 4;
   for (int i = 0; i < gLA->verticesQuant; i++) {
      printf ("%d: ", i);
      lista_iteraAninhado(gLA->listas[i], funcao, grafoLA_pegaDadoImp);
      putchar('\n');
   }
   return 0;
}

/* IteraAninhado facilita o acesso do grafo aos dados, caso contrario teria que ir pegando
nó por nó salvando a posição na gaveta. */
Erro grafoLA_iteraValor(GrafoLA *gLA, dado_usa funcao) {
   for (int i = 0; i < gLA->verticesQuant; i++)  
      lista_iteraAninhado(gLA->listas[i], funcao, grafoLA_pegaDado); // Liga apenas para o valor.
   return 0;
}

Erro grafoLA_iteraVertice(GrafoLA *gLA, dado_usa funcao); 
