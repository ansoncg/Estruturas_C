/* Anderson Cardoso Gonçalves - github.com/ansoncg */

/* Lista com as seguintes caracteristicas:
 * - Dados genericos
 * - Deep copy dos dados
 * - Duplamente encadeada
 * - Circular
 * - Opção de ser ordenada ou não
 * - Opção de aceitar repetições ou não

 * Funções de copia, liberação e comparação são necessarias apenas para dados complexos, ou seja, uma
 struct com ponteiros dentro. 
 */

#include <string.h>
#include <stdlib.h>
#include "../include/lista.h"

/* Estrutura do nó. */
typedef struct lista_no {
    void *dado;
    struct lista_no *prox;
    struct lista_no *ant;
} Lista_No;

/* Estrutura da lista. Caracteristicas
 * da lista aqui para facilitar acesso.
 * Funções de copia, liberação e comparação são 
 * necessarias apenas em dados que contém ponteiros. */
struct lista {
    Lista_No *referencia; // Referencia da lista circular.
    size_t dadoTam; // Tamanho do dado dentro da lista.
    size_t listaTam; // Tamanho atual da lista.
    bool ordenada; // Se é uma lista ordenada.
    bool repeticao; // Se a lista aceita dados repetidos.
    dado_copia funcCopia; // Se não for implementada (NULL) será um memcpy() do dado.
    dado_libera funcLibera; // Se não for implementada (NULL) será apenas um free() do dado.
    dado_compara funcCompara; // Se não for implementada (NULL) será apenas um memcmp() dos dados.
};


/* ### Funções internas à lista. ### */

static void lista_copiaDado(Lista *l, void *dadoDest, const void *dadoOrig) {
    if (l->funcCopia == NULL)
        memcpy(dadoDest, dadoOrig, l->dadoTam);
    else
        l->funcCopia(dadoDest, dadoOrig);
    return;
}

static int lista_comparaDado(Lista *l, const void *dadoUm, const void *dadoDois) {
    if (l->funcCompara == NULL)
        return memcmp(dadoUm, dadoDois, l->dadoTam);
    else
        return (l->funcCompara (dadoUm, dadoDois));
}

static void lista_liberaDado(Lista *l, void *dado) {
    if (l->funcLibera != NULL)
        l->funcLibera(dado);
    free(dado);
    return;
}

/**
 * Aloca memoria para 'noNovo'e insere 'dadoNovo' nele.
 * @param noNovo No de lista destino.
 * @param dadoNovo O dado que sera inserido no novo no.
 * @param l Lista que 
 */
static void lista_criaNo(Lista_No **noNovo, const void *dadoNovo, Lista *l) {
    (*noNovo) = malloc(sizeof(Lista_No));
    (*noNovo)->dado = malloc(l->dadoTam);
    lista_copiaDado(l, (*noNovo)->dado, dadoNovo);
    (*noNovo)->prox = (*noNovo)->ant = NULL;
    return; 
}

/* Cria um referencia com seu dado especifico.
 * Parametros: referencia da lista, tamanho do dado da lista.
 * Erros: Nenhum. */
static void lista_IniciaReferencia(Lista_No **referencia) {
    (*referencia) = malloc(sizeof(Lista_No));
    (*referencia)->dado = NULL;
    (*referencia)->prox = (*referencia)->ant = *referencia;
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
static void lista_insereNo(Lista *l, Lista_No *noAntes, Lista_No *noDepois, const void *dado) {
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
    lista_liberaDado(l, noDeleta->dado);
    free(noDeleta);
    (l->listaTam)--; 
    return;
}

/* Testa se 'dado' já pertence a lista 'l'.
 * Parametros: Lista, dado generico.
 * Erros: 1. */
static Erro lista_veSeRepete(Lista *l, const void *dado) {
    size_t conta = l->listaTam;
    int comparacao;
    Lista_No *percorre = l->referencia->prox;

    while (conta--) {
        comparacao = lista_comparaDado(l, dado, percorre->dado);
        if (!comparacao)
            return REPETIDO;
        percorre = percorre->prox;
    }
    return SEM_ERRO;
}

/* ### Funcionalidades da lista. ### */

/* Cria a lista, precisa ser chamada antes de tudo.
 * Parametros: Lista, tamanho do dado, se será ordenada, se aceitará repetição, funções de copia, liberação e comparação.
 * Erros: Nenhum. */
Erro lista_cria(Lista **l, size_t dadoTam, bool ordenada, bool repeticao, dado_copia funcCopia, dado_libera funcLibera, dado_compara funcCompara) {
    Lista_No *referencia = NULL;

    (*l) = malloc(sizeof(Lista));
    (*l)->dadoTam = dadoTam;
    (*l)->listaTam = 0;
    (*l)->ordenada = ordenada;
    (*l)->repeticao = repeticao;
    (*l)->funcCopia = funcCopia;
    (*l)->funcLibera = funcLibera;
    (*l)->funcCompara = funcCompara;
    lista_IniciaReferencia(&referencia);
    (*l)->referencia = referencia;
    return SEM_ERRO;
}

/* Insere no inicio da lista. 
 * Parametros: Lista, dado generico.
 * Erros: 1, 2. */
Erro lista_insereIni(Lista *l, const void *dado) {
    if (l->ordenada)
        return ORDENADA;
    if (!(l->repeticao) && (lista_veSeRepete(l, dado)))
        return REPETIDO;
    lista_insereNo(l, l->referencia, l->referencia->prox, dado);
    return SEM_ERRO;
} 

/* Insere no fim da lista. 
 * Parametros: Lista, dado generico.
 * Erros: 1, 2.  */
Erro lista_insereFim(Lista *l, const void *dado) {
    if (l->ordenada)
        return ORDENADA;
    if (!(l->repeticao) && (lista_veSeRepete(l, dado)))
        return REPETIDO;
    lista_insereNo(l, l->referencia->ant, l->referencia, dado);
    return SEM_ERRO;
} 

/* Insere na lista ordenada.
 * Parametros: Lista, dado generico.
 * Erros: 1, 3. */ 
Erro lista_insereOrd(Lista *l, const void *dado) {
    size_t conta = l->listaTam; 
    int comparacao;
    bool repeticao = l->repeticao;
    Lista_No *percorre = l->referencia->prox;

    if (!(l->ordenada))
        return NAO_ORDENADA;
    while (conta--) {
        comparacao = lista_comparaDado(l, dado, percorre->dado);
        if (!repeticao && !comparacao)
            return REPETIDO;
        if (comparacao < 0)
            break;
        percorre = percorre->prox;
    }  
    lista_insereNo(l, percorre->ant, percorre, dado);
    return SEM_ERRO;
}

/* Remove primeiro elemento da lista.
 * Parametros: Lista. 
 * Erros: 5. */
Erro lista_removeIni(Lista *l) {
    if (!(l->listaTam))  // Checa por lista vazia (só referencia).
        return ESTR_VAZIA;
    lista_removeNo(l, l->referencia, l->referencia->prox, l->referencia->prox->prox);
    return SEM_ERRO;
}

/* Remove ultimo elemento da lista.
 * Parametros: Lista. 
 * Erros: 5. */
Erro lista_removeFim(Lista *l) {
    if (!(l->listaTam))  // Checa por lista vazia (só referencia).
        return ESTR_VAZIA;
    lista_removeNo(l, l->referencia->ant->ant, l->referencia->ant, l->referencia);
    return SEM_ERRO;
}

/* Remove um ou todos os elementos da lista que contem um dado especifico.
 * Parametros: Lista, dado para remover, opção de ser todos.
 * Erros: 5 e 6. */
Erro lista_removeDado(Lista *l, const void *dado, bool todos) {
    Erro erro = NAO_ENCONTRADO;
    size_t conta = l->listaTam;
    int comparacao;
    Lista_No *percorre = l->referencia->prox;
    Lista_No *deleta = NULL;

    if (!(l->listaTam)) 
        return ESTR_VAZIA;
    while (conta--) {
        comparacao = lista_comparaDado(l, dado, percorre->dado);
        if (!(comparacao)) {
            deleta = percorre;
            percorre = percorre->prox;
            lista_removeNo(l, deleta->ant, deleta, deleta->prox);
            erro = SEM_ERRO;
            if (!todos || !(l->repeticao))
                break;
        }
        else
            percorre = percorre->prox;
    }
    return erro;
}

/* Busca se um dado está na lista e devolve sua posição em 'pos'.
 * Parametros: Lista, dado a ser buscado, variavel de retorno.
 * Erros: 6 e 7. */ 
Erro lista_contemDado(Lista *l, const void *dado, size_t *pos) {
    size_t conta = l->listaTam;
    int comparacao;
    bool ordenada = l->ordenada;
    Lista_No *percorre = l->referencia->prox;

    if (lista_estaVazia(l))
        return ESTR_VAZIA;
    while (conta--) {
        comparacao = lista_comparaDado(l, dado, percorre->dado);
        if (!comparacao) {
            *pos = l->listaTam - conta - 1;
            return SEM_ERRO;
        }
        if (ordenada && (comparacao < 0))
            break;
        percorre = percorre->prox;
    }
    return NAO_ENCONTRADO;
}

/* Retorna o dado que satisfaz a comparação com um dado de entrada, util caso a função de
 * comparação use apenas parte do dado de entrada na comparação e não se sabe o resto. Há
 * opção de guardar o dado encontrado em uma gaveta.
 * Parametros: Lista, dado usada na comparação, ponteiro do dado de retorno, opção de gaveta.
 * Erros: 6 e 7. */
Erro lista_pegaDado(Lista *l, void *dadoEntrada, void **dadoRetorno) {
    size_t conta = l->listaTam; 
    int comparacao;
    bool ordenada = l->ordenada;
    Lista_No *percorre = l->referencia->prox;

    if (lista_estaVazia(l))
        return ESTR_VAZIA;
    while (conta--) {
        comparacao = lista_comparaDado(l, dadoEntrada, percorre->dado);
        if (!comparacao) {
            *dadoRetorno = percorre->dado;
            return SEM_ERRO;
        }
        if (ordenada && (comparacao < 0))
            break;
        percorre = percorre->prox;
    }
    return NAO_ENCONTRADO;
}


/* Devolve dado em certa posição da lista.
 * Parametros: Lista, posição do dado, variavel de retorno, se guarda na gaveta.
 * Erros: 7 e 8. */
Erro lista_pegaDadoPos(Lista *l, size_t pos, void **dadoRetorno, bool memoriaNova) {
    Lista_No *percorre = NULL;

    *dadoRetorno = NULL;
    if (lista_estaVazia(l)) 
        return ESTR_VAZIA;
    if (pos > l->listaTam)
        return POS_INVALIDA;
    if (pos <= (l->listaTam / 2)) {
        percorre = l->referencia->prox;
        while (pos--)
            percorre = percorre->prox;
    }
    else {
        percorre = l->referencia->ant;
        pos = l->listaTam - pos - 1;
        while (pos--)
            percorre = percorre->ant;
    }
    if (memoriaNova) {
        *dadoRetorno = malloc(sizeof(l->dadoTam));
        lista_copiaDado(l, *dadoRetorno, percorre->dado);
    }
    else 
        *dadoRetorno = percorre->dado;
    return SEM_ERRO;
}

/* Devolve primeiro dado da lista.
 * Parametros: Lista, variavel de retorno, se o dado será novo ou o que está.
 * Erros: 7. */
Erro lista_pegaDadoIni(Lista *l, void **dadoRetorno, bool memoriaNova) {
    return lista_pegaDadoPos(l, 0, dadoRetorno, memoriaNova);
}

/* Devolve ultimo dado da lista.
 * Parametros: Lista, variavel de retorno, se o dado será novo ou o que está.
 * Erros: 7. */
Erro lista_pegaDadoFim(Lista *l, void **dadoRetorno, bool memoriaNova) {
    return lista_pegaDadoPos(l, l->listaTam - 1, dadoRetorno, memoriaNova);
}

/* Destroi a lista.
 * Parametros: Lista.
 * Erros: Nenhum. */
Erro lista_destroi(Lista **l) {
    size_t conta = (*l)->listaTam;
    Lista_No *percorre = (*l)->referencia->prox;
    Lista_No *deleta = NULL;

    // Liberação da referencia.
    free((*l)->referencia); 
    while (conta--) { // Liberação do resto da lista.
        deleta = percorre;
        percorre = percorre->prox;
        lista_liberaDado(*l, deleta->dado);
        free(deleta);
    }
    free(*l);
    *l = NULL;
    return SEM_ERRO; 
}

/* Iterador da lista. Aplica a função a todos os nós.
 * Parametros: Lista, função a ser aplicada.
 * Erros: Nenhum. */ 
Erro lista_itera(Lista *l, dado_usa funcao) {
    size_t conta = l->listaTam;
    Lista_No *percorre = l->referencia->prox;

    while (conta--) {
        (*funcao)(percorre->dado);
        percorre = percorre->prox;
    }
    return SEM_ERRO;
}

/* Iterador da lista invertida. Aplica a função a todos os nós invertidamente.
 * Parametros: Lista, função a ser aplicada.
 * Erros: Nenhum. */
Erro lista_iteraInvertido(Lista *l, dado_usa funcao) {
    size_t conta = l->listaTam;
    Lista_No *percorre = l->referencia->ant;

    while (conta--) {
        (*funcao)(percorre->dado);
        percorre = percorre->ant;
    }
    return SEM_ERRO;
}


/* Getters da lista. */

size_t lista_pegaTam(Lista *l) {
    return l->listaTam;
}

size_t lista_pegaDadoTam(Lista *l) {
    return l->dadoTam;
}

bool lista_pegaOrdenada(Lista *l) {
    return l->ordenada;
}

bool lista_pegaRepetida(Lista *l) {
    return l->repeticao;
}
