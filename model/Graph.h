#include <iostream>
#include <list>
#include <vector>
#include <cstdio>
#include <bits/stdc++.h>
#define NIL -1

using namespace std;

//int tVisitados = 0;
//aux for reduction rules

//Tipo Estruturado para Vertex
typedef struct Vertex {
	int id;
	int tipo;
	double premio;
	vector<int> cobre; //conjunto de Vertexs W, que este Vertex V cobre
	vector<int> coberto_por;
	bool reduzido;
	double cover_value;
	int inVh;
	int removed;
	int penalidade;
} Vertex;

// A structure to represent a edge between
// two vertex
struct Edge {
	int v;  // Vertex v (or "to" vertex)
			// of a directed edge u-v. "From"
			// vertex u can be obtained using
			// index in adjacent array.

	int flow; // flow of data in edge

	int C;    // capacity

	int rev; // To store index of reverse
			 // edge in adjacency list so that
			 // we can quickly find it.
};

// A directed Grafo using adjacency list representation
class Grafo {
	int V; // No. of Vertexs in Grafo
	list<int> *adj; // Pointer to an array containing adjacency lists

	// A recursive function used by printAllPaths()
	void printAllPathsUtil(int, int, bool[], int[], int &);
	vector<bool> visitedTs;
	int *level; // stores level of a node
	vector<Edge> *adjFlow;

public:

	Grafo(int V);   // Constructor
	void addEdge(int u, int v);
	void path(int s, int d, int[]);
	void remove_edges(list<int> nodes);
	void print();
	bool isReachable(int s, int d, int* pred);
	void connected(vector<Vertex> Vertexs, int& restantes, bool *visited,
			vector<int>& componente);
	int degree(int v);
//	bool isBCUtil(int v, bool visited[], int disc[], int low[], int parent[],
//			vector<Vertex> vertexs);
//	bool isBC(vector<Vertex> vertexs);   // returns true if graph is Biconnected
	void copy(Grafo g);
	int getV() {
		return (this->V);
	}

//	list<int>*& getAdj();
//	void setAdj(const list<int>*& adj);
	void setV(int v);
	vector<bool>& getVisitedTs();
	void setVisitedTs(vector<bool>& visitedTs);
	bool isBCUtil(int v, bool visited[], int disc[], int low[], int parent[]);
	bool isBC(int);    // returns true if graph is Biconnected
	list<int>*& getAdj();
	void setAdj(list<int>*& adj);
	void printFlow();
	// add edge to the graph
	void addEdgeFlow(int u, int v, int C) {
		// Forward edge : 0 flow and C capacity
		Edge a { v, 0, C, adjFlow[v].size() };

		// Back edge : 0 flow and 0 capacity
		Edge b { u, 0, 0, adjFlow[u].size() };

		adjFlow[u].push_back(a);
		adjFlow[v].push_back(b); // reverse edge
	}

	bool BFS(int s, int t);
	int sendFlow(int s, int flow, int t, int ptr[]);
	int DinicMaxflow(int s, int t);
};

