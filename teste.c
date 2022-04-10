#include <stdio.h>
#include "grafo.h"

//------------------------------------------------------------------------------

int main(int argc, char **argv) {
	grafo g;
	vertice v;
	double coef;
	
	if (argc == 2) {
		FILE *input = fopen(argv[1], "r");
		g = le_grafo(input);
	} else {
		g = le_grafo(stdin);
	}

	v = le_vertice();

	if ( !g )
		return 1;

	coef = coeficiente_proximidade(g,v);

	printf ("O coeficiente de proximidade é %f\n", coef);

	return !destroi_grafo(g);
}
