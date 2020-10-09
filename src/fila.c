/* Anderson Cardoso Gonçalves - github.com/ansoncg */

#include <stdlib.h>
#include "../include/fila.h"

/* Para mais explicações ver TAD lista.
 * Erros de retorno da fila são os mesmos da lista. */

/* Estrutura da fila usando lista. */
struct fila {
   Lista *lista;
};


/* Cria a fila. */
Erro fila_cria(Fila **f, size_t dadoTam, bool repeticao, dado_copia funcCopia, dado_libera funcLibera, dado_compara funcCompara) {
   *f = malloc(sizeof(Fila));
   return lista_cria(&(*f)->lista, dadoTam, false, repeticao, funcCopia, funcLibera, funcCompara);
}

/* Insere dado generico na fila. */
Erro fila_insere(Fila *f, void *dado) {
   return lista_insereFim(f->lista, dado);
}

/* Remove o proximo dado da fila. */
Erro fila_remove(Fila *f) {
   return lista_removeIni(f->lista);
}

/* Retorna o proximo dado da fila, como dado novo ou dentro da fila. */
Erro fila_pegaProx(Fila *f, void **dadoRetorno, bool memoriaNova) {
   return lista_pegaDadoIni(f->lista, dadoRetorno, memoriaNova); 
}

/* Executa uma função em todos os dados da fila. */
Erro fila_itera(Fila *f, dado_usa funcao) {
   return lista_itera(f->lista, funcao);
}

/* Devolve o tamanho da fila. */
int fila_pegaTam(Fila *f) {
   return lista_pegaTam(f->lista);
}

/* Libera toda a memória da fila. */
Erro fila_destroi(Fila **f) {
   lista_destroi(&(*f)->lista);
   free (*f);
   *f = NULL;
   return 0;
}

