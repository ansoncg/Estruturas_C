/* Anderson Cardoso Gonçalves - github.com/ansoncg */

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

/* Fazedor de Makefile para programas que usam as estruturas. */

#define quantOps 6

int main () {
   FILE *makefile = fopen("Makefile", "w");
   char op, caminhos[200] = "";
   int *ops = calloc (quantOps + 1, sizeof(int));

   // Colocar opções em letras em vez de numeros
   // Colocar arquivos .o
   printf ("Quais estruturas seu programa usa?\n"
         "1 - Lista\n"
         "2 - Fila\n"
         "3 - Pilha\n"
         "4 - Grafo por lista de adjacencia\n"
         "5 - Grafo por matriz de adjacencia\n"
         "6 - Vetor\n");
   while ((op = getchar()) != '\n') 
      ops[op - '0']++;
   for (int i = 1; i <= quantOps; i++) {
      if (ops[i]) {
         switch (i) {
            case 1:
               strcat(caminhos, " ../Estruturas/Lista/lista.c ");
               break;
            case 2:
               if (!ops[1])
                  strcat(caminhos, " ../Estruturas/Lista/lista.c ");
               strcat(caminhos, " ../Estruturas/Fila/fila.c ");
               break;
            case 3:
               if (!ops[1] && !ops[2])
                  strcat(caminhos, " ../Estruturas/Lista/lista.c ");
               strcat(caminhos, " ../Estruturas/Pilha/pilha.c ");
               break;
            case 4:
               if (!ops[1] && !ops[2] && !ops[3])
                  strcat(caminhos, " ../Estruturas/Lista/lista.c ");
               strcat(caminhos, " ../Estruturas/Grafo/grafoLA.c ");
               break;
            case 5:
               strcat(caminhos, " ../Estruturas/Grafo/grafoMA.c ");
               break;
            case 6:
               strcat(caminhos, " ../Estruturas/Vetor/vetor.c ");
               break;
         }
      }
   } 
   fprintf(makefile, "all:\n"
         "\tgcc -march=native -O3 -o programa *.c ../Estruturas/Comum/utilidades.c %s\n"
         "s:\n"
         "\tgcc -Wall -Wextra -fsanitize=address -o programaTeste *.c ../Estruturas/Comum/utilidades.c %s\n"
         "\t./programaTeste\n"
         "v:\n"
         "\tgcc -Wall -Wextra -o programaTeste *.c ../Estruturas/Comum/utilidades.c %s\n"
         "\tvalgrind ./programaTeste\n"
         "run:\n"
         "\t./programa\n", caminhos, caminhos, caminhos); 
   free(ops);
   fclose(makefile);
   return 0;
}
