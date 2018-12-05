#include <stdio.h>
#include <stdlib.h>
#include <string.h>

struct Graph* graph;

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
	int color;
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

void addEdge( struct Graph* g, int v1, int v2 );
void addVertice( struct Graph* g, int value );
int isBipartite( struct Graph* g, int v, int* color, int* done, int currentColor );
struct Vertice* createVertice();
void verifyGraph();

/*
	Instance method to create an empty Graph with only one vertice and no edges.
*/
struct Graph* createEmpty() {
		
	struct Graph *graph;
	
	graph = (struct Graph*) malloc( sizeof( struct Graph ) );
	graph->nVertices = 1;
	graph->edges = 0;	
	struct Vertice* v = createVertice(0);
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
	
	printf( "Adding %d\n", value);
	struct Vertice* v = createVertice( value );	
	v->color = -1;
	v->prox = g->vertice;
	g->vertice = v;
	g->nVertices = g->nVertices + 1;
	
}

/*
	Recursevly check if a Graph is bipartite, based on current vertice
*/
int isBipartite( struct Graph* g, int v, int* color, int* done, int currentColor ) {
	
	struct Vertice* aux = g->vertice;	
	printf( "\nStarting vertice: %d", v );

	/*
		Iterates the Graph checking if the current Vertice is equal to an vertice
	*/
	while( aux != NULL ) {
		
		/*
			Finds the needed Vertice
		*/
		if( aux->number == v) {
			
			/*
				Change the color of bipartite validation algorithm
			*/
			if( currentColor == 0 ) {
				currentColor++;
			} else {
				currentColor--;
			}
			
			/*
				Marks the current vertice as done
			*/
			done[v] = 1;
			
			struct AdjList* adjListAux = aux->adjList;
			struct AdjNode* node = adjListAux->node;
			while( node != NULL ) {
				/*
					Checks if any adjascente node/vertice from the iterate vertice, have the same color than it
					Rule: Return -1 if both vertices shares the same color.
				*/
				if( color[node->number] == color[v]) {
					return -1;
				}		
				
				/*
					Fill the current node/vertice with the current color from iteration
				*/		
				color[node->number] = currentColor;
				
				/*
					If the node value is not on done list, do isBipartite to a node/vertice identifier recursevly
				*/
				if( done[node->number] == -1 ) {
					return isBipartite( g, node->number, color, done, currentColor );
				}
				
				node = node->prox;
			}
		}
		
		aux = aux->prox;
		
	}
	
	return 1;
	
}

/*
	Print the full graph representation
*/
void print( struct Graph* g ) {

	printf( "\n\n");
	
	struct Vertice* aux;
	aux = g->vertice;
	while( aux != NULL ) {
		printf( "\nVertice %d ", aux->number );
		struct AdjList* adjList = aux->adjList;
		
		struct AdjNode* node = adjList->node;
		printf( "Lista Adj ");
		while( node != NULL ) {
			printf( "->" );
			printf( " %d", node->number );
			node = node->prox;
		}
		
		aux = aux->prox;
	}
}

void verifyGraph() {
    
	/*
		Load file block start
	*/
	char currentline[1000];
    char *pt;
    int pos[1000];
    FILE *file;

    if ((file = fopen("graph.txt", "r")) == NULL)
    {
        printf("Error! opening file");
        // Program exits if file pointer returns NULL.
        exit(1);         
    }

	fgets(currentline, sizeof(currentline), file);
	fprintf(stderr, "got line: %s\n", currentline);
	pt = strtok(currentline,",");
	int i = 0;
	while (pt != NULL) {
    	int a = atoi(pt);
    	addVertice( graph, a );
    	pt = strtok(NULL, ",");
	}

	while (fgets(currentline, sizeof(currentline), file) != NULL) {
		pt = strtok (currentline,",");
    	i = 0;
		while (pt != NULL) {
        	int a = atoi(pt);
        	pos[i++] = a;
        	pt = strtok (NULL, ",");
    	}
    	addEdge(graph, (int)pos[0], (int)pos[1]);	
	}
	
  	fclose(file);
  	
  	print(graph);
  	/*
  		Load file end block
  	*/
  	
  	/*
	  Start iteration to bipartite verification 
  	*/
  	int color[graph->nVertices];
	int done[graph->nVertices];
	i = 0;
	

	for( i = 0 ; i < graph->nVertices ; i++ ) {
		color[i] = -1;
	}
	
	for( i = 0 ; i < graph->nVertices ; i++ ) {
		done[i] = -1;
	}

	int currentColor = 0;
	color[graph->vertice->number] = currentColor;
	if( graph->vertice->prox != NULL ) {
		int result = isBipartite( graph, graph->vertice->number, color, done, currentColor );
		if( result == -1 ) {
			printf( "\n\nResultado: O grafico nao e bipartido", result);
		} else {
			printf( "\n\nResultado: O grafico e bipartido", result);
		}
	
	}
  	
}

int main() {
	
	int ctrl = -1;
	int op;
	
	do {
		
		printf(" \n\n");
		printf( "**********************************************\n");
		printf( "*****************   GRAFOS  ******************\n");
		printf( "**********************************************\n");
		printf("\n\n");
		printf("Menu: \n");
		printf("1 - Verificar Grafo\n");
		printf("2 - Adicionar Funcionario\n" );
		printf("3 - Adicionar Tarefa\n" );
		printf("4 - Imprimir Grafo\n");
		printf("5 - Sair\n");
		
		scanf( "%i", &ctrl);
		
		if( ctrl == 1 ) {
			graph = createEmpty();
			verifyGraph();
		} else if( ctrl == 2 ) {
			
		} else if( ctrl == 3 ) {
		
		} else if( ctrl == 4 ) {
			print( graph );
		}

	} while( ctrl != 5 );

	/*
	graph = createEmpty();
	
	addVertice( graph, 1 );
	addVertice( graph, 2 );
	addVertice( graph, 3 );
	addVertice( graph, 4 );
	addVertice( graph, 5 );
	addVertice( graph, 6 );
	addVertice( graph, 7 );
	addVertice( graph, 8 );
	addEdge( graph, 0, 1 );
	addEdge( graph, 0, 2 );
	addEdge( graph, 2, 3 );
	addEdge( graph, 3, 1 );
	addEdge( graph, 0, 3 );
	
	print( graph );

	int color[graph->nVertices];
	int done[graph->nVertices];
	int i = 0;
	

	for( i = 0 ; i < graph->nVertices ; i++ ) {
		color[i] = -1;
	}
	
	for( i = 0 ; i < graph->nVertices ; i++ ) {
		done[i] = -1;
	}

	int currentColor = 0;
	color[graph->vertice->number] = currentColor;
	if( graph->vertice->prox != NULL ) {
		int result = isBipartite( graph, graph->vertice->number, color, done, currentColor );
		printf( "\n\nResultado: %d", result);
	}
	*/
	
	printf( "\n\n\n\nOk" );
	getch();
	
}
