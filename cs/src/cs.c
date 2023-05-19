#include <stdio.h>
#include <stdlib.h>
#include <time.h>

#include <string.h>
#include <unistd.h>

#ifdef _WIN32
#include <windows.h>
#include <tchar.h>
#else
#include <sys/time.h>
#include <sys/resource.h>
#endif

#include <math.h>

#define PAUSE getc(stdin)

#define MAX_VAL 9999
#define MAX_STRING_SIZE 100

#define MAX_NODES 3500
#define MAX_ODS 900
#define MAX_EDGES 3800
#define MAX_EDGES_PER_NODE 14
#define MAX_NODE_PATHS 9000000
#define MAX_PATHS 400000

#define WITH_2_DEGREE_NODES 1

// ----------------- USADOS NO CS
#define MAX_CLT 5 // número máximo de clusters no CS
//#define DBG     // habilita o modo DEBUG (exibe na tela as melhoras na FO)
#define RND       // habilita múltiplas (NUM_EXE) execuções com sementes aleatórias para a instância
//-------------------------------

// ***** Estruturas de Dados para vertices e arestas ******

typedef struct tNode {
	char isOD;
	char ID[MAX_STRING_SIZE];
	short index;
	struct tEdge* edgesNode[MAX_EDGES_PER_NODE];
	char numEdgesNode;
} Node;

typedef struct tEdge {
	char ID[MAX_STRING_SIZE];
	float lenght;
	short index;
	struct tNode *nodeA, *nodeB;
} Edge;

//*** Estrutura de Dados para Guardar os Caminhos

typedef struct tpathTree {
	short nodeID, edgeID;
	struct tpathTree *father, *children[MAX_EDGES_PER_NODE];
	char numChildren;
} PathTree;

//*** Estrutura de Dados com os dados da solucao do programa

typedef struct tSolution {
	char selectedEdges[MAX_EDGES];
	short numSelect;
} Solution;

//************** Dados Grafo ***************

static Edge *edges[MAX_EDGES];
static Node *nodes[MAX_NODES];
static short numEdges;
static short numNodes;

//********* Dados ODs ******************

static Node *targets[MAX_ODS][MAX_ODS];
static short numTargets[MAX_ODS];
static short numTargetsRoot;
static char arePaths[MAX_ODS];

//************ Dados Caminhos  ***********

static PathTree **nodePaths;
static PathTree **paths;
static int numNodePaths;
static int numPaths;

void startAlocations() {
	int i;

	for (i = 0; i < MAX_EDGES; i++)
		edges[i] = (Edge*) malloc(sizeof(Edge));
	for (i = 0; i < MAX_NODES; i++)
		nodes[i] = (Node*) malloc(sizeof(Node));

	paths = (PathTree**) malloc(sizeof(PathTree*) * MAX_PATHS);
	nodePaths = (PathTree**) malloc(sizeof(PathTree*) * MAX_NODE_PATHS);
	for (i = 0; i < MAX_NODE_PATHS; i++)
		nodePaths[i] = (PathTree*) malloc(sizeof(PathTree));
}

double getCpuTime() {
#ifdef _WIN32
	return (double)clock() / CLOCKS_PER_SEC;
#else
	struct timeval tim;
	struct rusage ru;
	getrusage(RUSAGE_SELF, &ru);
	tim = ru.ru_utime;
	double t = (double) tim.tv_sec + (double) tim.tv_usec / 1000000.0;
	tim = ru.ru_stime;
	t += (double) tim.tv_sec + (double) tim.tv_usec / 1000000.0;
	return t;
#endif
}

//*************************  Funcoes de Manipulacao de Grafos ******************************

Node* findOrCreateNode(char *nodeID) {
	int i;
	for (i = 0; i < numNodes; i++)
		if (strcmp(nodeID, nodes[i]->ID) == 0)
			return nodes[i];
	//New Node
	strcpy(nodes[i]->ID, nodeID);
	nodes[i]->numEdgesNode = 0;
	nodes[i]->index = i;
	nodes[i]->isOD = 0;
	numNodes++;

	return nodes[i];
}

Node* other(Edge *ed, Node *nd) {
	if (ed->nodeA != nd)
		return ed->nodeA;
	else
		return ed->nodeB;
}

void copyNode(Node *s, Node *p) {
	int i;
	strcpy(s->ID, p->ID);
	for (i = 0; i < p->numEdgesNode; i++)
		s->edgesNode[i] = p->edgesNode[i];
	s->numEdgesNode = p->numEdgesNode;
	s->index = p->index;
}

void copyEdge(Edge *s, Edge *p) {
	strcpy(s->ID, p->ID);
	s->nodeA = p->nodeA;
	s->nodeB = p->nodeB;
	s->index = p->index;
	s->lenght = p->lenght;
}

// ********************************** Funcoes Para Geracoes de Caminhos ************************

PathTree* getNewNodePath(short _ndIDx, short _edIDx, PathTree *_father) {
	PathTree *nd = nodePaths[numNodePaths];
	nd->nodeID = _ndIDx;
	nd->edgeID = _edIDx;
	nd->father = _father;
	nd->numChildren = 0;
	if (_father != NULL) {
		_father->children[_father->numChildren] = nd;
		_father->numChildren++;
	}
	numNodePaths++;
	if (numNodePaths == MAX_NODE_PATHS) {
		printf("Precisa de mais nos\n");
		exit(0);
	}
	return nd;
}

void foundNewPath(PathTree *ptN) {
	paths[numPaths] = ptN;
	numPaths++;
	if (numPaths == MAX_PATHS) {
		printf("Precisa de mais paths\n");
		exit(0);
	}
	return;
}

void getPaths(Solution *s, short targetID) {
	short edgesCandidates[MAX_EDGES], numEdgesCandidates, numTargToHit =
			numTargets[targetID] - 1;
	char visitedNodes[MAX_NODES];
	PathTree *father;

	if (!arePaths[targetID])
		return;

	PathTree *ptNRoot = getNewNodePath(targets[targetID][0]->index, -1, NULL);

	char isTarget(PathTree *ptN, short targetID) {
		short i;
		for (i = 1; i < numTargets[targetID]; i++)
			if (targets[targetID][i]->index == ptN->nodeID)
				return 1;
		return 0;
	}

	void addEdgeCand(short _edg) {
		edgesCandidates[numEdgesCandidates] = _edg;
		numEdgesCandidates++;
	}

	void cleanEdgeCand() {
		short i;
		Edge *ed;
		for (i = 0; i < numEdgesCandidates; i++) {
			ed = edges[edgesCandidates[i]];
			if (visitedNodes[ed->nodeA->index]
					&& visitedNodes[ed->nodeB->index]) {
				edgesCandidates[i] = edgesCandidates[numEdgesCandidates - 1];
				numEdgesCandidates--;
				i--;
			}
		}
	}

	void findFather(PathTree *ptN, short _ndID) {
		char i;
		if (_ndID == ptN->nodeID) {
			father = ptN;
			return;
		}
		for (i = 0; i < ptN->numChildren; i++)
			findFather(ptN->children[i], _ndID);
		return;
	}

	void fillCandidateList(PathTree *ptN) {
		char resp = 0, i;
		Edge *ed;
		for (i = 0; i < nodes[ptN->nodeID]->numEdgesNode; i++) {
			ed = nodes[ptN->nodeID]->edgesNode[i];
			if (!visitedNodes[other(ed, nodes[ptN->nodeID])->index]
					&& !s->selectedEdges[ed->index])
				addEdgeCand(ed->index);
		}
		for (i = 0; i < ptN->numChildren; i++)
			fillCandidateList(ptN->children[i]);
	}

	short edIdx;
	PathTree *newPtNode;
	Node *newNode;
	for (short i = 0; i < numNodes; i++)
		visitedNodes[i] = 0;
	visitedNodes[ptNRoot->nodeID] = 1;
	do {
		numEdgesCandidates = 0;
		fillCandidateList(ptNRoot);
		if (numEdgesCandidates == 0)
			break;
		do {
			edIdx = edgesCandidates[rand() % numEdgesCandidates];
			newNode
					= (visitedNodes[edges[edIdx]->nodeA->index]) ? edges[edIdx]->nodeB
							: edges[edIdx]->nodeA;
			visitedNodes[newNode->index] = 1;
			findFather(ptNRoot, other(edges[edIdx], newNode)->index);
			newPtNode = getNewNodePath(newNode->index, edIdx, father);
			if (isTarget(newPtNode, targetID)) {
				foundNewPath(newPtNode);
				numTargToHit--;
				if (numTargToHit == 0)
					break;
			}
			cleanEdgeCand();
		} while (numEdgesCandidates > 0);

	} while (numTargToHit > 0);
	if (numTargToHit == numTargets[targetID] - 1)
		arePaths[targetID] = 0;
	return;
}

int getAllPaths(Solution *s, char maxHits) {
	int oldNumPaths = numPaths;

	char equalPaths(PathTree *ptA, PathTree *ptB) {
		for (; ptA->father != NULL || ptB->father != NULL; ptA = ptA->father, ptB
				= ptB->father)
			if (ptA->edgeID != ptB->edgeID)
				return 0;
		return 1;
	}

	void clearClonePaths() {
		int i, j;
		for (i = oldNumPaths; i < numPaths - 1; i++)
			for (j = i + 1; j < numPaths; j++)
				if (equalPaths(paths[i], paths[j])) {
					numPaths--;
					paths[j] = paths[numPaths];
					j--;
				}
	}

	short i, j;
	for (j = 0; j < numTargetsRoot; j++)
		arePaths[j] = 1;

	for (i = 0; i < maxHits; i++) {
		for (j = 0; j < numTargetsRoot; j++)
			getPaths(s, j);
	}
	if (oldNumPaths > 0)
		clearClonePaths();
	return numPaths - oldNumPaths;
}

void SolverGuloso(Solution *s) {
	char isblockedPath(PathTree *ptN) {
		for (; ptN->father != NULL; ptN = ptN->father)
			if (s->selectedEdges[ptN->edgeID] == 1)
				return 1;
		return 0;
	}

	void clearSolution() {
		for (int i = 0; i < numEdges; i++)
			s->selectedEdges[i] = 0;
	}

	void penteFino() {
		short i;
		int newPaths;
		for (i = 0; i < numEdges; i++) {
			if (s->selectedEdges[i] == 1) {
				s->selectedEdges[i] = 0;
				newPaths = getAllPaths(s, 1);
				if (newPaths == 0)
					s->numSelect--;
				else
					s->selectedEdges[i] = 1;
			}
		}
	}

	void greedSetColverAlg() {
		short edgesLst[MAX_EDGES];
		int freePaths, edgeRelev[MAX_EDGES];
		char bPath[MAX_PATHS];

		int compareRelev(const void *a, const void *b) {
			return (int) ((-edgeRelev[*(short*) a] + edgeRelev[*(short*) b]));
		}

		void calcEdgesRelevance() {
			int i;
			for (i = 0; i < numEdges; i++)
				edgeRelev[i] = 0;
			freePaths = 0;
			for (i = 0; i < numPaths; i++)
				if (!bPath[i] && !isblockedPath(paths[i])) {
					freePaths++;
					for (PathTree *ptN = paths[i]; ptN->father != NULL; ptN
							= ptN->father)
						edgeRelev[ptN->edgeID]++;
				} else
					bPath[i] = 1;
		}

		s->numSelect = 0;
		for (short i = 0; i < numEdges; i++)
			edgesLst[i] = i;
		for (int i = 0; i < numPaths; i++)
			bPath[i] = 0;
		do {
			calcEdgesRelevance();
			if (freePaths == 0)
				break;
			qsort(edgesLst, (size_t) numEdges, sizeof(short), compareRelev);
			s->selectedEdges[edgesLst[0]] = 1;
			s->numSelect++;
		} while (edgeRelev[edgesLst[0]] != 0);
	}

	int newPaths;
	numPaths = 0;
	numNodePaths = 0;
	clearSolution();

	for (newPaths = getAllPaths(s, 1); newPaths != 0; newPaths = getAllPaths(s,
			20)) {
		clearSolution();
		greedSetColverAlg();
	}
	penteFino();
}

//***************** Funcoes Para Leitura dos 2 Arquivos de entrada

void removeCloneEdges() {
	char areCloneEdges(Edge *edA, Edge *edB) {
		return ((edA->nodeA == edB->nodeA && edA->nodeB == edB->nodeB)
				|| (edA->nodeA == edB->nodeB && edA->nodeB == edB->nodeA));
	}

	short i, j;
	char k;
	Node *ndo;
	Edge *edOut, *edKeep;

	for (i = 0; i < numEdges - 1; i++)
		for (j = i + 1; j < numEdges; j++)
			if (areCloneEdges(edges[i], edges[j])) {
				ndo = edges[j]->nodeA;
				for (k = 0; ndo->edgesNode[k] != edges[j]; k++)
					;
				ndo->numEdgesNode--;
				ndo->edgesNode[k] = ndo->edgesNode[ndo->numEdgesNode];
				ndo = edges[j]->nodeB;
				for (k = 0; ndo->edgesNode[k] != edges[j]; k++)
					;
				ndo->numEdgesNode--;
				ndo->edgesNode[k] = ndo->edgesNode[ndo->numEdgesNode];
				numEdges--;
				edOut = edges[j];
				edges[j] = edges[numEdges];
				edges[numEdges] = edOut;
				edges[j]->index = j;
				j--;
			}
}

void remove2ndDegreeNodes() {
	short i, j;
	Node *ndo;
	Edge *edOut, *edKeep;

	for (i = 0; i < numNodes; i++) {
		if (nodes[i]->numEdgesNode == 2 && !nodes[i]->isOD) {
			edKeep = nodes[i]->edgesNode[0];
			edOut = nodes[i]->edgesNode[1];
			ndo = other(edOut, nodes[i]);
			edKeep->nodeA = (edKeep->nodeA == nodes[i]) ? ndo : edKeep->nodeA;
			edKeep->nodeB = (edKeep->nodeB == nodes[i]) ? ndo : edKeep->nodeB;
			for (j = 0; ndo->edgesNode[j] != edOut; j++)
				;
			ndo->edgesNode[j] = edKeep;
			numNodes--;
			ndo = nodes[i];
			nodes[i] = nodes[numNodes];
			nodes[numNodes] = ndo;
			nodes[i]->index = i;
			j = edOut->index;
			numEdges--;
			edges[j] = edges[numEdges];
			edges[numEdges] = edOut;
			edges[j]->index = j;
			i--;
		}
	}
}

void leitura(char *file) {
	int i, j, k;
	float lenght;
	char edgeID[MAX_STRING_SIZE], nodeAID[MAX_STRING_SIZE],
			nodeBID[MAX_STRING_SIZE], *a;
	Edge *edg;
	Node *nodA, *nodB;
	char temp[1000];
	numEdges = 0;
	numNodes = 0;
	numTargetsRoot = 0;
	int nos = 0, arestas = 0, pod = 0;
	FILE *fileread = fopen(file, "r");

	if (fileread == NULL)
		exit(0);

	fgets(temp, sizeof 1000, fileread);
	printf("1a linha: %s \n", temp);

	fscanf(fileread, "%d %d %d", &nos, &arestas, &pod);

	printf("%d %d %d \n", nos, arestas, pod);

	int contOD = 0;
	while (contOD < pod) {
		i = fscanf(fileread, "%s", nodeAID);
		nodA = findOrCreateNode(nodeAID);
		nodA->isOD = 1;
		targets[numTargetsRoot][0] = nodA;
		numTargetsRoot++;
		contOD++;
	}
	numTargetsRoot--;

	int contA = 0;
	while (contA < arestas) {
		i = fscanf(fileread, "%s", edgeID);
		i = fscanf(fileread, "%s", nodeAID);
		i = fscanf(fileread, "%s", nodeBID);
		//		i = fscanf(fileread, "%f", &lenght);
		if (feof(fileread))
			break;

		nodA = findOrCreateNode(nodeAID);
		nodB = findOrCreateNode(nodeBID);

		edg = edges[numEdges];

		strcpy(edg->ID, edgeID);
		//		edg->lenght = lenght;
		edg->nodeA = nodA;
		edg->nodeB = nodB;

		nodA->edgesNode[nodA->numEdgesNode] = edg;
		nodA->numEdgesNode++;
		nodB->edgesNode[nodB->numEdgesNode] = edg;
		nodB->numEdgesNode++;
		numEdges++;
		contA++;
	}

	for (i = 0; i < numNodes; i++)
		nodes[i]->index = i;
	for (i = 0; i < numEdges; i++)
		edges[i]->index = i;

	for (i = 0; i < numTargetsRoot - 1; i++) {
		for (j = i + 1; j < numTargetsRoot; j++) {
			targets[i][j - i] = targets[j][0];
		}
		numTargets[i] = numTargetsRoot - i;
	}
	numTargetsRoot--;

	fclose(fileread);

	//	removeCloneEdges();
	if (WITH_2_DEGREE_NODES == 0)
		remove2ndDegreeNodes();
}

void startStructuresRealInstances(char *fileEdges, char *fileODs) {
	int i, j, k;
	float lenght;
	char edgeID[MAX_STRING_SIZE], nodeAID[MAX_STRING_SIZE],
			nodeBID[MAX_STRING_SIZE], *a;
	Edge *edg;
	Node *nodA, *nodB;

	numEdges = 0;
	numNodes = 0;
	numTargetsRoot = 0;

	FILE *fileread = fopen(fileEdges, "r");

	if (fileread == NULL)
		exit(0);

	while (!feof(fileread)) {
		i = fscanf(fileread, "%s", edgeID);
		i = fscanf(fileread, "%s", nodeAID);
		i = fscanf(fileread, "%s", nodeBID);
		i = fscanf(fileread, "%f", &lenght);
		if (feof(fileread))
			break;

		nodA = findOrCreateNode(nodeAID);
		nodB = findOrCreateNode(nodeBID);

		edg = edges[numEdges];

		strcpy(edg->ID, edgeID);
		edg->lenght = lenght;
		edg->nodeA = nodA;
		edg->nodeB = nodB;

		nodA->edgesNode[nodA->numEdgesNode] = edg;
		nodA->numEdgesNode++;
		nodB->edgesNode[nodB->numEdgesNode] = edg;
		nodB->numEdgesNode++;
		numEdges++;
	}
	fclose(fileread);

	fileread = fopen(fileODs, "r");
	while (!feof(fileread)) {
		i = fscanf(fileread, "%s", nodeAID);
		nodA = findOrCreateNode(nodeAID);
		nodA->isOD = 1;
		targets[numTargetsRoot][0] = nodA;
		numTargetsRoot++;
	}
	numTargetsRoot--;
	fclose(fileread);

	for (i = 0; i < numNodes; i++)
		nodes[i]->index = i;
	for (i = 0; i < numEdges; i++)
		edges[i]->index = i;

	for (i = 0; i < numTargetsRoot - 1; i++) {
		for (j = i + 1; j < numTargetsRoot; j++) {
			targets[i][j - i] = targets[j][0];
		}
		numTargets[i] = numTargetsRoot - i;
	}
	numTargetsRoot--;

	removeCloneEdges();
	if (WITH_2_DEGREE_NODES == 0)
		remove2ndDegreeNodes();
}

//========================================= CLUSTERING SEARCH =========================================
// ----- Parâmetros
char INST[50] = "AC"; // arquivo com a instância (estado)
char SAI[50] = "saida.txt"; // nome do arquivo de saída 'geral' (com o resumo de todas as execuções)
short NUM_EXE = 3; // número de execuções do método
double MAX_TIME = 10.0; // tempo máximo de execução (segundos)
short MAX_ITE = 2; // número máximo de iterações (MAX_ITE = MAX_ITE * número de arestas)
double INI_TMP = 1; // temperatura inicial (INI_TMP = INI_TMP * fo da solução inicial)
double FRZ_TMP = 0.01; // temperatura de congelamento
double COO_RTE = 0.975; // taxa de resfriamento
short NUM_CLT = 3; // número máximo de clusters
short MAX_VOL = 2; // volume máximo dos clusters
short MAX_INE = 3; // índice de ineficácia máximo
// ----- Variáveis do CS
short numCltUsa__; // número de clusters usados
short vetVolClt__[MAX_CLT]; // vetor com o volume de cada cluster
short vetIneClt__[MAX_CLT]; // vetor com o índice de ineficácia de cada cluster
Solution vetClt__[MAX_CLT]; // vetor com o centro (solução) de cada cluster
// ----- Variáveis com os resultados
short foIni__, foFin__; // função objetivo das soluções inicial e final
double solTime__, bstTime__, excTime__; // tempo para gerar a solução inicial, para encontrar a melhor solução, e tempo total
int solAva__; // número de soluções avaliadas pelo CS

//------------------------------------------------------------------------------
char existPath(Solution *s) {
	numPaths = 0;
	numNodePaths = 0;
	for (short i = 0; i < numTargetsRoot; i++) {
		arePaths[i] = 1;
		getPaths(s, i);
		if (arePaths[i])
			return 1;
	}
	return 0;
}
//------------------------------------------------------------------------------

//------------------------------------------------------------------------------
int pathsCount(Solution *s) {
	numPaths = 0;
	numNodePaths = 0;
	return getAllPaths(s, 1);
}
//------------------------------------------------------------------------------

//------------------------------------------------------------------------------
void copySolution(Solution *ori, Solution *cop) {
	for (short i = 0; i < numEdges; i++)
		cop->selectedEdges[i] = ori->selectedEdges[i];
	cop->numSelect = ori->numSelect;
}
//------------------------------------------------------------------------------

//------------------------------------------------------------------------------
void createIniSol(Solution *s) {
	double hI, hF;
	hI = getCpuTime();
	SolverGuloso(s);
	hF = getCpuTime();
	solTime__ = hF - hI;
}
//------------------------------------------------------------------------------

//------------------------------------------------------------------------------
void genNeighbor(Solution *s) {
	short pos1, pos2, aux, paths;
	pos1 = rand() % numEdges;
	if (s->selectedEdges[pos1] == 0) {
		s->selectedEdges[pos1] = 1;
		s->numSelect++;
		for (short i = 0; i < numEdges; i++) {
			do
				pos2 = rand() % numEdges;
			while ((s->selectedEdges[pos2] == 0) || (pos2 == pos1));
			s->selectedEdges[pos2] = 0;
			s->numSelect--;
			if (existPath(s)) {
				s->selectedEdges[pos2] = 1;
				s->numSelect++;
			}
		}
	} else {
		paths = pathsCount(s);
		s->selectedEdges[pos1] = 0;
		s->numSelect--;
		paths = pathsCount(s);
		while (paths > 0) {
			do
				pos2 = rand() % numEdges;
			while (s->selectedEdges[pos2] == 1);
			s->selectedEdges[pos2] = 1;
			s->numSelect++;
			aux = paths;
			paths = pathsCount(s);
			if (paths >= aux) {
				s->selectedEdges[pos2] = 0;
				s->numSelect--;
			}
		}
	}
}
//------------------------------------------------------------------------------

//------------------------------------------------------------------------------
short atuCenClt(Solution *s) {
	short clt, dst, bst;
	bst = numEdges + 1;
	for (short i = 0; i < NUM_CLT; i++) {
		dst = 0;
		for (short j = 0; j < numEdges; j++)
			if (s->selectedEdges[j] != vetClt__[i].selectedEdges[j])
				dst++;
		if (dst < bst) {
			bst = dst;
			clt = i;
		}
	}
	if (s->selectedEdges < vetClt__[clt].selectedEdges)
		copySolution(s, &vetClt__[clt]);
	return clt;
}
//------------------------------------------------------------------------------

//------------------------------------------------------------------------------
void localSearch(Solution *s) {
	short pos1, pos2, pos3, bst;
	while (1) {
		bst = s->numSelect;
		while (1) {
			pos1 = rand() % numEdges;
			do
				pos2 = rand() % numEdges;
			while (s->selectedEdges[pos2] == s->selectedEdges[pos1]);
			s->selectedEdges[pos1] = 1 - s->selectedEdges[pos1];
			s->selectedEdges[pos2] = 1 - s->selectedEdges[pos2];
			if (existPath(s)) {
				s->selectedEdges[pos1] = 1 - s->selectedEdges[pos1];
				s->selectedEdges[pos2] = 1 - s->selectedEdges[pos2];
			} else
				break;
		}
		for (short i = 0; i < numEdges; i++) {
			do
				pos3 = rand() % numEdges;
			while ((s->selectedEdges[pos3] == 0) || (pos3 == pos2) || (pos3
					== pos1));
			s->selectedEdges[pos3] = 0;
			s->numSelect--;
			if (existPath(s)) {
				s->selectedEdges[pos3] = 1;
				s->numSelect++;
			}
		}
		if (s->numSelect < bst)
			bst = s->numSelect;
		else
			break;
	}
}
//------------------------------------------------------------------------------

//------------------------------------------------------------------------------
void startCS() {
	numCltUsa__ = 0;
	memset(vetVolClt__, 0, sizeof(vetVolClt__));
	memset(vetIneClt__, 0, sizeof(vetIneClt__));
	memset(vetClt__, 0, sizeof(vetClt__));
}
//------------------------------------------------------------------------------

//------------------------------------------------------------------------------
void execCS(Solution *s) {
	int iter, itMax;
	short foAux, clt;
	double x, iniTmp, tmp, tempo, hI, hF;
	Solution s1, s2;
	Solution *sAux1 = &s1;
	Solution *sAux2 = &s2;
	iniTmp = INI_TMP * s->numSelect;
	itMax = MAX_ITE * numEdges;
	bstTime__ = solTime__;
	hI = getCpuTime();
	copySolution(s, sAux1);
	foIni__ = s->numSelect;
#ifdef DBG
	printf("\n>>> Sol. Ini.: %d\tTempo: %.2f\n\n",foIni__,solTime__);
#endif
	excTime__ = 0.0;
	solAva__ = 0;
	tempo = MAX_TIME - solTime__;
	solAva__ = 1;
	while (excTime__ < tempo) {
		tmp = iniTmp;
		while (tmp > FRZ_TMP) {
			for (int i = 0; i < itMax; i++) {
				solAva__++;
				copySolution(sAux1, sAux2);
				genNeighbor(sAux1);
				if (sAux1->numSelect < sAux2->numSelect) {
					if (sAux1->numSelect < s->numSelect) {
						hF = getCpuTime();
						bstTime__ = hF - hI;
						copySolution(sAux1, s);
#ifdef DBG
						printf(">>> Best Sol: %d\tTempo: %.2f\n",s->numSelect,bstTime__);
#endif
					}
				} else {
					x = rand() % 1001;
					x = x / 1000.0;
					if (!(x
							< expl(-(sAux1->numSelect - sAux2->numSelect) / tmp)))
						copySolution(sAux2, sAux1);
				}
				hF = getCpuTime();
				excTime__ = hF - hI;
				if (excTime__ >= tempo)
					break;
			}
			//--------------------------------------------
			// CS
			if (numCltUsa__ < NUM_CLT) {
				vetVolClt__[numCltUsa__]++;
				copySolution(sAux1, &vetClt__[numCltUsa__]);
				numCltUsa__++;
			} else {
				clt = atuCenClt(sAux1);
				vetVolClt__[clt]++;
				if (vetVolClt__[clt] >= MAX_VOL) {
					vetVolClt__[clt] = 1;
					if (vetIneClt__[clt] >= MAX_INE) {
						genNeighbor(&vetClt__[clt]);
						vetIneClt__[clt] = 0;
					} else {
						foAux = vetClt__[clt].numSelect;
						localSearch(&vetClt__[clt]);
						if (vetClt__[clt].numSelect < foAux)
							vetIneClt__[clt] = 0;
						else
							vetIneClt__[clt]++;
					}
					if (vetClt__[clt].numSelect < s->numSelect) {
						hF = getCpuTime();
						bstTime__ = hF - hI;
						copySolution(&vetClt__[clt], s);
#ifdef DBG
						printf(">>> Best Sol: %d\tTempo: %.2f\n",s->numSelect,bstTime__);
#endif
					}
				}
			}
			//-----------------------------------------------
			hF = getCpuTime();
			excTime__ = hF - hI;
			if (excTime__ >= tempo)
				break;
			tmp = COO_RTE * tmp;
		}
	}
	hF = getCpuTime();
	excTime__ = hF - hI;
	foFin__ = s->numSelect;
#ifdef DBG
	printf("\n>>> Sol. Fin.: %d\tTempo: %.2f\tSol. Ava.: %d\n",foFin__,solTime__+excTime__,solAva__);
#endif
}
//------------------------------------------------------------------------------

//------------------------------------------------------------------------------
void writeResults(Solution *s) {
	char aux[MAX_STRING_SIZE];
	sprintf(aux, "Solucoes//bstSol-%s.txt", INST);
	FILE *f = fopen(aux, "w");
	fprintf(f,
			"< ---------------------------- PROBLEMA ---------------------------- >\n");
	fprintf(f, "Instancia...........................................: %s\n",
			INST);
	fprintf(f, "Numero de nos.......................................: %d\n",
			numNodes);
	fprintf(f, "Numero de arestas...................................: %d\n",
			numEdges);
	fprintf(f, "Numero de nos que definem os pares..................: %d\n",
			numTargetsRoot);
	fprintf(f, "Numero total de pares (ida e volta).................: %d\n",
			numTargetsRoot * (numTargetsRoot - 1));
	fprintf(f,
			"\n< --------------------------- PARAMETROS --------------------------- >\n");
	fprintf(f, "Numero de clusters..................................: %d\n",
			NUM_CLT);
	fprintf(f, "Limite maximo de volume dos clusters................: %d\n",
			MAX_VOL);
	fprintf(f, "Limite maximo de ineficacia dos clusters............: %d\n",
			MAX_INE);
	fprintf(f, "Temperatura inicial.................................: %.2f\n",
			INI_TMP);
	fprintf(f, "Temperatura de congelamento.........................: %.3f\n",
			FRZ_TMP);
	fprintf(f, "Taxa de resfriamento................................: %.3f\n",
			COO_RTE);
	fprintf(f, "N. maximo de iteracoes..............................: %d\n",
			MAX_ITE);
	fprintf(
			f,
			"Tempo maximo de processamento.......................: %.2f segundos!\n",
			MAX_TIME);
	fprintf(f,
			"\n< --------------------------- RESULTADOS --------------------------- >\n");
	fprintf(f, "Custo da solucao inicial............................: %d\n",
			foIni__);
	fprintf(f, "Custo da solucao final..............................: %d\n",
			foFin__);
	fprintf(f, "Numero de solucoes avaliadas........................: %d\n",
			solAva__);
	fprintf(
			f,
			"Tempo para gerar a solucao inicial..................: %.2f segundos!\n",
			solTime__);
	fprintf(
			f,
			"Tempo para encontrar a melhor solucao (BST).........: %.2f segundos!\n",
			bstTime__);
	fprintf(
			f,
			"Tempo de execucao do metodo.........................: %.2f segundos!\n",
			excTime__);
	fprintf(
			f,
			"Tempo total de processamento........................: %.2f segundos!\n",
			solTime__ + excTime__);
	fprintf(f,
			"\n< ---------------------------- SOLUCAO ----------------------------- >\n");
	fprintf(f, "Numero de postos de contagem instalados.............: %d\n",
			s->numSelect);
	fprintf(f, "\nVetor solucao: [ ");
	for (short i = 0; i < numEdges; i++)
		fprintf(f, "%d ", s->selectedEdges[i]);
	fprintf(f, "]\n\n");
	fprintf(
			f,
			"-----------------------\n>>> ARESTAS PARA INSTALACAO DOS POSTOS DE CONTAGEM:\n");
	for (short i = 0; i < numEdges; i++)
		if (s->selectedEdges[i] == 1)
			fprintf(f, "%s\r\n", edges[i]->ID);
	fclose(f);
}
//------------------------------------------------------------------------------

//=====================================================================================================


int main(int argc, const char * argv[]) {
	FILE *arq;
	char fileEdges[MAX_STRING_SIZE], fileNodes[MAX_STRING_SIZE],
			fileName[MAX_STRING_SIZE];
	int bstSol;
	double solMed, medSolAva, temMed, desvio;
	Solution sol;
	Solution *s = &sol;
#ifdef RND
	srand(time(NULL));
#else
	NUM_EXE = 1;
#endif
	if (argc > 1) {
		strcpy(fileName, argv[1]);
		NUM_EXE = atoi(argv[2]);
		NUM_CLT = atof(argv[3]);
		MAX_VOL = atof(argv[4]);
		MAX_INE = atof(argv[5]);
		INI_TMP = atof(argv[6]);
		FRZ_TMP = atof(argv[7]);
		COO_RTE = atof(argv[8]);
		MAX_ITE = atoi(argv[9]);
		MAX_TIME = atoi(argv[10]);
		strcpy(SAI, argv[11]);
	}
	startAlocations();
	//	sprintf(fileEdges, "Brasil//arestas%s.txt", INST);
	//	sprintf(fileNodes, "Brasil//centroides%s.txt", INST);
	//	startStructuresRealInstances(fileEdges, fileNodes);
	leitura(fileName);
	bstSol = numEdges + 1;
	solMed = medSolAva = temMed = desvio = 0.0;
	for (int r = 1; r <= NUM_EXE; r++) {
		printf("\n\n>>> Resolvendo a instancia %s com o CS - rodada %d\n\n",
				INST, r);
		createIniSol(s);
		startCS();
		execCS(s);
		if (s->numSelect < bstSol) {
			writeResults(s);
			bstSol = s->numSelect;
		}
		solMed += s->numSelect;
		medSolAva += solAva__;
		temMed += bstTime__;
	}
	solMed = solMed / (double) NUM_EXE;
	medSolAva = medSolAva / (double) NUM_EXE;
	temMed = temMed / (double) NUM_EXE;
	desvio = ((solMed - bstSol) / bstSol) * 100;
	arq = fopen(SAI, "at");
	fprintf(arq, "%s\t%d\t%d\t%d\t%.2f\t\t%d\t\t%.2f\t\t%.2f\t\t%.2f\n", INST,
			numNodes, numEdges, numTargetsRoot, medSolAva, bstSol, solMed,
			temMed, desvio);
	fclose(arq);
	return EXIT_SUCCESS;
}
