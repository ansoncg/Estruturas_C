/* Anderson Cardoso Gonçalves - github.com/ansoncg */

#ifndef STRING_H
#define STRING_H

#include <stdio.h>
#include <stddef.h>
#include "definicoes.h"


typedef struct stringVariavel StringVar; 

// Uso geral.
Erro strVar_pegaStr(StringVar *sv, char **string); 
Erro strVar_cria(StringVar **sv); 
Erro strVar_copia(StringVar *dest, const StringVar *orig, const char *origCStr); 
Erro strVar_junta(StringVar *dest, const StringVar *orig, const char *origCStr); 
Erro strVar_destroi(StringVar **sv); 
Erro strVar_compara(const StringVar *svUm, const StringVar *svDois, int *resultado); 
Erro strVar_le(FILE *entrada, StringVar **sv); 
Erro strVar_pegaTam(const StringVar *sv, size_t *tamStr); 
Erro strVar_imprime(const StringVar *sv); 
Erro strVar_imprimeV(const StringVar *sv); 

// Uso dentro de estruturas.
size_t strVar_dadoTam();  
void strVar_dadoCopia(void *dest, const void *orig); 
int strVar_dadoCompara(const void *dadoUm, const void *dadoDois); 
int strVar_dadoComparaVet(const void *dadoUm, const void *dadoDois); 
void strVar_dadoLibera(void *dado); 
void strVar_dadoImprime(void *dado); 
void strVar_dadoImprimeV(void *dado); 

#endif
