#include <stdio.h>
#include <stdlib.h>

/* run this program using the console pauser or add your own getch, system("pause") or input loop */

/*
 A graph structure compose by
 int nVertices - A number of vertices
 int edges - A number of edges
 Vertice* vertice - A linked list that references all the vertices
*/
struct Graph {
	int nVertices;
	int edges;
	struct Vertice *vertice; 
};

/*
	A linked list structure that references all vertices from a Graph g
	number - Unique identifier of a vertice in a Graph
	Vertice* prox - References another Vertice to compose a Graph
	AdjList* adjList - Adjascent List structure from a Vertice 
*/
struct Vertice {
	int number;
	struct Vertice *prox;
	struct AdjList *adjList;
}; 

/*
	Adjascent linked list structure with a list of nodes adjascents from a Vertice
	AdjNode* node - Node referencing an vertice of Graph
*/
struct AdjList {
	struct AdjNode *node;
}; 

/*
	Adjascent vertice structure, linked list of all adjascent nodes from a Vertice v
	int number - Unique identifier of a vertice
*/
struct AdjNode {
	int number;
	struct AdjNode *prox;
};

/*
	Instance method to create an empty Graph with only one vertice and no edges.
*/
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

/*
	Instance method that creates an individual Vertice with the respective adjascent linked list of the Vertice v
	Param: int value - Unique identifier of the Vertice on a Graph
*/
struct Vertice* createVertice( int value ) {
	struct Vertice* v;
	v = (struct Vertice*) malloc( sizeof( struct Vertice ) );
	v->number = value;
	v->prox = NULL;
	
	struct AdjList *adjList;
	adjList = (struct AdjList *) malloc( sizeof( struct AdjList ) );
	
	v->adjList = adjList;
	v->adjList->node = NULL;
	
	return v;

}

/*
	Add a relationship between two vertices from a Graph
	Params: struct Graph* g - Reference to a graph that will be modified
			int v1 - Unique identifier of one Vertice
			int v2 - Unique identifier of one Vertice
*/
void addEdge( struct Graph* g, int v1, int v2 ) {
	
	struct Vertice* vAux;
	
	vAux = g->vertice;
	
	while( vAux != NULL ) {
		
		if( vAux->number == v1 ) {
					
			struct AdjList* adjList = vAux->adjList;
			
			struct AdjNode* adjNode = (struct AdjNode*)malloc(sizeof(struct AdjNode));
			adjNode->number = v2;
			adjNode->prox = vAux->adjList->node;
			vAux->adjList->node = adjNode;
			adjList = NULL;
			
		} else if( vAux->number == v2 ) {
			struct AdjList* adjList = vAux->adjList;
			
			struct AdjNode* adjNode = (struct AdjNode*)malloc(sizeof(struct AdjNode));
			adjNode->number = v1;
			adjNode->prox = vAux->adjList->node;
			vAux->adjList->node = adjNode;
			adjList = NULL;
		}
		
		vAux = vAux->prox;
	}
	
	
}

/*
	Add a vertice in a Graph
	Params: int value - Unique identifier of the Vertice
			struct Graph* g - Reference to a graph that will be modified
*/
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
		printf( "\nVertice %d   ", aux->number );
		struct AdjList* adjList = aux->adjList;
		
		struct AdjNode* node = adjList->node;
		printf( "Lista Adj -> ");
		while( node != NULL ) {
			printf( "%d -> ", node->number );
			node = node->prox;
		}
		
		aux = aux->prox;
	}
}

int main() {
	
	void addEdge( struct Graph* g, int v1, int v2 );
	void addVertice( struct Graph* g, int value );
	
	struct Graph* g = createEmpty();
	
	addVertice( g, 2 );
	addVertice( g, 3 );
	addVertice( g, 4 );
	addEdge( g, 1, 2 );
	addEdge( g, 1, 3 );
	
	print( g );
	
	
	
	printf( "\n\n\n\nOk" );
	getch();
	
}
