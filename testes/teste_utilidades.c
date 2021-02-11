#include <stdio.h>
#include <stdlib.h>
#include <stdarg.h>
#include <string.h>
#include "teste_utilidades.h"

void pessoa_cria(Pessoa *p, size_t idade, char *nome, size_t quantNSorte, ...) {
    va_list varLista;

    va_start(varLista, quantNSorte);
    p->idade = idade;
    p->nome = malloc(strlen(nome) + 1);
    strcpy(p->nome, nome);
    p->quantNSorte = quantNSorte;
    p->numerosSorte = malloc(sizeof(int)*quantNSorte);         
    for (size_t i = 0; i < quantNSorte; i++) 
        p->numerosSorte[i] = va_arg(varLista, int);
    va_end(varLista);
    return;
}

void pessoa_destroi(Pessoa *p) {
    free(p->nome);
    free(p->numerosSorte);
    return;
}

void pessoa_imprime(void *dado) { // Imprime stdout.
    Pessoa p = *(Pessoa *) dado;
    printf("Nome: %5s | Idade: %2ld | NSortes: ", p.nome, p.idade);
    for (size_t i = 0; i < p.quantNSorte; i++)
        printf("%3d ", p.numerosSorte[i]);
    printf("|\n");
    return;
}

void pessoa_imprimeS(Pessoa p, char tmp[1000]) { // Imprime para string.
    char aux[100];

    sprintf(tmp, "Nome: %5s | Idade: %2ld | NSortes: ", p.nome, p.idade);
    for (size_t i = 0; i < p.quantNSorte; i++) {
        sprintf(aux, "%3d ", p.numerosSorte[i]);
        strcat(tmp, aux);
    }
    strcat(tmp, "|\n");
    return;
}

dadoCopia(pessoa_copia) {
    Pessoa p = *(Pessoa *) orig;

    ((Pessoa *) dest)->nome = malloc(strlen(p.nome) + 1);
    strcpy(((Pessoa *) dest)->nome, p.nome);
    ((Pessoa *) dest)->numerosSorte = malloc(sizeof(int)*p.quantNSorte); 
    for (size_t i = 0; i < p.quantNSorte; i++) 
        ((Pessoa *) dest)->numerosSorte[i] = p.numerosSorte[i];     
    ((Pessoa *) dest)->idade = p.idade;
    ((Pessoa *) dest)->quantNSorte = p.quantNSorte;
    return;
}

dadoCompara(pessoa_compara) { // Compara por idade.
    return ((int) ((Pessoa *) dadoUm)->idade - (int) ((Pessoa *) dadoDois)->idade);
}

dadoLibera(pessoa_libera) {
    free(((Pessoa *) dado)->nome);
    free(((Pessoa *) dado)->numerosSorte);
    return;
}
