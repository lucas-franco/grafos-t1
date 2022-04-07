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

grafo le_grafo(char * filename) {
  char fileToArray[1024][1024];
  char * line = NULL;
  char line_aux[3];
  size_t len = 0;
  ssize_t read;
  int lines = 0;
  int whitespaces = 0;

  FILE *input = fopen(filename, "r");
  if (input == NULL)
      exit(EXIT_FAILURE);

  while ((read = getline(&line, &len, input)) != EOF) {
    if (!(read == 1 && line[0] == '\n')){
      // whitespaces = removeSpaces(line);
      separateString(line);
      strcpy(line_aux, line);
      lines++;
      // printf("%s whitespaces: %d\n", line, whitespaces);
      /*
      TODO: para cada linha, ver se o vertice ta no grafo, se tiver, adiciona na lista de arestas 
      se nao tiver, cria vertice */
    }
  }

  printf("\n\nLINHAS: %d\n", lines);

  fclose(input);
  if (line)
      free(line);

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
int removeSpaces(char *str)
{
    int count = 0;
    int whitespaces = 0;
    for (int i = 0; str[i]; i++)
        if ( (str[i] != '\0') && (str[i] != '\n') && (str[i] != '\t') && (str[i] != ' ') ) 
          str[count++] = str[i];
        else
          whitespaces++;
    str[count] = '\0';
    return whitespaces;
}

int separateString(char *str){
  char ** res  = NULL;
  char delimit[]=" \t\r\n\v\f";
  char *  p    = strtok (str, delimit);
  int n_spaces = 0, i;

  /* split string and append tokens to 'res' */
  while (p) {
    res = realloc (res, sizeof (char*) * ++n_spaces);

    if (res == NULL)
      exit (-1); /* memory allocation failed */

    res[n_spaces-1] = p;

    p = strtok (NULL, delimit);
  }

  /* realloc one extra element for the last NULL */

  // res = realloc (res, sizeof (char*) * (n_spaces+1));
  // res[n_spaces] = 0;

  /* print the result */

  for (i = 0; i < (n_spaces); ++i)
    printf ("res[%d] = %s\n", i, res[i]);

  /* free the memory allocated */

  free (res);
  return res;
  }