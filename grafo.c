#define  _GNU_SOURCE
#include <stdio.h>
#include <stdlib.h>
#include <sys/types.h>
#include <string.h>
#include "grafo.h"

//------------------------------------------------------------------------------
// (apontador para) estrutura de dados para representar um grafo
// 
// o grafo tem um nome, que é uma "string"

typedef struct grafo *grafo;

//------------------------------------------------------------------------------
// (apontador para) estrutura de dados para representar um vértice
// 
// o vértice tem um nome, que é uma "string"

typedef struct vertice *vertice;

//------------------------------------------------------------------------------
// desaloca toda a memória usada em *g
// 
// devolve 1 em caso de sucesso,
//         ou 
//         0, caso contrário

int destroi_grafo(grafo g) {

  return g == NULL;
}


//------------------------------------------------------------------------------
// lê um grafo
// 
// devolve o grafo lido,
//         ou 
//         NULL, em caso de erro 

grafo le_grafo(FILE *input) {
  char * line = NULL;
  char line_aux[3];
  size_t len = 0;
  ssize_t read;
  int lines = 0;

  if (input == NULL)
      exit(EXIT_FAILURE);

  while ((read = getline(&line, &len, input)) != EOF) {
    if (!(read == 1 && line[0] == '\n')){
      removeSpaces(line);
      strcpy(line_aux, line);
      printf("%s\n", line_aux);
      lines++;
      /*
      TODO: para cada linha, ver se o vertice ta no grafo, se tiver, adiciona na lista de arestas 
      se nao tiver, cria vertice */
    }
  }

  fclose(input);
  if (line)
      free(line);

  printf("\n\n%d\n", lines);

  return (grafo)input;
}


//------------------------------------------------------------------------------
// lê um vertice 

vertice le_vertice(void) {
  return (vertice) NULL;
}




//------------------------------------------------------------------------------
// devolve o coeficiente de proximidade do vértice v de g
// 

double coeficiente_proximidade(grafo g, vertice v) {
	
  v = NULL;
  g = NULL;

  return !(v == (vertice) g);
}

//------------------------------------------------------------------------------
// remove espaços brancos de uma string
void removeSpaces(char *str)
{
    int count = 0;
    for (int i = 0; str[i]; i++)
        if ( (str[i] != '\0') && (str[i] != '\n') && (str[i] != '\t') && (str[i] != ' ') ) 
            str[count++] = str[i]; 
            
    str[count] = '\0';
}

