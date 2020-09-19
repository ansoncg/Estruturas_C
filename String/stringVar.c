#include <stdlib.h>
#include <string.h>
#include "stringVar.h"

struct stringVariavel {
   char *str;
   size_t strTam;
};

Erro strVar_pegaStr(StringVar *sv, char **string) {
   *string = sv->str; 
   return 0;
}

Erro strVar_cria(StringVar **sv) {
   *sv = malloc(sizeof(StringVar));
   (*sv)->str = malloc(1);
   (*sv)->str[0] = '\0';
   (*sv)->strTam = 0;
   return 0;
}

Erro strVar_copia(StringVar *dest, const StringVar *orig, const char *origCStr) {
   size_t strTam;

   if (dest == NULL)
      return 10;
   if (orig != NULL) {
      if (orig->strTam != dest->strTam) {
         dest->str = realloc(dest->str, orig->strTam + 1);        
         dest->strTam = orig->strTam;
      }
      strcpy(dest->str, orig->str);
   }
   else {
      strTam = strlen(origCStr);
      if (strTam != dest->strTam) {
         dest->str = realloc(dest->str, strTam + 1);        
         dest->strTam = strTam;
      }
      strcpy(dest->str, origCStr);
   }
   return 0;
}

Erro strVar_junta(StringVar *dest, const StringVar *orig, const char *origCStr) {
   size_t strTam;

   if (dest == NULL)
      return 10;
   if (orig != NULL) {
      dest->str = realloc(dest->str, orig->strTam + dest->strTam + 1);        
      dest->strTam += orig->strTam;
      strcat(dest->str, orig->str);
   }
   else {
      strTam = strlen(origCStr);
      dest->str = realloc(dest->str, strTam + dest->strTam + 1);        
      dest->strTam += strTam;
      strcat(dest->str, origCStr);
   }
   return 0;
}

Erro strVar_destroi(StringVar **sv) {
   if (*sv == NULL)
      return 10;
   free((*sv)->str);
   free(*sv);  
   (*sv) = NULL;
   return 0;
}

Erro strVar_compara(const StringVar *svUm, const StringVar *svDois, int *resultado) {
   *resultado = strcmp(svUm->str, svDois->str); 
   return 0;
}

Erro strVar_le(FILE *entrada, StringVar **sv) {
   int strTam = 32, i = 0;
   char c;

   if (*sv == NULL) 
      return 10;
   else 
      free ((*sv)->str);
   (*sv)->str = malloc(strTam);
   while (((c = getc(entrada)) != '\n') && c != EOF) {
      if (i == strTam) {
         strTam *= 2;
         (*sv)->str = realloc((*sv)->str, strTam);
      }
      (*sv)->str[i++] = c;
   }
   (*sv)->strTam = i;
   (*sv)->str[i] = '\0';
   (*sv)->str = realloc((*sv)->str, i + 1);
   return 0;
}

Erro strVar_imprime(const StringVar *sv) {
   printf("%s ", sv->str);
   return 0;
}

Erro strVar_imprimeV(const StringVar *sv) {
   printf("%s\n", sv->str);
   return 0;
}

Erro strVar_pegaTam(const StringVar *sv, size_t *tamStr) {
   *tamStr = sv->strTam;
   return 0;
} 

/* Funções para usar como parametros nas estruturas. */

size_t strVar_dadoTam() { 
   return sizeof(StringVar);
}

void strVar_dadoCopia(void *dest, const void *orig) {
   memcpy(dest, orig, sizeof(StringVar));
   ((StringVar *) dest)->str = malloc(strlen(((StringVar *) orig)->str) + 1);
   strcpy(((StringVar *) dest)->str, ((StringVar *) orig)->str);
   return;
}

int strVar_dadoCompara(const void *dadoUm, const void *dadoDois) {
   return strcmp(((StringVar *) dadoUm)->str, ((StringVar *) dadoDois)->str);
}

int strVar_dadoComparaVet(const void *dadoUm, const void *dadoDois) {
   return strcmp((*(StringVar **) dadoUm)->str, (*(StringVar **) dadoDois)->str);
}

void strVar_dadoLibera(void *dado) {
   free(((StringVar *) dado)->str);
   return;
}

void strVar_dadoImprime(void *dado) {
   printf("%s ", ((StringVar *) dado)->str);
   return;
}

void strVar_dadoImprimeV(void *dado) {
   printf("%s\n", ((StringVar *) dado)->str);
   return;
}

