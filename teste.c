#include <stdio.h>
#include "grafo.h"

//------------------------------------------------------------------------------

int main(int argc, char **argv) {
  grafo g;
  vertice v;
  double coef;

  if (argc == 2) {
    g = le_grafo(argv[1]);
    v = le_vertice();

    if ( !g )
      return 1;

    coef = coeficiente_proximidade(g,v);

    printf ("O coeficiente de proximidade é %f\n", coef);

    return ! destroi_grafo(g);
  }

	printf("Como executar: ./teste <nome_do_arquivo>\n");
  return (-1);
}
