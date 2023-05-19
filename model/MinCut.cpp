#include "MinCut.h"

using namespace std;
using namespace lemon;

MinCut::MinCut() {
	g = NULL;
	alg = NULL;
	dist = NULL;
	cut = NULL;
	N = 0;
}

MinCut::~MinCut() {
	delete (g);
	delete (alg);
	delete (dist);
	delete (cut);
}

void MinCut::init() {
	if (g != NULL)
		delete g;
	if (dist != NULL)
		delete dist;
	if (alg != NULL)
		delete alg;
	this->N = 0;
	this->g = new Digraph();
	this->dist = new Digraph::ArcMap<double>(*g);
}

void MinCut::buildGraphFlow(int n, int **x) {
	init();
	for (int i = 0; i < n; ++i) { // n nos no grafo
		addNode();
	}

	for (int i = 0; i < n; i++) {
		for (int j = i + 1; j < n; j++) {
			//
			if (x[i][j] >= 0.9) {
				int id = addArc(i, j);
				(*dist)[g->arcFromId(id)] = 1.0;
				//				arcos++;
				// printf("add %d-%d=%.2f \n", i, j, x[i][j]);
				id = addArc(j, i);
				(*dist)[g->arcFromId(id)] = 1.0;
				// printf("add %d-%d=%.2f \n", j, i, x[i][j]);
				//				arcos++;
			}
		}
	}

}

void MinCut::build_graph(int *sol, int n, double **x) {
	init();

	for (int i = 0; i < n; ++i) { // n nos no grafo
//		if (sol[i] == 1)
		addNode();
	}

	for (int i = 0; i < n; i++) {
		if (sol[i] != 1)
			continue;
		for (int j = i + 1; j < n; j++) {
			if (sol[j] != 1)
				continue;
			//
			if (x[i][j] > 0) {
				// printf("x_%d_%d=%.2f \t", i, j, x[i][j]);
				int id = addArc(i, j);
				(*dist)[g->arcFromId(id)] = x[i][j];
//				arcos++;
				// printf("add %d-%d=%.2f \n", i, j, x[i][j]);
				id = addArc(j, i);
				(*dist)[g->arcFromId(id)] = x[i][j];
				// printf("add %d-%d=%.2f \n", j, i, x[i][j]);
//				arcos++;
			}
		}
	}
}

void MinCut::build_graph2(int *sol, int n, double **x) {
	init();
	int nnos = 0;
	int arcos = 0;

	for (int i = 0; i < n + 1; ++i) { // n nos no grafo
//		if (sol[i] == 1)
		addNode();
	}

	for (int i = 0; i < n + 1; i++) {
		if (sol[i] != 1)
			continue;
		for (int j = i + 1; j < n; j++) {
			if (sol[j] != 1)
				continue;
			//
			if (x[i][j] >= 1e-5) {
				// printf("x_%d_%d=%.2f \t", i, j, x[i][j]);
				int id = addArc(i, j);
				(*dist)[g->arcFromId(id)] = x[i][j];
				arcos++;
				// printf("add %d-%d=%.2f \n", i, j, x[i][j]);
				id = addArc(j, i);
				(*dist)[g->arcFromId(id)] = x[i][j];
				// printf("add %d-%d=%.2f \n", j, i, x[i][j]);
				arcos++;
			}
		}
	}
}

void MinCut::run_maxflow(int *sol, int n, double **x) {
	init();
	build_graph(sol, n, x);

}

int MinCut::addNode() {
	Digraph::Node node = g->addNode();
	++N;
	return g->id(node);
}

int MinCut::addArc(int i, int j) {
	Digraph::Node node_i = g->nodeFromId(i);
	Digraph::Node node_j = g->nodeFromId(j);
	Digraph::Arc e = g->addArc(node_i, node_j);
	return g->id(e);
}

void MinCut::addCost(int e_id, double cost) {
	Digraph::Arc e = g->arcFromId(e_id);
	(*dist)[e] = cost;
}

bool MinCut::node_side(int n) {
	Digraph::Node node = g->nodeFromId(n);
	return (*cut)[node];
}

double MinCut::generate_min_cut(int source, int target) {
	// printf("generate_min_cut: \n");
	if (cut != NULL)
		delete cut;
	cut = new Digraph::NodeMap<bool>(*g);
	if (alg != NULL)
		delete alg;
	// printf("antes EdmondsKarp \n");
	this->alg = new Preflow<Digraph, Digraph::ArcMap<double> >(*g, *dist,
			g->nodeFromId(source), g->nodeFromId(target));
	this->alg->run();
	// printf("depois EdmondsKarp\n");
	double val = this->alg->flowValue();
	// this->alg->minCutMap(*cut);
	return (val);
}

double MinCut::runFlow(int source, int target) {
	 printf("run flow: \n");
//	if (cut != NULL)
//		delete cut;
//	cut = new Digraph::NodeMap<bool>(*g);
	if (alg != NULL)
		delete alg;
	 printf("antes preflow\n");
	this->alg = new Preflow<Digraph, Digraph::ArcMap<double> >(*g, *dist,
			g->nodeFromId(source), g->nodeFromId(target));
	this->alg->run();
	 printf("depois preflow\n");
	double val = this->alg->flowValue();
	// this->alg->minCutMap(*cut);
	return (val);
}


