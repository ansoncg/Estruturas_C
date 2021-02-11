/* Anderson Cardoso Gonçalves - github.com/ansoncg */

#include <stdlib.h>
#include "../include/pilha.h"

/* Para mais explicações ver TAD lista.
 * Erros de retorno da pilha são os mesmos da lista. */

/* Estrutura da pilha usando lista. */
struct pilha {
   Lista *lista;
};


/* Cria a pilha. */
Erro pilha_cria(Pilha **p, size_t dadoTam, bool repeticao, dado_copia funcCopia, dado_libera funcLibera, dado_compara funcCompara) {
   *p = malloc(sizeof(Pilha));
   return lista_cria(&(*p)->lista, dadoTam, false, repeticao, funcCopia, funcLibera, funcCompara);
}

/* Insere dado generico na pilha. */
Erro pilha_insere(Pilha *p, void *dado) {
   return lista_insereIni(p->lista, dado);
}

/* Remove o proximo dado da pilha. */
Erro pilha_remove(Pilha *p) {
   return lista_removeIni(p->lista);
}

/* Retorna o proximo dado da pilha, como dado novo ou dentro da pilha. */
Erro pilha_pegaProx(Pilha *p, void **dadoRetorno, bool memoriaNova) {
   return lista_pegaDadoIni(p->lista, dadoRetorno, memoriaNova); 
}

/* Executa uma função em todos os dados da pilha. */
Erro pilha_itera(Pilha *p, dado_usa funcao) {
   return lista_itera(p->lista, funcao);
}

/* Devolve o tamanho da pilha. */
size_t pilha_pegaTam(Pilha *p) {
   return lista_pegaTam(p->lista);
}

/* Libera toda a memória da pilha. */
Erro pilha_destroi(Pilha **p) {
   lista_destroi(&(*p)->lista);
   free (*p);
   *p = NULL;
   return SEM_ERRO;
}
