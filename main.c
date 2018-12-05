#include <stdio.h>
#include <stdlib.h>
#include <string.h>

/**
	Global variables
*/
struct Graph* graph;
struct Graph* company;

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
	int type - Type of the Vertice in a context: 1 - Employee / 2 - Task
*/
struct Vertice {
	int number;
	int color;
	int type;
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

/**
	Functions scopes
*/
void addEdge( struct Graph* g, int v1, int v2 );
void addVertice( struct Graph* g, int value, int type );
int validate( int identifier, int type );
int isBipartite( struct Graph* g, int v, int* color, int* done, int currentColor );
struct Vertice* createVertice( int value, int type );
void verifyGraph();
void addRelationship( struct Graph* g, int v1, int t1, int v2, int t2 );
int bipartiteRestriction( int employeeId, int taskId );
void checkAttributions();

/*
	Instance method to create an empty Graph with only one vertice and no edges.
*/
struct Graph* createEmpty() {
		
	struct Graph *graph;
	
	graph = (struct Graph*) malloc( sizeof( struct Graph ) );
	graph->nVertices = 1;
	graph->edges = 0;	
	struct Vertice* v = createVertice(0, 1);
	graph->vertice = v;
	
	return graph;
	
}

/*
	Instance method that creates an individual Vertice with the respective adjascent linked list of the Vertice v
	Param: int value - Unique identifier of the Vertice on a Graph
*/
struct Vertice* createVertice( int value, int type ) {
	struct Vertice* v;
	v = (struct Vertice*) malloc( sizeof( struct Vertice ) );
	v->number = value;
	v->prox = NULL;
	v->type = type;
	
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

/**
	Add a relationship between an employee and a task
*/
void addRelationship( struct Graph* g, int v1, int t1, int v2, int t2 ) {
	
	struct Vertice* vAux;
	
	vAux = g->vertice;
	
	while( vAux != NULL ) {
		
		/**
			Gets the Vertice (n1,t1) to add an edge to Vertice (n2,t2)
			Does the same thing to Vertice (n2,t2) and add an edge to Vertice (n1,t1)
		*/
		if( vAux->number == v1 && vAux->type == t1 ) {
					
			struct AdjList* adjList = vAux->adjList;
			
			struct AdjNode* adjNode = (struct AdjNode*)malloc(sizeof(struct AdjNode));
			adjNode->number = v2;
			adjNode->prox = vAux->adjList->node;
			vAux->adjList->node = adjNode;
			adjList = NULL;
			
		} else if( vAux->number == v2 && vAux->type == t2 ) {
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
void addVertice( struct Graph* g, int value, int type ) {
	
	printf( "Adding %d\n", value);
	struct Vertice* v = createVertice( value, type );	
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

/**
	Loads a graph from a text file, then verifies if that Graph has a bipartite condition
*/
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
    	addVertice( graph, a, 1 );
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
			printf( "\n\nResultado: O grafico nao e bipartido");
		} else {
			printf( "\n\nResultado: O grafico e bipartido");
		}
	
	}
  	
}

/**
	Validate the unique identifier compose by number plus type from all Vertices of a Graph
	Return -1 if the Vertice (n,t) is already in the Graph, otherwise return 1
*/
int validate( int identifier, int type ) {
	
	if( identifier == 0 ) {
		return -1;
	}
	
	struct Vertice* v = company->vertice;
	while( v != NULL ) {
		if( v->number == identifier && v->type == type ) {
			return -1;
		}
		v = v->prox;
	}
	
	return 1;
	
}

/**
	Checks if a Vertice (n,t) is present in the current Graph representation
	Returns 1 if a Vertice (n,t) is present in the Graph, otherwise return -1
*/
int isPresent( int identifier, int type ) {
	if( identifier == 0 ) {
		return 1;
	}
	
	struct Vertice* v = company->vertice;
	while( v != NULL ) {
		if( v->number == identifier && v->type == type ) {
			return 1;
		}
		v = v->prox;
	}
	
	return -1;
}

/**
	Checks if the Employee and a Task can be related to not broke the bipartite condition from the Graph
*/
int bipartiteRestriction( int employeeId, int taskId ) {
	
	int typeEmployee;
	int typeTask;
	
	struct Vertice* v = company->vertice;
	while( v != NULL ) {
		if( v->number == employeeId && v->type == 1 ) {
			typeEmployee = v->type;
		}
		if( v->number == taskId && v->type == 2 ) {
			typeTask = v->type;
		}
		v = v->prox;
	}
	
	if( typeEmployee == typeTask ) {
		return -1;
	} else {
		return 1;
	}
}

/**
	Function to check the quantity of tasks that are assigned to an Employee
	The quantity of tasks is equal to the quantity of nodes in the respective Vertice adjascent list
*/
void checkAttributions() {
	
	struct Vertice* v = company->vertice;
	
	while( v != NULL ){
		if( v->type == 1 ) {
			struct AdjNode* node = v->adjList->node;
			int qnt = 0;
			if( node != NULL ) {
				while( node != NULL ) {
					qnt++;
					node = node->prox;
				}
			}
			printf( "\nA quantidade de tarefas alocadas para o funcionario %d, e de %d", v->number, qnt);
		}
		v = v->prox;
	}
	
}


/**
	IO FUNCIONS BLOCK START
*/

/*
	Print the full graph representation
*/
void print( struct Graph* g ) {

	printf( "\n\n");
	
	struct Vertice* aux;
	aux = g->vertice;
	while( aux != NULL ) {
		printf( "\nVertice %d | type %d ", aux->number, aux->type );
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

/**
	IO function to add a Employee ( Vertice (n,1) ) in the Graph representation
*/
void addEmployee() {
	
	int employeeId;
	
	system("cls");
	printf(" \n\n");
	printf( "**********************************************\n");
	printf( "*********** ADICIONAR FUNCIONARIO  ***********\n");
	printf( "**********************************************\n");
	printf("\n\n");
	printf( "Digite o identificador do usuario\n");
	scanf( "%i", &employeeId );
	
	int valid = validate( employeeId, 1 );
	if( valid == 1 ) {
		addVertice( company, employeeId, 1 );
	} else {
		printf("\n\nNao foi possivel inserir o funcionario, o funcionario ja esta cadastrado na empresa" );
	}
	
}

/**
	IO function to add a Task ( Vertice (n, 2) ) in the Graph representation
*/
void addTask() {
	
	int taskId;
	
	system("cls");
	printf(" \n\n");
	printf( "**********************************************\n");
	printf( "************* ADICIONAR TAREFA  **************\n");
	printf( "**********************************************\n");
	printf("\n\n");
	printf( "Digite o identificador da tarefa\n");
	scanf( "%i", &taskId);
	
	int valid = validate( taskId, 2 );
	if( valid == 1 ) {
		addVertice( company, taskId, 2 );
	} else {
		printf( "\n\nNao foi possivel inserir a tarefa, a tarefa ja esta cadastrada na empresa" );
	}
	
}

/**
	IO function to assign one Task to one Employee
	The task will be only added, if it does not broke the bipartite condition of the Graph
*/
void assignTask() {
	
	int employeeId;
	int taskId;
	int valid;
	
	system("cls");
	printf(" \n\n");
	printf( "**********************************************\n");
	printf( "*********** ADICIONAR FUNCIONARIO  ***********\n");
	printf( "**********************************************\n");
	printf("\n\n");
	printf( "Digite o identificador do usuario\n");
	scanf( "%i", &employeeId );
	printf( "\nDigite o identificador da tarefa" );
	scanf( "%i", &taskId );
	
	valid = isPresent( employeeId, 1 );
	if( valid == -1 ) {
		printf("\n\nO funcionario informado nao pertence a empresa." );
	} else {
		valid = isPresent( taskId, 2 );
		if( valid == -1 ) {
			printf("\n\nA tarefa informada nao pertence a empresa." );
		} else {
			int valid = bipartiteRestriction(employeeId, taskId);
			if( valid == 1 ) {
				addRelationship(company, employeeId, 1, taskId, 2 );
			}
		}
	}
	
}

/**
	IO FUNCTIONS BLOCK END
*/

/**
	IO main function
*/
int main() {
	
	int ctrl = -1;
	int op;
	
	graph = createEmpty();
	company = createEmpty();
	
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
		printf("4 - Atribuir Tarefa\n" );
		printf("5 - Imprimir Grafo\n");
		printf("6 - Imprimir Empresa\n");
		printf("7 - Validar Atribuicoes\n" );
		printf("9 - Sair\n");
		
		scanf( "%i", &ctrl );
		
		if( ctrl == 1 ) {
			verifyGraph();
		} else if( ctrl == 2 ) {
			addEmployee();
		} else if( ctrl == 3 ) {
			addTask();
		} else if( ctrl == 4 ) {
			assignTask();
		} else if( ctrl == 5 ) {
			print( graph );
		} else if( ctrl == 6 ) {
			print( company );
		} else if( ctrl == 7 ) {
			checkAttributions();
		}

	} while( ctrl != 9 );
	
	printf( "\n\n\n\nOk" );
	getch();
	
}
