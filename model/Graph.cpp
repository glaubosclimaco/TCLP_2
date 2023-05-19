/*
 * graph.cpp
 *
 *  Created on: 14/10/2017
 *      Author: Glaubos
 */

#include "Graph.h"

Grafo::Grafo(int V) {
	this->V = V;
	adj = new list<int> [V];
	adjFlow = new vector<Edge> [V];
	level = new int[V];
//	visitedTs.resize(V, false);
}
// A recursive function that returns true if there is an articulation
// point in given graph, otherwise returns false.
// This function is almost same as isAPUtil() here ( http://goo.gl/Me9Fw )
// u --> The vertex to be visited next
// visited[] --> keeps tract of visited vertices
// disc[] --> Stores discovery times of visited vertices
// parent[] --> Stores parent vertices in DFS tree
bool Grafo::isBCUtil(int u, bool visited[], int disc[], int low[],
		int parent[]) {
	// A static variable is used for simplicity, we can avoid use of static
	// variable by passing a pointer.
	static int time = 0;

	// Count of children in DFS Tree
	int children = 0;

	// Mark the current node as visited
	visited[u] = true;

	// Initialize discovery time and low value
	disc[u] = low[u] = ++time;

	// Go through all vertices aadjacent to this
	list<int>::iterator i;
	for (i = adj[u].begin(); i != adj[u].end(); ++i) {
		int v = *i;  // v is current adjacent of u

		// If v is not visited yet, then make it a child of u
		// in DFS tree and recur for it
		if (!visited[v]) {
			cout << v << " \t";
			visitedTs[v] = true;
			children++;
			parent[v] = u;

			// check if subgraph rooted with v has an articulation point
			if (isBCUtil(v, visited, disc, low, parent))
				return true;

			// Check if the subtree rooted with v has a connection to
			// one of the ancestors of u
			low[u] = min(low[u], low[v]);

			// u is an articulation point in following cases

			// (1) u is root of DFS tree and has two or more chilren.
			if (parent[u] == NIL && children > 1)
				return true;

			// (2) If u is not root and low value of one of its child is
			// more than discovery value of u.
			if (parent[u] != NIL && low[v] >= disc[u])
				return true;
		}

		// Update low value of u for parent function calls.
		else if (v != parent[u])
			low[u] = min(low[u], disc[v]);
	}
	return false;
}

list<int>*& Grafo::getAdj() {
	return adj;
}

void Grafo::setAdj(list<int>*& adj) {
	this->adj = adj;
}

// The main function that returns true if graph is Biconnected,
// otherwise false. It uses recursive function isBCUtil()
bool Grafo::isBC(int start) {
//	cout << "dentro de isBC \n";
//	print();
//	cin.get();

	// Mark all the vertices as not visited
	bool *visited = new bool[V];
	int *disc = new int[V];
	int *low = new int[V];
	int *parent = new int[V];

	// Initialize parent and visited, and ap(articulation point)
	//  arrays
	for (int i = 0; i < V; i++) {
		parent[i] = NIL;
		visited[i] = false;
	}

	// Call the recursive helper function to find if there is an articulation
	// point in given graph. We do DFS traversal starring from vertex 0
	if (isBCUtil(start, visited, disc, low, parent) == true)
		return false;

	// Now check whether the given graph is connected or not. An undirected
	// graph is connected if all vertices are reachable from any starting
	// point (we have taken 0 as starting point)
	bool r = true;
	for (int i = 0; i < V; i++) {
//		cout << i << " visitado = " << visited[i] << endl;
		if (visited[i] == false) {
			if (r)
				r = false;
		}

	}
	if (!r)
		return false;

	return true;
}

int Grafo::degree(int v) {
	return adj[v].size();
}

void Grafo::addEdge(int u, int v) {
	adj[u].push_back(v); // Add v to u’s list.
	adj[v].push_back(u);  // Note: the graph is undirected
}


void Grafo::remove_edges(list<int> nodes) {
	list<int>::iterator i;
	for (i = nodes.begin(); i != nodes.end(); ++i) {
		int no = *i;
		adj[no].clear();
	}
	list<int>::iterator a;
	for (int j = 0; j < V; j++) {
		for (a = nodes.begin(); a != nodes.end(); a++) {
			int elemento = *a;
			adj[j].remove(elemento);
		}
	}

}

void Grafo::print() {
	cout << "\nPrint Grafo:\n";
	list<int>::iterator j;
	for (int i = 0; i < V; ++i) {
		if (adj[i].size() < 1)
			continue;
		cout << i << ": ";
		for (j = adj[i].begin(); j != adj[i].end(); ++j)
			cout << *j << " ";
		cout << endl;
	}
}

void Grafo::printFlow() {
	cout << "\nPrint Grafo Flow:\n";
	vector<Edge>::iterator j;
	for (int i = 0; i < V; ++i) {
		if (adjFlow[i].size() < 1)
			continue;
		cout << i << ": ";
		for (j = adjFlow[i].begin(); j != adjFlow[i].end(); ++j)
			cout << (*j).v << " (" << (*j).C << ")";
		cout << endl;
	}
}

vector<bool>& Grafo::getVisitedTs() {
	return visitedTs;
}

void Grafo::setVisitedTs(vector<bool>& visitedTs) {
	this->visitedTs = visitedTs;
}

//const list<int>*& Grafo::getAdj() const {
//	return adj;
//}
//
//void Grafo::setAdj(const list<int>*& adj) {
//	this->adj = adj;
//}

void Grafo::setV(int v) {
	V = v;
}

void Grafo::copy(Grafo g) {
	this->V = g.V;
//	this->adj = new list<int> [V];
	for (int i = 0; i < g.V; i++)
		this->adj[i].assign(g.adj[i].begin(), g.adj[i].end());
}

// Prints all paths from 's' to 'd'
void Grafo::path(int s, int d, int path[]) {
// Mark all the Vertexs as not visited
	bool *visited = new bool[V];

	int path_index = 0; // Initialize path[] as empty

// Initialize all Vertexs as not visited
	for (int i = 0; i < V; i++)
		visited[i] = false;

// Call the recursive helper function to print all paths
	cout << "printAllPathsUtil" << endl;
	printAllPathsUtil(s, d, visited, path, path_index);

	cout << "fim path" << endl;
}

// A recursive function to print all paths from 'u' to 'd'.
// visited[] keeps track of Vertexs in current path.
// path[] stores actual Vertexs and path_index is current
// index in path[]
void Grafo::printAllPathsUtil(int u, int d, bool visited[], int path[],
		int &path_index) {
// Mark the current node and store it in path[]
	visited[u] = true;
	path[path_index] = u;
	path_index++;
// If current vertex is same as destination, then print
	// printf("u:%d d:%d\n", u, d);
	// cin.get();
	if (u == d) {
		return;

	} else // If current vertex is not destination
	{
		// Recur for all the Vertexs adjacent to current vertex
		list<int>::iterator i;
		for (i = adj[u].begin(); i != adj[u].end(); ++i)
			if (!visited[*i])
				printAllPathsUtil(*i, d, visited, path, path_index);
	}

// Remove current vertex from path[] and mark it as unvisited
	path_index--;
	visited[u] = false;
}

// A BFS based function to check whether d is reachable from s.
bool Grafo::isReachable(int s, int d, int* pred) {
	// Base case
	if (s == d)
		return true;

	// Mark all the Vertexs as not visited
	bool *visited = new bool[V];
	for (int i = 0; i < V; i++) {
		visited[i] = false;
		pred[i] = -1;
	}

	// Create a queue for BFS
	list<int> queue;

	// Mark the current node as visited and enqueue it
	visited[s] = true;
	queue.push_back(s);

	// it will be used to get all adjacent Vertexs of a vertex
	list<int>::iterator i;
	bool achou = false;
	while (!queue.empty()) {
		// Dequeue a vertex from queue and print it
		s = queue.front();
		queue.pop_front();

		// Get all adjacent Vertexs of the dequeued vertex s
		// If a adjacent has not been visited, then mark it visited
		// and enqueue it

		for (i = adj[s].begin(); i != adj[s].end(); ++i) {
			int v = (*i);
			// If this adjacent node is the destination node, then
			// return true
			if (v == d) {
//				printf("achou \n");
				pred[v] = s;
				return true;
				// achou = true;
				// break;
			}

			// Else, continue to do BFS

			if (!visited[v]) {
				visited[v] = true;
//				printf("%d->%d  ", v, s);
				pred[v] = s;
				queue.push_back(*i);
			}
		}
	}

	// If BFS is complete without visiting d
	return false;
}

//funcao para encontrar  componetes desconexas
void Grafo::connected(vector<Vertex> Vertexs, int& restantes, bool *visited,
		vector<int>& componente) {
	// Mark all the Vertexs as not visited
	int s = -1;
	bool debug = false;

	for (int i = 0; i < V; i++) {
		if (visited[i] == false && Vertexs[i].reduzido == false
				&& Vertexs[i].tipo != 2) {
			s = i;
			break;
		}
	}

	if (s == -1) {
		if (debug)
			printf("nao ha mais componentes conexas");
		return;
	}

	componente.clear();

	if (debug)
		printf("-- Grafo::connected -- \n");
// Create a queue for BFS
	list<int> queue;

// Mark the current node as visited and enqueue it
	if (debug)
		printf("s = %d visitado = %d\n", s, visited[s]);
	if (debug)
		printf("restantes = %d", restantes);
	visited[s] = true;
	queue.push_back(s);
	componente.push_back(s);

// it will be used to get all adjacent Vertexs of a vertex
	list<int>::iterator i;
	while (!queue.empty()) {
		// Dequeue a vertex from queue and print it
		s = queue.front();
		queue.pop_front();

		// Get all adjacent Vertexs of the dequeued vertex s
		// If a adjacent has not been visited, then mark it visited
		// and enqueue it

		for (i = adj[s].begin(); i != adj[s].end(); ++i) {
			int v = (*i);
			if (!visited[v]) {
				visited[v] = true;
				if (debug)
					printf("%d-%d\n", v, s);
				queue.push_back(*i);
				componente.push_back(v);
			}
		}
	}

	if (debug)
		printf("componente size = %lu \n", componente.size());

	restantes = restantes - componente.size();

	//nao visitados
//	for (int i = 0; i < Vertexs.size(); i++) {
//		if (!visited[i] && !Vertexs[i].reduzido && Vertexs[i].tipo != 2)
//			if (debug)
//				printf("%d n foi visitado \n", i);
//	}
}

// Finds if more flow can be sent from s to t.
// Also assigns levels to nodes.
bool Grafo::BFS(int s, int t) {
	for (int i = 0; i < V; i++)
		level[i] = -1;

	level[s] = 0;  // Level of source vertex

	// Create a queue, enqueue source vertex
	// and mark source vertex as visited here
	// level[] array works as visited array also.
	list<int> q;
	q.push_back(s);

	vector<Edge>::iterator i;
	while (!q.empty()) {
		int u = q.front();
		q.pop_front();
		for (i = adjFlow[u].begin(); i != adjFlow[u].end(); i++) {
			Edge &e = *i;
			if (level[e.v] < 0 && e.flow < e.C) {
				// Level of current vertex is,
				// level of parent + 1
				level[e.v] = level[u] + 1;

				q.push_back(e.v);
			}
		}
	}

	// IF we can not reach to the sink we
	// return false else true
	return level[t] < 0 ? false : true;
}

// A DFS based function to send flow after BFS has
// figured out that there is a possible flow and
// constructed levels. This function called multiple
// times for a single call of BFS.
// flow : Current flow send by parent function call
// start[] : To keep track of next edge to be explored.
//           start[i] stores  count of edges explored
//           from i.
//  u : Current vertex
//  t : Sink
int Grafo::sendFlow(int u, int flow, int t, int start[]) {
	// Sink reached
	if (u == t)
		return flow;

	// Traverse all adjacent edges one -by - one.
	for (; start[u] < adjFlow[u].size(); start[u]++) {
		// Pick next edge from adjacency list of u
		Edge &e = adjFlow[u][start[u]];

		if (level[e.v] == level[u] + 1 && e.flow < e.C) {
			// find minimum flow from u to t
			int curr_flow = min(flow, e.C - e.flow);

			int temp_flow = sendFlow(e.v, curr_flow, t, start);

			// flow is greater than zero
			if (temp_flow > 0) {
				// add flow  to current edge
				e.flow += temp_flow;

				// subtract flow from reverse edge
				// of current edge
				adjFlow[e.v][e.rev].flow -= temp_flow;
				return temp_flow;
			}
		}
	}

	return 0;
}

// Returns maximum flow in graph
int Grafo::DinicMaxflow(int s, int t) {
	// Corner case
	if (s == t)
		return -1;

	int total = 0;  // Initialize result

	// Augment the flow while there is path
	// from source to sink
	while (BFS(s, t) == true) {
		// store how many edges are visited
		// from V { 0 to V }
		int *start = new int[V + 1];

		// while flow is not zero in graph from S to D
		while (int flow = sendFlow(s, INT_MAX, t, start))

			// Add path flow to overall flow
			total += flow;
	}

	// return maximum flow
	return total;
}
