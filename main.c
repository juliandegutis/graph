#include <stdio.h>
#include <stdlib.h>

/* run this program using the console pauser or add your own getch, system("pause") or input loop */

struct Graph {
	int nVertices;
	int edges;
	struct Vertice *vertice; 
};

struct Vertice {
	int number;
	struct Vertice *prox;
	struct AdjList *adjList;
}; 

struct AdjList {
	struct AdjNode *node;
}; 

struct AdjNode {
	int number;
	struct AdjNode *prox;
};

struct Graph* createEmpty() {
	
	struct Vertice* createVertice();
	
	struct Graph *graph;
	
	graph = (struct Graph*) malloc( sizeof( struct Graph ) );
	graph->nVertices = 1;
	graph->edges = 0;	
	struct Vertice* v = createVertice(1);
	graph->vertice = v;
	
	return graph;
	
}

struct Vertice* createVertice( int value ) {
	struct Vertice* v;
	v = (struct Vertice*) malloc( sizeof( struct Vertice ) );
	v->number = value;
	v->prox = NULL;
	
	struct AdjList *adjList;
	adjList = (struct AdjList *) malloc( sizeof( struct AdjList ) );
	
	v->adjList = adjList;
	
	return v;

}

void addVertice( struct Graph* g, int value ) {
	
	struct Vertice* createVertice();
	struct Vertice* v = createVertice( value );
	
	v->prox = g->vertice;
	g->vertice = v;
	
	g->nVertices = g->nVertices + 1;
	
}

void print( struct Graph* g ) {
	printf( "%d\n", g->nVertices);
	
	struct Vertice* aux;
	aux = g->vertice;
	while( aux != NULL ) {
		printf( "%d\n", aux->number );
		aux = aux->prox;
	}
}

int main() {
	
	struct Graph* g = createEmpty();
	void addVertice( struct Graph* g, int value );
	
	addVertice( g, 2 );
	
	print( g );
	
	
	
	printf( "\n\n\n\nOk" );
	getch();
	
}
