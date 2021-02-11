#ifndef TESTE_UTILIDADES_H
#define TESTE_UTILIDADES_H

#include "../include/definicoes.h"
#include <stddef.h>

typedef struct {
    size_t idade, quantNSorte;
    char *nome;
    int *numerosSorte;
} Pessoa; 

enum dadoTipos {
    INT, FLOAT, STR, STRUCT, ESTR
};

void pessoa_cria(Pessoa *p, size_t idade, char *nome, size_t quantNSorte, ...); 
void pessoa_destroi(Pessoa *p); 
void pessoa_imprime(void *dado); 
void pessoa_imprimeS(Pessoa p, char tmp[1000]); 
dadoCopia(pessoa_copia); 
dadoCompara(pessoa_compara); 
dadoLibera(pessoa_libera); 

#endif
