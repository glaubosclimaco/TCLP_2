#ifndef MinCut_H_
#define MinCut_H_

#include <lemon/smart_graph.h>
#include <lemon/preflow.h>
#include <lemon/edmonds_karp.h>
#include <lemon/bfs.h>

#include <vector>
#include <set>
#include <stdio.h>

using namespace std;

//Tipo Estruturado para vertice
// typedef struct vertice {
// 	int id;
// 	int tipo;
// 	double premio;
// 	vector<int> cobre; //conjunto de vertices W, que este vertice V cobre
// 	vector<int> coberto_por;
// 	bool reduzido;
// } Vertice;
typedef lemon::SmartDigraph Digraph;

class MinCut {
private:
	int N;
	Digraph *g;
	Digraph::ArcMap<double> *dist;
	Digraph::NodeMap<bool> *cut;
	lemon::Preflow<Digraph, Digraph::ArcMap<double> > *alg;

public:
	MinCut();
	void init();
	void build_graph(int *sol, int n, double **x);
	void build_graph2(int *sol, int n, double **x);
	void run_maxflow(int *sol, int n, double **x);
	int addNode();
	int addArc(int i, int j);
	void addCost(int e_id, double cost);
	bool node_side(int n);
	void run();
	double generate_min_cut(int source, int target);
	double runFlow(int source, int target);
	double fluxoMax(int source, int target);
	~MinCut();
	void setG(const Digraph*& g);
	void buildGraphFlow(int n, int **x);
};

#endif /* MinCut_H_ */
