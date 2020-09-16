/* Anderson Cardoso Gonçalves - github.com/ansoncg */

#ifndef DEFINICOES_H
#define DEFINICOES_H

/* - Toda funcionalidade externa de uma estrutura retorna um valor tipo Erro. Retorno 0 é sucesso. 
 * - Toda funcionalidade começa com o nome da estrutura antes. Ex: lista_destroi().
 * - Nos nomes das variaveis, caracteristicas que as diferenciam vem depois do tipo. Ex: dadoUm, dadoNovo.
 * - As estruturas sempre terão partes de mesmo tamanho. Porém é possivel usar ponteiros para regiões que variam de tamanho. */

#define Erro int
#define Sim true
#define Nao false
#define Nada NULL
#define Proximo true
#define Anterior false
#define Zero 0
#define Primeira 1
#define Segunda 2
#define Ambas 3

typedef void (*dado_copia)(void *dest, const void *orig); 
typedef void (*dado_libera)(void *dado);
typedef int (*dado_compara)(const void *dadoUm, const void *dadoDois);
typedef void (*dado_usa)(void *dado); 
typedef void *(*dado_aninhado)(void *dado); 

#endif
