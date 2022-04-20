#define _GNU_SOURCE
#include <stdio.h>
#include <stdlib.h>
#include <sys/types.h>
#include <string.h>
#include "grafo.h"

#define LINE_SIZE 1024 // Tamanho máximo de string

unsigned long int ultimo_id_vertice = 0; // O id do último vértice

//------------------------------------------------------------------------------
// (apontador para) estrutura de dados para representar um grafo
//
// o grafo tem um nome, que é uma "string"

struct grafo
{
	char *nome;
	unsigned long int num_vertices;
	// char padding[4];
	struct node *vertices;
};

//------------------------------------------------------------------------------
// (apontador para) estrutura de dados para representar um vértice
//
// o vértice tem um nome, que é uma "string"

struct vertice
{
	char *nome;
	unsigned long int id;
	unsigned long int grau;
	unsigned long int distancia;
	unsigned long int estado;
	struct node *vizinhos;
};

typedef struct node
{
	struct vertice *vertice;
	struct node *prox;
	struct node *pai;
} node;

struct queue
{
	unsigned long int front, rear;
	unsigned long int size;
	unsigned long int capacity;
	vertice *components;
};

/****************************** Funções adicionais *********************************/
// Grafo
grafo cria_grafo(void);
vertice cria_vertice(char *nome);
int adiciona_vertice(grafo g, vertice v);
int aresta_existe(vertice u, vertice v);
vertice vertice_existe(grafo g, char *nome_vertice);
int cria_aresta(vertice v, vertice u);
int conecta_vertices(vertice v, vertice u);
void distancia_de_v(grafo g, vertice v);

// Fila
struct queue *criaFila(unsigned long int capacity);
int isFull(struct queue *queue);
int isEmpty(struct queue *queue);
void enfileira(struct queue *queue, vertice item);
vertice desemfileira(struct queue *queue);
vertice rear(struct queue *queue);
vertice front(struct queue *queue);

/************ Grafo ***************/
grafo cria_grafo(void)
{
	grafo g = (grafo)malloc(sizeof(struct grafo));
	g->num_vertices = 0;

	return g;
}

vertice cria_vertice(char *nome)
{
	vertice v = malloc(sizeof(struct vertice));

	v->nome = malloc(strlen(nome) + 1);
	strcpy(v->nome, nome);

	v->id = ultimo_id_vertice;
	v->distancia = 0;
	v->grau = 0;
	v->estado = 0;
	v->vizinhos = malloc(sizeof(struct node));

	ultimo_id_vertice += 1;
	return v;
}

int adiciona_vertice(grafo g, vertice v)
{
	struct node *ultimo_vertice = NULL;

	if (g->num_vertices == 0)
	{
		g->vertices = malloc(sizeof(struct node));

		ultimo_vertice = g->vertices;
	}
	else
	{
		// busca o último vertice
		ultimo_vertice = g->vertices;
		while (ultimo_vertice->prox != NULL)
			ultimo_vertice = ultimo_vertice->prox;

		ultimo_vertice->prox = malloc(sizeof(struct node));
		ultimo_vertice = ultimo_vertice->prox;
	}

	ultimo_vertice->vertice = v;
	ultimo_vertice->prox = NULL;
	g->num_vertices++;

	return 0;
}

int aresta_existe(vertice u, vertice v)
{
	if (u->grau == 0 || v->grau == 0)
		return 0;

	struct node *vertice_atual = v->vizinhos;

	do
	{
		if (vertice_atual->vertice == u)
			return 1;
		vertice_atual = vertice_atual->prox;
	} while (vertice_atual != NULL);

	return 0;
}

vertice vertice_existe(grafo g, char *nome_vertice)
{
	if (g->num_vertices == 0)
		return NULL;

	struct node *vertice_atual = g->vertices;

	if (g->vertices == NULL)
		return NULL;

	do
	{	
		if (strcmp(vertice_atual->vertice->nome, nome_vertice) == 0)
		{
			return vertice_atual->vertice;
		}
		vertice_atual = vertice_atual->prox;
	} while (vertice_atual != NULL);

	return NULL;
}

int cria_aresta(vertice u, vertice v)
{
	if (aresta_existe(u, v))
		return 0;

	conecta_vertices(u, v);
	conecta_vertices(v, u);

	return 1;
}

int conecta_vertices(vertice v, vertice u)
{
	struct node *vizinho_u = malloc(sizeof(struct node));
	if (u->grau > 0)
	{
		struct node *primeiro_vizinho_u = u->vizinhos;
		vizinho_u->prox = primeiro_vizinho_u;
	}
	else
	{
		vizinho_u->prox = NULL;
	}

	vizinho_u->vertice = v;

	u->vizinhos = vizinho_u;

	u->grau += 1;

	return 0;
}


/************ Fila ***************/
struct queue *criaFila(unsigned long int capacity)
{
	struct queue *queue = (struct queue *)malloc(sizeof(struct queue));
	queue->capacity = capacity;
	queue->front = queue->size = 0;
	queue->rear = capacity - 1;
	queue->components = (vertice *)malloc(queue->capacity * sizeof(vertice));
	return queue;
}

int isFull(struct queue *queue)
{
	return (queue->size == queue->capacity);
}

int isEmpty(struct queue *queue)
{
	return (queue->size == 0);
}

void enfileira(struct queue *queue, vertice item)
{
	if (isFull(queue))
		return;

	queue->rear = (queue->rear + 1) % queue->capacity;
	queue->components[queue->rear] = item;
	queue->size = queue->size + 1;
	return;
}

vertice desemfileira(struct queue *queue)
{
	if (isEmpty(queue))
		return NULL;

	vertice item = queue->components[queue->front];
	queue->front = (queue->front + 1) % queue->capacity;
	queue->size = queue->size - 1;
	return item;
}

vertice rear(struct queue *queue)
{
	if (isEmpty(queue))
		return NULL;
	return queue->components[queue->rear];
}

vertice front(struct queue *queue)
{
	if (isEmpty(queue))
		return NULL;
	return queue->components[queue->front];
}

/****************************** Funções especifícadas *********************************/

//------------------------------------------------------------------------------
// desaloca toda a memória usada em *g
//
// devolve 1 em caso de sucesso,
//         ou
//         0, caso contrário

int destroi_grafo(grafo g)
{
	free(g->vertices);
	free(g);
	return g == NULL;
}

//------------------------------------------------------------------------------
// lê um grafo
//
// devolve o grafo lido,
//         ou
//         NULL, em caso de erro

grafo le_grafo(FILE *input)
{
	grafo g = cria_grafo();

	char *line = NULL;
	size_t len = 0;
	ssize_t read;

	int linesCount = 0;

	char delimit[] = " \t\r\n\v\f\0";

	char *u_nome;
	char *v_nome;

	vertice u;
	vertice v;

	long unsigned int i = 0;

	if (input == NULL)
		exit(EXIT_FAILURE);

	while ((read = getline(&line, &len, input)) != EOF)
	{
		if (!(read == 1 && line[0] == '\n'))
		{

			// Fix para não ler espaço em branco
			i = strlen(line) - 1;
			if (line[i] == '\n')
				line[i] = '\0';

			char *separatedString = strtok(line, delimit);
			linesCount++;

			int tokens_read = 0;
			while (separatedString != NULL && separatedString[0] != '\0')
			{
				if (tokens_read == 0)
				{
					u_nome = separatedString;
					u = vertice_existe(g, u_nome);
					if (u == NULL)
					{
						u = cria_vertice(u_nome);
						adiciona_vertice(g, u);
					}
				}
				else if (tokens_read == 1)
				{
					v_nome = separatedString;
					v = vertice_existe(g, v_nome);

					if (v == NULL)
					{
						v = cria_vertice(v_nome);
						adiciona_vertice(g, v);
					}

					cria_aresta(u, v);
				}
				separatedString = strtok(NULL, " ");
				tokens_read++;
			}
		}
	}

	fclose(input);
	return g;
}

//------------------------------------------------------------------------------
// lê um vertice

vertice le_vertice(void)
{
	char nome[LINE_SIZE];
	printf("Digite o nome do vértice v: ");
	scanf("%s", nome);
	vertice v = cria_vertice(nome);
	return v;
}

//------------------------------------------------------------------------------
// devolve o coeficiente de proximidade do vértice v de g
//

double coeficiente_proximidade(grafo g, vertice v)
{
	vertice v_de_g = vertice_existe(g, v->nome);
	if (v_de_g == NULL)
	{
		printf("Vértice %s não existe no grafo.\n", v->nome);
		return 0;
	}

	distancia_de_v(g, v_de_g);
	unsigned long int soma_de_distancia = 0;
	unsigned long int n = g->num_vertices;

	struct node *vertice_atual = g->vertices;
	do
	{
		soma_de_distancia += vertice_atual->vertice->distancia;
		vertice_atual = vertice_atual->prox;
	} while (vertice_atual != NULL);

	double coeficiente = 0;
	if (soma_de_distancia >= n - 1 && n > 1) {
		coeficiente = (double)n / (double)soma_de_distancia;
	} else if (n == 1) {
		printf("Grafo trivial.\n");
	} else {
		printf("Grafo não é conexo.\n");
	}
	return coeficiente;
}

void distancia_de_v(grafo g, vertice v)
{
	struct queue *fila = criaFila(g->num_vertices);
	
	g->vertices->pai = NULL;
	v->distancia = 0;
	v->estado = 1;
	
	vertice v1 = v;
	enfileira(fila, v1);

	// enquanto a fila nao estiver vazia
	while (!isEmpty(fila))
	{
		v1 = desemfileira(fila);
		node *vertice_atual = v1->vizinhos;
		while (vertice_atual != NULL && vertice_atual->vertice != NULL)
		{
			if (vertice_atual->vertice->estado == 1){
				// Faz nada
			}
			else if (vertice_atual->vertice->estado == 0){
				vertice_atual->vertice->distancia = v1->distancia + 1;

				if (vertice_atual->pai == NULL) {
					vertice_atual->pai = malloc(sizeof(struct node));
				}

				vertice_atual->pai->vertice = v1;				
				enfileira(fila, vertice_atual->vertice);
				vertice_atual->vertice->estado = 1;

			}
			vertice_atual = vertice_atual->prox;

		}
		v1->estado = 2;
	}
	return;
}