#include <algorithm>
#include "Graph.h"
#include "MinCut.h"
#include <ctime>
#include <lemon/maps.h>
#include <lemon/kruskal.h>
#include <lemon/list_graph.h>
#include <sstream>

using namespace std;
using namespace lemon;

string itos(int i) // convert int to string
		{
	stringstream s;
	s << i;
	return s.str();
}

/*
 * Estruturas de dados para a 2-Matching
 */

typedef struct node {
	int id;
//	vector<int> adjID;
	bool visitado;
} no;

typedef struct edge {
	no o;
	no d;
	double custo;

} aresta;

typedef struct conjunto2Matching {
	int V;
	int E;
	vector<no> nos;
	vector<aresta> arestas;
	vector<vector<no> > adj;
} conj2M;

// FIM ESTRUTRAS DE DADOS 2-MATCHING

//Tipo Estruturado para Vertex
// typedef struct Vertex {
// 	int id;
// 	int tipo;
// 	double premio;
// 	vector<int> cobre; //conjunto de Vertexs W, que este Vertex V cobre
// 	vector<int> coberto_por;
// 	bool reduzido;
// } Vertex;

int cover_cuts = 0;
int cut_sets = 0;
int twoMatching_cuts = 0;
int mipsol8 = 0;
int mipsol7 = 0;
int mipnode9 = 0;
int mipsol9 = 0;
bool do_cuts = true;
std::vector<long> hash_table;
std::vector<long> hash_table_cs;

typedef struct a_node {
	int id;
	ListGraph::Node node;
} a_node;

typedef struct an_edge {
	double cost;
	ListGraph::Edge edge;
} an_edge;

typedef struct type_VH {
	std::vector<Vertex> e;
} type_VH;

/*
 FUNÇÕES AUXILIARES
 */

//order idependent hash function
long hash(vector<Vertex> v) {
	long r = 0l;
	r = v.size(); // para reduzir conflitos
	int k = 33; // constante para reduzir conflitos
	for (unsigned i = 0; i < v.size(); i++) {
		r += k * v[i].id;
	}

	return (r);
}

//função para ordenar elementos da cover
bool myfunction(Vertex a, Vertex b) {
	return (a.cover_value < b.cover_value);
}

//funcao para ordenar (nao decrescente) um vector de arestas, de acordo com o seu custo
bool myfunctionAresta(aresta a, aresta b) {
	return (a.custo < b.custo);
}

bool ArestaDecresc(aresta a, aresta b) {
	return (a.custo > b.custo);
}

bool non_increasing_prize(Vertex a, Vertex b) {
	return (a.premio > b.premio);
}

//funcao para retornar a componente conexa a que uma aresta, passada por parametro, pertence
conj2M bfs2Matching(conj2M grafo, aresta e, int n, double **x, double& sum,
		bool debug) {
	conj2M componente;

	// Mark all the vertices as not visited
	bool *visited = new bool[n];
	for (int i = 0; i < n; i++)
		visited[i] = false;

	no s = e.o;

	// Create a queue for BFS
	vector<no> queue;

	// Mark the current node as visited and enqueue it
	visited[s.id] = true;
//	s.visitado = true;
	queue.push_back(s);

	// 'i' will be used to get all adjacent vertices of a vertex
	vector<no>::iterator it;

	while (!queue.empty()) {
		// Dequeue a vertex from queue and print it
		s = queue.front();
		queue.erase(queue.begin());
		componente.nos.push_back(s);

		// Get all adjacent vertices of the dequeued vertex s
		// If a adjacent has not been visited, then mark it visited
		// and enqueue it
//		cout << "\n" << s.id << ": ";
		for (it = grafo.adj[s.id].begin(); it != grafo.adj[s.id].end(); ++it) {
			if (!visited[(*it).id]) {
				visited[(*it).id] = true;
				queue.push_back(*it);
				if (debug) {
					printf("(\"%d\", \"%d\"), ", s.id, (*it).id);
				}
				aresta arestaTemp;
				arestaTemp.o = s;
				arestaTemp.d = (*it);
				if (s.id < (*it).id) {
					arestaTemp.custo = x[s.id][((*it).id)];
				} else {
					arestaTemp.custo = x[((*it).id)][s.id];
				}
				sum += arestaTemp.custo;
				componente.arestas.push_back(arestaTemp);
			}
		}
	}

	return (componente);
}

//int* escolhaTeeths(vector<aresta> teeths) {
//	int n = teeths.size();
//	int* escolhidos;
//	escolhidos = new int[n];
//
//	GRBEnv env = GRBEnv();
////	env.set(GRB_IntParam_Presolve, 0);
////	env.set(GRB_IntParam_Cuts, 0);
//	//necessario quando se utiliza seus próprios cortes
////	env.set(GRB_IntParam_PreCrush, 1);
//	env.set(GRB_IntParam_Threads, 1);
////	env.set(GRB_DoubleParam_Heuristics, 0.0);
//	env.set(GRB_DoubleParam_TimeLimit, 3600.0);
//	//env->set(GRB_IntParam_OutputFlag, 0);
//
//	GRBModel model = GRBModel(env);
//
//	GRBVar *z = 0; //representa os vertices
//	GRBVar **x = 0; //representa as arestas
//	char varName[100];
//
//	//cadastrando variavies
//	z = new GRBVar[n];
//
//}

//se o conjunto 'a' contemM o conjunto 'b'
bool contemM(vector<int> a, vector<int> b) {
	for (int i = 0; i < b.size(); i++) {
		bool contains = find(a.begin(), a.end(), b[i]) != a.end();
		if (!contains)
			return false;
	}
	return true;
}

//verifica se ha interseccao entre 'a' e 'b'
bool ha_interseccaoM(vector<int> a, vector<int> b) {
	for (int i = 0; i < a.size(); i++) {
		bool achou = find(b.begin(), b.end(), a[i]) != b.end();
		if (achou)
			return true;
	}

	return false;
}

bool teste_dois_em_Rw(int n, vector<Vertex> &v, Grafo g);
bool teste_t_rw(int n, vector<Vertex> &Vertexs, Grafo g);
bool teste_dois_t(int n, vector<Vertex> &Vertexs, Grafo g);

/*
 REGRAS DE REDUÇÃO
 */

/*int regra_R0(vector<Vertex> &Vertexs, int &qnd_w) {
 int qtd_w = 0;
 int qtd_w_reduzidos = 0;
 for (int l = 0; l < Vertexs.size(); l++) {
 int i = l;
 if (Vertexs[i].tipo != 2)
 continue;
 qtd_w++;
 //verificando se w é coberto por algum t
 for (int j = 0; j < Vertexs[i].coberto_por.size(); j++) {
 int indiceT;
 indiceT = Vertexs[i].coberto_por[j];
 if (Vertexs[indiceT].tipo == 1) {
 Vertexs[i].reduzido = true;
 qtd_w_reduzidos++;
 qnd_w--;
 break;
 }
 }
 }
 // printf("Quantidade W Reduzidos %d/%d \n", qtd_w_reduzidos, qtd_w);
 return qtd_w_reduzidos;
 }*/

/*
 Essa regra pode aumentar o número de arestas, mas pode facilitar a redução por outras
 regras e diminui o número de vértices. Se $r \in R \setminus R_W$ e $p_r = 0$,
 então $R = R \setminus \{ r \}$ e $\forall (i,r), (j,r) \in \delta(r)$ $c_{ij} = \min (c_{ij},
 c_{ir} + c_{rj}$, caso $(i,j) \notin E$ adicionamos $(i,j)$ em $E$ com custo $c_{ir} + c_{rj}$
 */

//auxiliares
void printArestas(vector<aresta> arestas, bool comPeso) {
	printf("\n");
	for (vector<aresta>::iterator it = arestas.begin(); it != arestas.end();
			++it) {
		if (comPeso)
			printf("(\"%d\",\"%d\",\"%f\"), \t", (*it).o.id, (*it).d.id,
					(*it).custo);
		else

			printf("(\"%d\", \"%d\"), \t", (*it).o.id, (*it).d.id);
	}
	printf("\n");
}

bool cicla(conj2M agm, aresta arestaAdd) {
	bool oIn = false, dIn = false;
	for (unsigned i = 0; i < agm.arestas.size(); i++) {
		aresta arestaTemp = agm.arestas[i];
		if (arestaAdd.o.id == arestaTemp.o.id
				or arestaAdd.o.id == arestaTemp.d.id) {
			oIn = true;
		}
		if (arestaAdd.d.id == arestaTemp.o.id
				or arestaAdd.d.id == arestaTemp.d.id) {
			dIn = true;
		}

		if (oIn == true and dIn == true) {
			return (false);
		}

	}

	return (true);
}

/* Returns true if there is a path from source 's' to sink 't' in
 residual graph. Also fills parent[] to store the path */
bool bfs(int n, int** rGraph, int s, int t, int parent[]) {
	// Create a visited array and mark all vertices as not visited
	bool visited[n];
	memset(visited, 0, sizeof(visited));

	// Create a queue, enqueue source vertex and mark source vertex
	// as visited
	queue<int> q;
	q.push(s);
	visited[s] = true;
	parent[s] = -1;

	// Standard BFS Loop
	while (!q.empty()) {
		int u = q.front();
		q.pop();

		for (int v = 0; v < n; v++) {
			if (visited[v] == false && rGraph[u][v] > 0) {
				q.push(v);
				parent[v] = u;
				visited[v] = true;
			}
		}
	}

	// If we reached sink in BFS starting from source, then return
	// true, else false
	return (visited[t] == true);
}

// Returns the maximum flow from s to t in the given graph
int fordFulkerson(int** graph, int s, int t, int n) {
	int u, v;
//	printf("\taplicando fluxo maximo \n");
	// Create a residual graph and fill the residual graph with
	// given capacities in the original graph as residual capacities
	// in residual graph
	int **rGraph; // Residual graph where rGraph[i][j] indicates
				  // residual capacity of edge from i to j (if there
				  // is an edge. If rGraph[i][j] is 0, then there is not)

	rGraph = new int*[n];
	for (int i = 0; i < n; i++) {
		rGraph[i] = new int[n];
	}

	for (u = 0; u < n; u++)
		for (v = 0; v < n; v++){
			rGraph[u][v] = graph[u][v];
			rGraph[v][u] = graph[u][v];
		}

	int *parent = new int[n];  // This array is filled by BFS and to store path


	int max_flow = 0;  // There is no flow initially

	// Augment the flow while tere is path from source to sink
	while (bfs(n, rGraph, s, t, parent)) {
		// Find minimum residual capacity of the edges along the
		// path filled by BFS. Or we can say find the maximum flow
		// through the path found.
		int path_flow = INT_MAX;
		for (v = t; v != s; v = parent[v]) {
			u = parent[v];
			path_flow = min(path_flow, rGraph[u][v]);
		}

		// update residual capacities of the edges and reverse edges
		// along the path
		for (v = t; v != s; v = parent[v]) {
			u = parent[v];
			rGraph[u][v] -= path_flow;
			rGraph[v][u] += path_flow;
		}

		// Add path flow to overall flow
		max_flow += path_flow;
	}

	// Return the overall flow
	return max_flow;
}

