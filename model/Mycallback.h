/*
 * Mycallback.h
 *
 *  Created on: 06/10/2017
 *      Author: user
 */

#include "gurobi_c++.h"
#include "utils.h"
#include <iostream>
#include <cstdlib>
//#include <fstream>
#include <sstream>
#include <algorithm>    // std::sort
#include <vector>       // std::vector
#include <ctime>
#include <utility>      // std::pair
#include <math.h>

//using namespace std;

class Mycallback: public GRBCallback {
public:

	int n;
	GRBVar **vars;
	double **matriz_custo;
	int r;
	vector<Vertex> Vertexs;
	GRBVar* varsY;
	GRBModel *model;
	double prize;

	Mycallback(GRBVar **xvars, int _r, int qnd_n, double **_matriz_custo,
			std::vector<Vertex> _Vertexs, GRBVar* _y, GRBModel *m,
			double _prize) {
		vars = xvars;
		n = qnd_n;
		matriz_custo = _matriz_custo;
		r = _r;
		Vertexs = _Vertexs;
		varsY = _y;
		model = m;
		prize = _prize;
		// = _model;
	}

protected:

//	void fischetti2Matching(bool debug) {
//		GRBLinExpr lhs = 0, rhs = 0;
//		double **x = new double *[n];
//		int *grafoRL;
//		grafoRL = new int[n];
//		//		bool debug = false;
//		unsigned tamE = 0, tamV = 0;
//		vector<aresta> arestas;
//		conj2M agm, handle, grafo;
//		agm.adj.resize(n); //alocando n posicoes para n vertices
//		grafo.adj.resize(n);
//		no raiz;
//		raiz.id = r;
//
//		//		printf("\n---------------- 2-matching -----------------\n\n");
//
//		for (int i = 0; i < n; i++) {
//			x[i] = new double[n];
//			grafoRL[i] = 0;
//		}
//
//		for (int i = 0; i < n; i++) {
//			if (Vertexs[i].reduzido || Vertexs[i].tipo == 2)
//				continue;
//			for (int j = i + 1; j < n; j++) {
//				if (Vertexs[j].reduzido || Vertexs[j].tipo == 2)
//					continue;
//				x[i][j] = getNodeRel(vars[i][j]);
//				// if (debug)  printf("x_%d_%d=%.2f \t", i, j, x[i][j]);
//				if (x[i][j] >= 1e-5) {
//					// if (debug)
//					// 	printf("x_%d(%d)_%d(%d)=%.2f \n", i, Vertexs[i].tipo,
//					// 			j, Vertexs[j].tipo, x[i][j]);
//					//					printf("(\"%d\", \"%d\"), ", i, j);
//
//					aresta umaAresta;
//					no o, d;
//					if (i < j) {
//						o.id = i;
//						d.id = j;
//					} else {
//						o.id = j;
//						d.id = i;
//					}
//
//					if (grafoRL[i] != 1) {
//						tamV++;
//						grafo.nos.push_back(o);
//					}
//					grafoRL[i] = 1;
//					if (grafoRL[j] != 1) {
//						tamV++;
//						grafo.nos.push_back(d);
//					}
//					grafoRL[j] = 1;
//
//					umaAresta.o = o;
//					umaAresta.d = d;
//					umaAresta.custo = x[i][j];
//					arestas.push_back(umaAresta);
//					grafo.adj[o.id].push_back(d);
//					grafo.adj[d.id].push_back(o);
//					//					printf("aqui 1
//
//					tamE++;
//				}
//			}
//		}
//
//		//		cout << "tam V: " << tamV << endl;
//
//		//ordenar as arestas de acordo com seus "custos"
////		sort(arestas.begin(), arestas.end(), myfunctionAresta);
//
//		grafo.arestas.assign(arestas.begin(), arestas.end());
//
//		if (debug) {
//			cout << "Grafo: " << endl;
//			printArestas(grafo.arestas, true);
//		}
//		//		cout << "aresta's size: " << arestas.size() << endl;
//
//		unsigned cont = 0;
//		bool noAdicionado[n] = { false };
//		agm.nos.push_back(raiz);
//
//		//enquanto 'arestas' nao for vazio
//		while (agm.arestas.size() < tamV - 1 and cont < arestas.size()) {
//			//			printf("cont: %d \n", cont);
//			//detectar se a adicao de arestas[0] vai formar um ciclo
//
//			vector<no>::iterator it;
//			for (it = grafo.adj[a.id].begin(); it != grafo.adj[a.id].end();
//					++it) {
//			}
//		}
//
//		aresta arestaAdd = arestas[cont];
//		string log = "";
//
//		bool formaCiclo = cicla(agm, arestaAdd);
//
//		cont++;
//
//		if (formaCiclo)
//			continue;
//
//		//adicionando aresta na agm
//		agm.arestas.push_back(arestaAdd);
//
//		//adicionando nos na agm
//		unsigned oID = arestaAdd.o.id;
//		if (!noAdicionado[oID]) {
//			agm.nos.push_back(arestaAdd.o);
//			noAdicionado[oID] = true;
//
//		}
//		agm.adj[oID].push_back(arestaAdd.d);
//
//		unsigned dID = arestaAdd.d.id;
//		if (!noAdicionado[dID]) {
//			agm.nos.push_back(arestaAdd.d);
//			noAdicionado[dID] = true;
//
//		}
//		agm.adj[dID].push_back(arestaAdd.o);
//
//		if (debug) {
//			printf("\nAGM:\n");
//			//			printf("[");
//			//				for (unsigned i = 0; i < agm.arestas.size(); i++) {
//			//					printf("(\"%d\", \"%d\"), ", agm.arestas[i].o.id,
//			//							agm.arestas[i].d.id);
//			//				}
//			printArestas(agm.arestas, true);
//		}
//
//		//			arestas.erase(arestas.begin());
//
//		//			cout << "agms arestas size: " << agm.arestas.size() << endl;
//		//			cout << "areasta size: " << arestas.size() << endl;
////			if (debug) {
////				cout << "\nHandle:" << endl;
////			}
////			double sumHandle = 0.0;
////			string lhsTemp = "";
////			lhs = 0;
////			handle = bfs2Matching(agm, arestaAdd, n, x, sumHandle, debug);
////			if (debug) {
////				cout << "\nx(H): " << sumHandle << endl;
////			}
//		vector<aresta> teeths;
//		double sumTeeths = 0.0;
//
//		//formando os teeths
//		for (unsigned i = 0; i < agm.nos.size(); i++) {
//			no a = agm.nos[i];
//			//				cout << "\nverificando teeths ligados a " << a.id << endl;
//			//				cout << "possui " << grafo.adj[a.id].size()
//			//						<< " incidentes no grafo" << endl;
//			vector<no>::iterator it;
//			for (it = grafo.adj[a.id].begin(); it != grafo.adj[a.id].end();
//					++it) {
//				no b = (*it);
//				//					cout << "\n verificando " << a.id << " e " << b.id << endl;
//				//					cin.get();
//				bool bInHandle = false;
//				for (unsigned k = 0; k < agm.nos.size(); k++) {
//					if (agm.nos[k].id == b.id) {
//						bInHandle = true;
////							if (a.id < b.id) {
////								lhs += vars[a.id][b.id];
////							} else {
////								lhs += vars[a.id][b.id];
////							}
//
//						break;
//					}
//				}
//
//				if (bInHandle) {
//					continue;
//				}
//				//
//				aresta arestaTemp;
//				arestaTemp.o = a;
//				arestaTemp.d = b;
//
//				if (a.id < b.id)
//					arestaTemp.custo = x[a.id][b.id];
//				else
//					arestaTemp.custo = x[b.id][a.id];
//
//				teeths.push_back(arestaTemp);
//				sumTeeths += arestaTemp.custo;
//				//
//			}
//		}
//		//
//		//ordenando os teehts de acordo com seu custo (ordem decrescente)
//		sort(teeths.begin(), teeths.end(), myfunctionArestaDecresc);
//
//		unsigned tamTeeths = teeths.size();
//
//		if (debug) {
//			cout << "Teeths: ";
//			printArestas(teeths, true);
//			cout << "x(T): " << sumTeeths << endl;
//			cout << "|T|: " << tamTeeths << endl;
//		}
//
//		//testando se |T| é maior que 3, e se |T| é ímpar
//		if (tamTeeths < 3 or tamTeeths % 2 == 0) {
//			continue;
//		}
//
//		if (debug)
//			printf("T impar e > 2 \n");
//		//escolhendo os teeths de maneira ótima
//
//		GRBEnv env = GRBEnv();
//		env.set(GRB_IntParam_Threads, 1);
//		env.set(GRB_IntParam_OutputFlag, 0);
//
//		GRBModel model = GRBModel(env);
//
//		GRBVar *z = 0; //representa os teehths escolhidos
//		//			GRBVar **x = 0; //representa os teeths
//		char varName[100];
//		GRBLinExpr obj;
//
//		z = new GRBVar[n];
//		for (unsigned i = 0; i < tamTeeths - 1; i++) {
//			//				cont++;
//			if (i == 0) {
//				sprintf(varName, "z_%d.%d", teeths[i].o.id, teeths[i].d.id);
//				double valorA1 = -1.0 * teeths[i].custo;
//				z[i] = model.addVar(0.0, 1.0, valorA1, GRB_BINARY, varName);
//
//				//					lhs += vars[teeths[i].o.id][teeths[i].d.id];
//				//				lhsTemp += " + x_" + itos(teeths[i].o.id) + "_"
//				//						+ itos(teeths[i].d.id);
//				if (teeths[i].o.id < teeths[i].d.id) {
//					lhs += vars[teeths[i].o.id][teeths[i].d.id];
//				} else {
//					lhs += vars[teeths[i].d.id][teeths[i].o.id];
//				}
//			} else {
//				//					if (cont == 2) {
//				sprintf(varName, "z_%d.%d_%d.%d", teeths[i].o.id,
//						teeths[i].d.id, teeths[i + 1].o.id, teeths[i + 1].d.id);
//				double valor1;
//				valor1 = -1 * (teeths[i].custo + teeths[i + 1].custo - 1);
//				z[i] = model.addVar(0.0, 1.0, valor1, GRB_BINARY, varName);
//
//				//					lhs += vars[teeths[i].o.id][teeths[i].d.id];
//				//					lhs += vars[teeths[i + 1].o.id][teeths[i + 1].d.id];
//
//				//				lhsTemp += " + x_" + itos(teeths[i].o.id) + "_"
//				//						+ itos(teeths[i].d.id);
//
//				if (teeths[i].o.id < teeths[i].d.id) {
//					lhs += vars[teeths[i].o.id][teeths[i].d.id];
//				} else {
//					lhs += vars[teeths[i].d.id][teeths[i].o.id];
//				}
//
//				//				lhsTemp += " + x_" + itos(teeths[i + 1].o.id) + "_"
//				//						+ itos(teeths[i + 1].d.id);
//
//				if (teeths[i + 1].o.id < teeths[i + 1].d.id) {
//					lhs += vars[teeths[i + 1].o.id][teeths[i + 1].d.id];
//				} else {
//					lhs += vars[teeths[i + 1].d.id][teeths[i + 1].o.id];
//				}
//
//				i++;
//				//						cont = 0;
//				//					} else {
//				//						double valor;
//				//						valor = -1 * teeths[i].custo;
//				//						z[i] = model.addVar(0.0, 1.0, valor, GRB_BINARY,
//				//								varName);
//			}
//			//					cout << z[i].get(GRB_StringAttr_VarName) << "("
//			//							<< teeths[i].custo << " + " << teeths[i + 1].custo
//			//							<< " + -1)";
//			//				}
//		}
//
//		//			cin.get();
//
//		//			cout << endl;
//
//		model.update();
//
//		//			model.write("2matching.lp");
//		model.optimize();
//
//		//mostrando as arestas escolhidas
//
//		//			sumTeeths = 0.0;
//		//			int nTeeths = 0;
//		//
//		//			for (unsigned i = 0; i < tamTeeths - 1; i++) {
//		//				if (i == 0) {
//		//					if (debug) {
//		//						cout << z[i].get(GRB_StringAttr_VarName) << " = "
//		//								<< z[i].get(GRB_DoubleAttr_X) << "\t -> "
//		//								<< teeths[i].o.id << "_" << teeths[i].d.id
//		//								<< endl;
//		//					}
//		//					if (z[i].get(GRB_DoubleAttr_X) == 1) {
//		//						sumTeeths += teeths[i].custo;
//		//						nTeeths++;
//		//					}
//		//				} else {
//		//					if (debug) {
//		//						cout << z[i].get(GRB_StringAttr_VarName) << " = "
//		//								<< z[i].get(GRB_DoubleAttr_X) << "\t -> "
//		//								<< teeths[i].o.id << "_" << teeths[i].d.id
//		//								<< " e " << teeths[i + 1].o.id << "_"
//		//								<< teeths[i + 1].d.id << endl;
//		//					}
//		//					if (z[i].get(GRB_DoubleAttr_X) == 1) {
//		//						sumTeeths += teeths[i].custo + teeths[i + 1].custo;
//		//						nTeeths++;
//		//						nTeeths++;
//		//					}
//		//
//		//					i++;
//		//
//		//				}
//		//
//		//			}
//
//		//dados da 2matching
//		double tamHandle = (double) handle.nos.size();
//
//		if (debug) {
//			printf("\nsoma handle = %f \n", sumHandle);
//			printf("soma teeths = %f \n", sumTeeths);
//			printf("tam do handle = %f \n", tamHandle);
//			printf("qnt de teeths = %d \n", tamTeeths);
//			printf("lhs Sum = %f \n", sumHandle + sumTeeths);
//			printf("rhs Sum = %f \n", tamHandle + floor(tamTeeths / 2));
//			//				printf("%.2f + %.2f > %.2f + %f ? \n", sumHandle, sumTeeths,
//			//						handle.nos.size(), floor(nTeeths / 2));
//			cout << sumHandle << " + " << sumTeeths << " > " << tamHandle
//					<< " + " << floor(tamTeeths / 2) << endl;
//		}
//
//		//			if (debug)
//		//				getchar();
//
//		//verificando violacao
//		long double lhsSum = sumHandle + sumTeeths;
//		double piso = floor(tamTeeths / 2);
//		long double rhsSum = tamHandle + piso;
//		//			printf("%le > %le ? \n", lhsSum, rhsSum);
//
//		lhsSum += 1E-5;
//		if ((lhsSum - rhsSum) > 0.0001) {
//			cout << lhsSum - rhsSum << endl;
//			printf("VIOLADO !!! \n");
//
//			//				getchar();
//			//corte
//
//			rhs = handle.nos.size() + floor(tamTeeths / 2);
//			if (debug) {
//				cout << "corte 2-Matching: ";
//				cout << lhsTemp << "<=" << rhs << endl;
//			}
//			//				cout << "lhs: " << lhs << endl;
//			//				cout << "rhs: " << rhs << endl;
//
//			addCut(lhs <= rhs);
//			lhs = rhs = 0;
//			twoMatching_cuts++;
//		}
//
//	}
//}

	void kruskal2Matching(bool debug) {
//		clock_t begin = clock();
		GRBLinExpr lhs = 0, rhs = 0;
		double **x = new double *[n];
		int *grafoRL;
		grafoRL = new int[n];
		//		bool debug = false;
		unsigned tamE = 0, tamV = 0;
		vector<aresta> arestas;
		conj2M agm, handle, grafo;
		agm.adj.resize(n); //alocando n posicoes para n vertices
		grafo.adj.resize(n);
//		double yH = 0.0;
		double *y;

		y = new double[n];
		//		printf("\n---------------- 2-matching -----------------\n\n");

		for (int i = 0; i < n; i++) {
			x[i] = new double[n];
			grafoRL[i] = 0;
			y[i] = 0.0;
			if (Vertexs[i].reduzido || Vertexs[i].tipo == 2)
				continue;
			y[i] = getNodeRel(varsY[i]);
		}

		for (int i = 0; i < n; i++) {
			if (Vertexs[i].reduzido || Vertexs[i].tipo == 2)
				continue;
			for (int j = i + 1; j < n; j++) {
				if (Vertexs[j].reduzido || Vertexs[j].tipo == 2)
					continue;
				x[i][j] = getNodeRel(vars[i][j]);
				// if (debug)  printf("x_%d_%d=%.2f \t", i, j, x[i][j]);
				if (x[i][j] > 0) {
					// if (debug)
					// 	printf("x_%d(%d)_%d(%d)=%.2f \n", i, Vertexs[i].tipo,
					// 			j, Vertexs[j].tipo, x[i][j]);
					//					printf("(\"%d\", \"%d\"), ", i, j);

					aresta umaAresta;
					no o, d;
					if (i < j) {
						o.id = i;
						d.id = j;
					} else {
						o.id = j;
						d.id = i;
					}

					if (grafoRL[i] != 1) {
						tamV++;
						grafo.nos.push_back(o);
					}
					grafoRL[i] = 1;
					if (grafoRL[j] != 1) {
						tamV++;
						grafo.nos.push_back(d);
					}
					grafoRL[j] = 1;

					umaAresta.o = o;
					umaAresta.d = d;
					umaAresta.custo = x[i][j];
					arestas.push_back(umaAresta);
					grafo.adj[o.id].push_back(d);
					grafo.adj[d.id].push_back(o);
					//					printf("aqui 1

					tamE++;
				}
			}
		}

		//		cout << "tam V: " << tamV << endl;

		//ordenar as arestas de acordo com seus "custos"
		sort(arestas.begin(), arestas.end(), myfunctionAresta);

		grafo.arestas.assign(arestas.begin(), arestas.end());

//		if (debug) {
//			cout << "Grafo: " << endl;
//			printArestas(grafo.arestas, true);
//		}
		//		cout << "aresta's size: " << arestas.size() << endl;

		unsigned cont = 0;
		bool noAdicionado[n] = { false };

		//enquanto 'arestas' nao for vazio
		while (agm.arestas.size() < tamV - 1 and cont < arestas.size()) {
			//			printf("cont: %d \n", cont);
			//detectar se a adicao de arestas[0] vai formar um ciclo
			aresta arestaAdd = arestas[cont];
			string log = "";

			bool oIn = false, dIn = false, formaCiclo = false;
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
					formaCiclo = true;
					break;
				}

			}

//			bool formaCiclo = cicla(agm, arestaAdd);

			cont++;

			if (formaCiclo)
				continue;

			//adicionando aresta na agm
			agm.arestas.push_back(arestaAdd);

			//adicionando nos na agm
			unsigned oID = arestaAdd.o.id;
			if (!noAdicionado[oID]) {
				agm.nos.push_back(arestaAdd.o);
				noAdicionado[oID] = true;

			}
			agm.adj[oID].push_back(arestaAdd.d);

			unsigned dID = arestaAdd.d.id;
			if (!noAdicionado[dID]) {
				agm.nos.push_back(arestaAdd.d);
				noAdicionado[dID] = true;

			}
			agm.adj[dID].push_back(arestaAdd.o);

			if (debug) {
				printf("\nAGM:\n");
				//			printf("[");
				//				for (unsigned i = 0; i < agm.arestas.size(); i++) {
				//					printf("(\"%d\", \"%d\"), ", agm.arestas[i].o.id,
				//							agm.arestas[i].d.id);
				//				}
				printArestas(agm.arestas, true);
			}

			//			arestas.erase(arestas.begin());

			//			cout << "agms arestas size: " << agm.arestas.size() << endl;
			//			cout << "areasta size: " << arestas.size() << endl;
//			if (debug) {
//				cout << "\nHandle:" << endl;
//			}
			double sumHandle = 0.0;
			string lhsTemp = "";

			handle = bfs2Matching(agm, arestaAdd, n, x, sumHandle, debug);
//			if (debug) {
//				cout << "\nx(H): " << sumHandle << endl;
//			}

			//formando lhs do handle:
			lhs = 0;
			for (unsigned i = 0; i < handle.arestas.size(); i++) {
				no a, b;
				a = handle.arestas[i].o;
				b = handle.arestas[i].d;
				if (a.id < b.id) {
					lhs += vars[a.id][b.id];
//					lhsTemp += " + x_" + itos(a.id) + "_" + itos(b.id);
				} else {
					lhs += vars[b.id][a.id];
//					lhsTemp += " + x_" + itos(b.id) + "_" + itos(a.id);
				}

			}

			vector<aresta> teeths;
			double sumTeeths = 0.0;

			//formando os teeths
//			yH = 0;
			for (unsigned i = 0; i < handle.nos.size(); i++) {
				no a = handle.nos[i];
//				yH += y[a.id];
				//				cout << "\nverificando teeths ligados a " << a.id << endl;
				//				cout << "possui " << grafo.adj[a.id].size()
				//						<< " incidentes no grafo" << endl;
				vector<no>::iterator it;
				for (it = grafo.adj[a.id].begin(); it != grafo.adj[a.id].end();
						++it) {
					no b = (*it);
					//					cout << "\n verificando " << a.id << " e " << b.id << endl;
					//					cin.get();
					bool bInHandle = false;
					for (unsigned k = 0; k < handle.nos.size(); k++) {
						if (handle.nos[k].id == b.id) {
							bInHandle = true;
//						if (a.id < b.id) {
//							lhs += vars[a.id][b.id];
//							lhsTemp += " + x_" + itos(a.id) + "_" + itos(b.id);
//						} else {
//							lhs += vars[b.id][a.id];
//							lhsTemp += " + x_" + itos(b.id) + "_" + itos(a.id);
//						}
							break;
						}
					}

					if (bInHandle)
						continue;
					//
					aresta arestaTemp;
					if (a.id < b.id) {
						arestaTemp.o = a;
						arestaTemp.d = b;
						arestaTemp.custo = x[a.id][b.id];

					} else {
						arestaTemp.o = b;
						arestaTemp.d = a;
						arestaTemp.custo = x[b.id][a.id];
					}

					teeths.push_back(arestaTemp);
					sumTeeths += arestaTemp.custo;
					//
				}
			}
			//
			//ordenando os teehts de acordo com seu custo (ordem decrescente)
			sort(teeths.begin(), teeths.end(), ArestaDecresc);

			unsigned tamTeeths = teeths.size();

//			if (debug) {
//				cout << "Teeths: ";
//				printArestas(teeths, true);
//				cout << "x(T): " << sumTeeths << endl;
//				cout << "|T|: " << tamTeeths << endl;
//				cout << "lhs: " << lhs << endl;
//			}

			//testando se |T| é maior que 3, e se |T| é ímpar
			if (tamTeeths < 3 or tamTeeths % 2 == 0) {
				continue;
			}

//			if (debug)
//				printf("T impar e > 2 \n");
			//escolhendo os teeths de maneira ótima

			GRBEnv env = GRBEnv();
			env.set(GRB_IntParam_Threads, 1);
			env.set(GRB_IntParam_OutputFlag, 0);

			GRBModel model = GRBModel(env);

			GRBVar *z = 0; //representa os teehths escolhidos
			//			GRBVar **x = 0; //representa os teeths
			char varName[100];
			GRBLinExpr obj;

			z = new GRBVar[n];
			for (unsigned i = 0; i < tamTeeths - 1; i++) {
				//				cont++;
				if (i == 0) {
					sprintf(varName, "z_%d.%d", teeths[i].o.id, teeths[i].d.id);
					double valorA1 = -1.0 * teeths[i].custo;
					z[i] = model.addVar(0.0, 1.0, valorA1, GRB_BINARY, varName);

//					lhs += vars[teeths[i].o.id][teeths[i].d.id];
					//				lhsTemp += " + x_" + itos(teeths[i].o.id) + "_"
					//						+ itos(teeths[i].d.id);
//					if (teeths[i].o.id < teeths[i].d.id) {
//						lhs += vars[teeths[i].o.id][teeths[i].d.id];
//					} else {
//						lhs += vars[teeths[i].d.id][teeths[i].o.id];
//					}
				} else {
					//					if (cont == 2) {
					sprintf(varName, "z_%d.%d_%d.%d", teeths[i].o.id,
							teeths[i].d.id, teeths[i + 1].o.id,
							teeths[i + 1].d.id);
					double valor1;
					valor1 = -1 * (teeths[i].custo + teeths[i + 1].custo - 1);
					z[i] = model.addVar(0.0, 1.0, valor1, GRB_BINARY, varName);

//					lhs += vars[teeths[i].o.id][teeths[i].d.id];
//					lhs += vars[teeths[i + 1].o.id][teeths[i + 1].d.id];

//									lhsTemp += " + x_" + itos(teeths[i].o.id) + "_"
//											+ itos(teeths[i].d.id);

//					if (teeths[i].o.id < teeths[i].d.id) {
//						lhs += vars[teeths[i].o.id][teeths[i].d.id];
//					} else {
//						lhs += vars[teeths[i].d.id][teeths[i].o.id];
//					}
//
//									lhsTemp += " + x_" + itos(teeths[i + 1].o.id) + "_"
//											+ itos(teeths[i + 1].d.id);

//					if (teeths[i + 1].o.id < teeths[i + 1].d.id) {
//						lhs += vars[teeths[i + 1].o.id][teeths[i + 1].d.id];
//					} else {
//						lhs += vars[teeths[i + 1].d.id][teeths[i + 1].o.id];
//					}

					i++;

				}
			}

			model.update();

			//			model.write("2matching.lp");
			model.optimize();

			//mostrando as arestas escolhidas

			sumTeeths = 0.0;
			int nTeeths = 0;

			for (unsigned i = 0; i < tamTeeths - 1; i++) {
				if (i == 0) {
//					if (debug) {
//						cout << z[i].get(GRB_StringAttr_VarName) << " = "
//								<< z[i].get(GRB_DoubleAttr_X) << "\t -> "
//								<< teeths[i].o.id << "_" << teeths[i].d.id
//								<< endl;
//					}
					if (z[i].get(GRB_DoubleAttr_X) == 1) {
						sumTeeths += teeths[i].custo;
						nTeeths++;
						if (teeths[i].o.id < teeths[i].d.id) {
							lhs += vars[teeths[i].o.id][teeths[i].d.id];
						} else {
							lhs += vars[teeths[i].d.id][teeths[i].o.id];
						}
					}
				} else {
//					if (debug) {
//						cout << z[i].get(GRB_StringAttr_VarName) << " = "
//								<< z[i].get(GRB_DoubleAttr_X) << "\t -> "
//								<< teeths[i].o.id << "_" << teeths[i].d.id
//								<< " e " << teeths[i + 1].o.id << "_"
//								<< teeths[i + 1].d.id << endl;
//					}
					if (z[i].get(GRB_DoubleAttr_X) == 1) {
						sumTeeths += teeths[i].custo + teeths[i + 1].custo;
						nTeeths++;
						nTeeths++;

						if (teeths[i].o.id < teeths[i].d.id) {
							lhs += vars[teeths[i].o.id][teeths[i].d.id];
//							lhsTemp += " + x_" + itos(teeths[i].o.id) + "_"
//									+ itos(teeths[i].d.id);
						} else {
							lhs += vars[teeths[i].d.id][teeths[i].o.id];
//							lhsTemp += " + x_" + itos(teeths[i].d.id) + "_"
//									+ itos(teeths[i].o.id);
						}

//						lhsTemp += " + x_" + itos(teeths[i + 1].o.id) + "_"
//								+ itos(teeths[i + 1].d.id);
//
						if (teeths[i + 1].o.id < teeths[i + 1].d.id) {
							lhs += vars[teeths[i + 1].o.id][teeths[i + 1].d.id];
//							lhsTemp += " + x_" + itos(teeths[i + 1].o.id) + "_"
//									+ itos(teeths[i + 1].d.id);
						} else {
							lhs += vars[teeths[i + 1].d.id][teeths[i + 1].o.id];

//							lhsTemp += " + x_" + itos(teeths[i + 1].d.id) + "_"
//									+ itos(teeths[i + 1].o.id);
						}

					}

					i++;

				}

			}

			//dados da 2matching
			double tamHandle = (double) handle.nos.size();

//			if (debug) {
//				printf("\nx(H) = %f \n", sumHandle);
//				printf("x(T) = %f \n", sumTeeths);
//				printf("|H| = %f \n", tamHandle);
//				printf("|T| = %d \n", nTeeths);
//				printf("lhs Sum = %f \n", sumHandle + sumTeeths);
//				printf("rhs Sum = %f \n", tamHandle + floor(nTeeths / 2));
//				//				printf("%.2f + %.2f > %.2f + %f ? \n", sumHandle, sumTeeths,
//				//						handle.nos.size(), floor(nTeeths / 2));
//				cout << sumHandle << " + " << sumTeeths << " > " << tamHandle
//						<< " + " << floor(nTeeths / 2) << endl;
//			}

			//			if (debug)
			//				getchar();

			//verificando violacao
			long double lhsSum = sumHandle + sumTeeths;
			double piso = floor(nTeeths / 2);
			long double rhsSum = tamHandle + piso;
			//			printf("%le > %le ? \n", lhsSum, rhsSum);

//			lhsSum += 1E-10;

			if ((lhsSum - rhsSum) > 0.0) {
//				cout << lhsSum - rhsSum << endl;
//				printf("VIOLADO !!! \n");
//				cout << "lhs temp: " << lhsTemp << endl;
//				cout << "lhs temp: " << lhsTemp << endl;
//				cout << lhs << endl;
//				getchar();
				//corte

				rhs = tamHandle + floor(nTeeths / 2);
//				if (debug) {
//					cout << "corte 2-Matching: ";
//					cout << lhs << "<=" << rhs << endl;
//				}
				//				cout << "lhs: " << lhs << endl;
				//				cout << "rhs: " << rhs << endl;

				addCut(lhs <= rhs);
				lhs = rhs = 0;
				twoMatching_cuts++;
			}

		}
//		clock_t end = clock();
//		double elapsed_secs = double(end - begin) / CLOCKS_PER_SEC;
//		printf("tempo 2-M: %f \n", elapsed_secs);
	}

	/*
	 * a new cut set for cost of a new solution and collected prize so far
	 */
	void cut_set(bool debug) {
//		unsigned i, j;
		GRBLinExpr lhsY = 0, lhs = 0, rhs = 0;
		double **x = new double *[n];
		int *g_linha;
		g_linha = new int[n];
		vector<int> s, vs;
		typedef ListGraph::Node Node;
		typedef ListGraph::Edge Edge;

		for (int i = 0; i < n; i++) {
			x[i] = new double[n];
			g_linha[i] = 0;
		}

		double obj_node = getDoubleInfo(GRB_CB_MIPNODE_OBJBST);

		for (int i = 0; i < n; i++) {
			// Vertexs[i].id == r, força r a nao estar em S e sim em S_barra
			if (Vertexs[i].reduzido || Vertexs[i].tipo == 2)
				continue;
			for (int j = i + 1; j < n; j++) {
				if (Vertexs[j].reduzido || Vertexs[j].tipo == 2)
					continue;
				x[i][j] = getNodeRel(vars[i][j]);
				// printf("x_%d_%d=%.2f \t", i, j, x[i][j]);
				if (x[i][j] > 0) {
					//					//mostrando S
					// if (debug)
					// printf("x_%d(%d)_%d(%d)=%.2f \n", i, Vertexs[i].tipo,
					//        j, Vertexs[j].tipo, x[i][j]*matriz_custo[i][j]);
					//					soma1 += x[i][j] * matriz_custo[i][j];
					g_linha[i] = 1;
					g_linha[j] = 1;

					// cont_arestas++;
				}
			}
		}

		MinCut alg = MinCut();
		alg.build_graph(g_linha, n, x);
		double valor = 0.0;
		double p_vs = 0.0;
		double obj_kruskal = 0.0;

		// if (debug)
		// 	printf("r: %d (%d)\n", r, Vertexs[r].tipo);

		vs.push_back(r);
		p_vs += Vertexs[r].premio;

		//		printf("\n\np(%d): %.0f  ", r, Vertexs[r].premio);

		for (int i = 0; i < n; i++) {
			if (r == i || g_linha[i] != 1 || Vertexs[i].tipo == 2)
				continue;
			valor = alg.generate_min_cut(r, i); //Calculando Corte Mínimo
			//			if (debug)
			//				printf("valor entre %d e %d (%d) =  %f \n", r, i,
			//						Vertexs[i].tipo, valor);
			if (valor < 2 - 1E-5) {
				s.push_back(i);

				//				lhsY += varsY[i];
			} else {
				//				if (Vertexs[i].tipo == 0) {

				//				printf("p(%d)[%d]: %.0f  ", i, Vertexs[i].tipo,
				//						Vertexs[i].premio);
				p_vs += Vertexs[i].premio;
				vs.push_back(i);
			}
		}

		if (debug) {
			printf("\n\nPRIZE: %f \n", prize);
			printf("vs.size(): %lu \n", vs.size());
			printf("premio V/S: %f \n", p_vs);
		}

		if (vs.size() < 3)
			return; //nao ha ciclo com menos de 3 vertices

		/*
		 * Using kruskal, checks if the current nodes' solution is worst than
		 *  a known solution
		 */

		if (p_vs - prize >= 0.0) { //se o criterio do prize ja for violado, nao precisa checar o custo do ciclo

			ListGraph g;
			vector<a_node> nodes;
			vector<an_edge> edges;

			for (unsigned i = 0; i < vs.size(); i++) {
				a_node ns;
				ns.id = vs[i];
				for (unsigned j = i + 1; j < vs.size(); j++) {
					a_node nt;
					nt.id = vs[j];

					if (x[ns.id][nt.id] <= 0)
						continue;
					ns.node = g.addNode();
					nodes.push_back(ns);
					nt.node = g.addNode();
					an_edge e;
					e.edge = g.addEdge(ns.node, nt.node);
					e.cost = matriz_custo[ns.id][nt.id] * x[ns.id][nt.id];
					// printf("edge: %d-%d  : %f\n", ns.id, nt.id, matriz_custo[ns.id][nt.id]*x[ns.id][nt.id]);
					//				soma2 += e.cost;
					edges.push_back(e);
				}
			}

			//Make the input for the kruskal.
			typedef ListGraph::EdgeMap<int> ECostMap;
			ECostMap edge_cost_map(g);

			// Fill the edge_cost_map.
			for (unsigned i = 0; i < edges.size(); i++) {
				edge_cost_map.set(edges[i].edge, edges[i].cost);
			}

			// Make the map or the vector, which will contain the edges of the minimum
			// spanning tree.

			typedef ListGraph::EdgeMap<bool> EBoolMap;
			EBoolMap tree_map(g);

			vector<Edge> tree_edge_vec;

			// Kruskal with boolmap;
			obj_kruskal = kruskal(g, edge_cost_map, tree_map);

		}
		//não tem uma solucao (ciclo)
		//		if (vs.size() < 3)
		//			return;
		//checando se a cut set é ativada em relação ao prize

		if (p_vs < prize || obj_kruskal > obj_node) {

//			if (debug) {
//				printf("\n\nviolado!!! \n");
//				printf("\nlower bound V/S: %f \n", obj_kruskal);
//				printf("custo atual: %f \n", obj_node);
//			}

			//para nao repetir cutsets
			vector<Vertex> lhs_hash;

			for (unsigned u = 0; u < vs.size(); u++) {
				int a = vs[u];
				for (int v = 0; v < n; v++) {
					if (Vertexs[v].tipo == 2 || Vertexs[v].reduzido) {
						continue;
					}
					std::vector<int>::iterator it;
					it = std::find(vs.begin(), vs.end(), v);
					if (it != vs.end()) {
						continue;
					}
					//for hashing
					lhs_hash.push_back(Vertexs[a]);
					lhs_hash.push_back(Vertexs[v]);
					//
					if (a < v) {
						lhs += vars[a][v];
					} else {
						lhs += vars[v][a];
					}
				}
			}

			//checar cortes repetidos
//			long h = hash(lhs_hash);
			long h;
			// printf("h: %lu \n", h);
			for (unsigned i = 0; i < hash_table_cs.size(); i++) {
				// printf("h:%lu ht[%d]=%lu \n", h,i,hash_table_cs[i]);
				if (h == hash_table_cs[i]) {
					// printf("corte repetido! \n");
					return;
				}
			}
			//

			hash_table_cs.push_back(h);

//			if (debug) {
//				//conjuto VS:
//				printf("\nconjunto vs: \n");
//				for (unsigned i = 0; i < vs.size(); i++) {
//					printf("%d ", vs[i]);
//				}
//				printf("\nprize de vs: %.0f\n", p_vs);
//				printf("\nprize total: %.0f\n", prize);
//				printf("\n");
//				printf("\nconjunto s: \n");
//				for (unsigned i = 0; i < s.size(); i++) {
//					printf("%d ", s[i]);
//				}
//				printf("\n");
//
//				cout << lhs << ">=" << 2.0;
//			}
			addCut(lhs >= 2.0);
			lhs = 0;
			cut_sets++;

			lhs = lhsY = 0;

			delete g_linha;
			for (int i = 0; i < n; i++)
				delete[] x[i];
			delete[] x;
			return;
		}
	}

//	paper: Cutting planes in integer and mixed integer 	programming

	bool lifted_cover_GENDREAU() {
		/*
		 * Obs.: one must apply the prize rule first, so the total prize is reduced to be
		 * achieved on with optional vertices (R)
		 */

		GRBLinExpr lhs = 0, rhs = 0;
		double* y;
		vector<Vertex> sorted;
		vector<Vertex> cover;
		vector<Vertex> extended;
		vector<Vertex> e_cut; //Vertexs in the cut

		//		double p_V_minus_S = 0.0;
		double p_V;
		double p_C;

		y = new double[n];
//		p_V = prize_V;

		for (int i = 0; i < n; i++) {
			if (Vertexs[i].reduzido || Vertexs[i].tipo != 0)
				continue;
			y[i] = getNodeRel(varsY[i]);
			// printf("%.2f(%.0f)\t", y[i], Vertexs[i].premio);
			double a_value = (1 - y[i]) / Vertexs[i].premio;
			// double a_value = y[i];
			Vertexs[i].cover_value = a_value;
			sorted.push_back(Vertexs[i]);
		}

		sort(sorted.begin(), sorted.end(), myfunction);

		// cout << endl;
		// for (int i = 0; i < sorted.size(); i++) {

		// 	cout << sorted[i].cover_value << " ";
		// }

		double max_premio = 0.0;
		bool violated = false;

		//		printf("\nPV antes: %f\n", p_V);

		// printf("Cover: \n");
		for (unsigned i = 0; i < sorted.size(); i++) {
			cover.push_back(sorted[i]);
			extended.push_back(sorted[i]);
			//			printf("p(%d):%.0f   ", sorted[i].id, sorted[i].premio);
			p_V -= sorted[i].premio;
			// lhs += varsY[sorted[i].id];
			p_C += sorted[i].premio;

			if (max_premio < sorted[i].premio) {
				max_premio = sorted[i].premio;
			}

			// sorted.erase(sorted.begin() + i);
			sorted[i].removed = 1;		//flag para auxiliar na formação de Vh0
			if (p_V < prize) {
				violated = true;
				break;
			}
		}

		//		printf("\nPV dps: %f\n", p_V);
		//		printf("PRIZE: %f\n", prize);
		//
		//		printf("\n");
		//		printf("cover size = %lu \n", cover.size());
		//		printf("max_premio = %f \n", max_premio);

		// printf("\nExtensão: \n");

		//extension
		for (unsigned i = 0; i < sorted.size(); i++) {
			if (sorted[i].premio >= max_premio) {
				// printf("p(%d):%.0f \n", sorted[i].id, sorted[i].premio);
				extended.push_back(sorted[i]);
				// lhs += varsY[sorted[i].id];
				sorted.erase(sorted.begin() + i);
			}
		}

		/**
		 Lifting process

		 **/

		//sorting cover in non increasing order
		sort(cover.begin(), cover.end(), non_increasing_prize);

		vector<type_VH> Vh(cover.size());
		type_VH Vh0;

		//filling Vho. Consider sorted as Vh0
		for (unsigned i = 0; i < sorted.size(); i++) {
			if (sorted[i].removed != 1) {
				Vh0.e.push_back(sorted[i]);
			}
		}

		//filling Vh, estarting from h=2
		double ph = 0.0;
		ph = cover[0].premio;

		// printf("cover[0].premio: %f\n", cover[0].premio);

		//no paper o indice inicial é h=2
		for (unsigned i = 1; i <= cover.size() - 1; i++) {
			// printf("\n----------------------------------------\n");
			// printf("h:%d \n", i + 1);
			ph += cover[i].premio;
			// printf("cover[h].id:%d \n \n", cover[i].id);
			for (unsigned j = 0; j < extended.size(); j++) {
				double pv = extended[j].premio;
				double ph1;
				ph1 = ph + cover[i + 1].premio;
				// printf("j: %d\n", j);
				// printf("extended[j].id: %d\n", extended[j].id);
				// printf("cover[i + 1]: %d\n", cover[i + 1].id);
				// printf("ph: %f\n", ph);
				// printf("pv: %f\n", extended[j].premio);
				// printf("ph+1: %f \n", ph1);
				if (ph <= pv and pv < ph1) {
					Vh[i].e.push_back(extended[j]);
					extended[j].inVh = 1;
					// printf("%d inserido em Vh[%d] \n", extended[j].id, i);
				}
			}

			// printf("----------------------------------------\n");
		}

		// filling Vh1
		for (unsigned j = 0; j < extended.size(); j++) {
			if (extended[j].inVh != 1) {
				Vh[0].e.push_back(extended[j]);
			}
		}

		//		printf("cover:\n");
		//
		//		for (unsigned i = 0; i < cover.size(); i++) {
		//			printf("%d ", cover[i].id);
		//		}

		// printf("\nVh0:\n");

		// for (int i = 0; i < Vh0.e.size(); i++) {
		// 	printf("%d ", Vh0.e[i].id);
		// }

		for (unsigned i = 0; i < cover.size() - 1; i++) {
			// printf("\nVh_%d \n", i + 1);
			for (unsigned j = 0; j < Vh[i].e.size(); j++) {
				// printf("%d ", Vh[i].e[j].id);
				lhs += (i + 1) * (1 - varsY[Vh[i].e[j].id]);
				e_cut.push_back(Vh[i].e[j]);
			}
		}

		if (Vh[1].e.size() > 0) {
			//			printf("\nVh2 !!! \n");
			//			cout << lhs << "<=" << cover.size() - 1 << endl;
		}

		//end of lifting

//		cout << "\n" << lhs << "<=" << cover.size() - 1 << endl;

//		cout << "\ncover: " << lhs << endl;
		//checar cortes repetidos
//		long h = hash(e_cut);
//		// printf("h: %lu \n", h);
//		for (unsigned i = 0; i < hash_table.size(); i++) {
//			// printf("h:%lu ht[%d]=%lu \n", h,i,hash_table[i]);
//			if (h == hash_table[i]) {
//				// printf("corte repetido! \n");
//				return (false);
//			}
//		}
		//

		// printf("\nextended cover size = %lu \n", cover.size());
		// printf("sorted size = %lu \n", sorted.size());
		// printf("prize = %f \n", prize);
		// printf("p(C) = %.0f \n", p_C);
		// cout << "p(C\\R) = " << p_V << endl;

//		hash_table.push_back(h);

		// printf("hash_table size: %lu\n", hash_table.size());
		// cout << "violated: " << violated << endl;
		if (!violated)
			return (false);

		cover_cuts++;
		cout << "\nLifted cober" << lhs << "<=" << cover.size() - 1 << endl;
		addCut(lhs <= cover.size() - 1);
		return (true);

	}

	bool cover_cut(bool debug, int criterio) {
		GRBLinExpr lhs = 0, rhs = 0;
		double* y;
		vector<Vertex> sorted;
		vector<Vertex> cover;
//		double p_V_minus_S = 0.0;
		double p_V = 0.0;
		double p_C = 0.0;
		double U = 0.0;
		y = new double[n];
//		p_V = prize_V;

		U = p_V - prize;

		for (int i = 0; i < n; i++) {
			if (Vertexs[i].reduzido || Vertexs[i].tipo != 0)
				continue;
			double a_value = 0.0;
//			p_V += Vertexs[i].premio;
			y[i] = getNodeRel(varsY[i]);
			// printf("%.2f(%.0f)\t", y[i], Vertexs[i].premio);
//			double a_value = (1 - y[i]) / Vertexs[i].premio;
			if (criterio == 1)
				a_value = (y[i]) / Vertexs[i].premio;
			else {
				a_value = (1 - y[i]) / Vertexs[i].premio;
//				printf("foi chamada a 2ª separação \n");
			}
//			double a_value = 1 / Vertexs[i].premio;
//			double a_value = 1 / y[i];
			Vertexs[i].cover_value = a_value;
			sorted.push_back(Vertexs[i]);
		}

		sort(sorted.begin(), sorted.end(), myfunction);

		if (debug) {
			cout << endl;
			for (unsigned i = 0; i < sorted.size(); i++) {
				cout << sorted[i].id << "(" << sorted[i].cover_value << ")   ";
			}
		}

		double max_premio = 0.0;
//		bool violated = false;
		double sumLhs = 0.0;
		double sumRhs = 0.0;
		bool visitado[n] = { false };

		for (unsigned i = 0; i < sorted.size(); i++) {
			cover.push_back(sorted[i]);
			// printf("p(%d):%.0f  ", sorted[0].id, sorted[0].premio);
//			p_V -= sorted[i].premio;
			visitado[i] = true;
			sumLhs += 1 - y[sorted[i].id];
			lhs += 1 - varsY[sorted[i].id];
			p_C += sorted[i].premio;

			if (max_premio < sorted[i].premio) {
				max_premio = sorted[i].premio;
			}

//			sorted.erase(sorted.begin() + i);

			if (p_C > U) {
				break;
			}

		}

		sumRhs = cover.size() - 1;
		rhs = cover.size() - 1;
//		if (debug) {
//			cout << "\np(V): " << p_V << endl;
//			cout << "prize: " << prize << endl;
//			cout << "U: " << U << endl;
//			cout << "p(C): " << p_C << endl;
//			cout << "sum lhs: " << sumLhs << endl;
//			cout << "rhs: " << sumRhs << endl;
//			cout << "cover: " << endl;
//
//			for (unsigned i = 0; i < cover.size(); i++) {
//				cout << cover[i].id << "  ";
//			}
//
//			// printf("\n");
//			// printf("cover size = %lu \n", cover.size());
//			// printf("max_premio = %f \n", max_premio);
//
////			cout << "\ncover:" << lhs << ">=" << 1 << endl;
//		}

		//extension
		for (unsigned i = 0; i < sorted.size(); i++) {
			if (sorted[i].premio >= max_premio and visitado[i] == false) {
				// printf("premio de extensao%.0f \n", sorted[i].premio);
//				cover.push_back(sorted[i]);
				lhs += 1 - varsY[sorted[i].id];
				sumLhs += 1 - y[sorted[i].id];
			}
		}

//		getchar();

//		if (debug) {
//			cout << "\nsum lhs: " << sumLhs << endl;
//			cout << "sum rhs: " << sumRhs << endl;
//		}

		if ((sumRhs - sumLhs) >= 0.0) {
			return (false);
		}

//		if (debug) {
//			printf("\nVIOLADO !!!\n");
//		}
//		//checar cortes repetidos
//		long h;
//		if (debug) {
//			h = hash(cover);
//			// printf("h: %lu \n", h);
//			for (int i = 0; i < hash_table.size(); i++) {
//				// printf("h:%lu ht[%d]=%lu \n", h,i,hash_table[i]);
//				if (h == hash_table[i]) {
//					printf("corte repetido! \n");
//
//				}
//			}
//		}
		//

		// printf("\nextended cover size = %lu \n", cover.size());
		// printf("sorted size = %lu \n", sorted.size());
		// printf("prize = %f \n", prize);
		// printf("p(C) = %.0f \n", p_C);
		// cout << "p(C\\R) = " << p_V << endl;

//		hash_table.push_back(h);
		// printf("hash_table size: %lu\n", hash_table.size());

		cover_cuts++;
		if (debug) {
			cout << "\ncover cut: " << lhs << "<=" << rhs << endl;
		}
		addCut(lhs <= rhs);
		return (true);
	}

// MIP NODE
	bool cut_set_MIPNODE_7(bool debug) {
		int i, j;
		GRBLinExpr lhs = 0, rhs = 0;
		double **x = new double *[n];
		int *g_linha;
		g_linha = new int[n];
		bool S_interseccao_T = false;
		vector<int> sBarra;
		vector<int> s;
		// int cont_arestas = 0;

		if (debug)
			printf("\n---------------- MIP SOL (7) -----------------\n\n");

		for (i = 0; i < n; i++) {
			x[i] = new double[n];
			g_linha[i] = 0;
		}

		double obj = getDoubleInfo(GRB_CB_MIPSOL_OBJ);
		if (debug)
			printf("\nSolução: %f \n\n", obj);

		for (i = 0; i < n; i++) {
			if (Vertexs[i].reduzido || Vertexs[i].tipo == 2)
				continue;
			for (j = i + 1; j < n; j++) {
				if (Vertexs[j].reduzido || Vertexs[j].tipo == 2)
					continue;
				x[i][j] = getNodeRel(vars[i][j]);
				if (x[i][j] > 0.000001) {
					if (debug)
						printf("(\"%d\",\"%d\",\"%f\"),\t", i, j, x[i][j]);

					//	S intersecao com T != {}
					if (!S_interseccao_T) {
						if (i != r && Vertexs[i].tipo == 1) {
							S_interseccao_T = true;
						}
						if (j != r && Vertexs[j].tipo == 1) {
							S_interseccao_T = true;
						}
					}
					g_linha[i] = 1;
					g_linha[j] = 1;

				}
			}
		}

//		if (S_interseccao_T == false) {
//			if (debug) {
//				printf("\n nao ha interseccao entre S e T \n");
//			}
//			return (false);
//		}

		MinCut alg = MinCut();
		alg.build_graph(g_linha, n, x);
		double valor;
		bool violado = false;
//		double soma = 0.0;
//		bool tem_s_barra = false;

		if (debug)
			printf("r: %d (%d)\n", r, Vertexs[r].tipo);

		sBarra.push_back(r);
		int cont_t = 0;
		for (i = 0; i < n; i++) {
			if (r == i || g_linha[i] != 1 || Vertexs[i].tipo == 2)
				continue;
			valor = alg.generate_min_cut(r, i); //Calculando Corte Mínimo
			if (debug)
				printf("valor entre %d e %d (%d) =  %f \n", r, i,
						Vertexs[i].tipo, valor);
			if (valor < 2 - 1e-5) {
				violado = true;
				s.push_back(i);
//				if (Vertexs[i].tipo == 1)
//					cont_t++;
			} else {
				sBarra.push_back(i);
			}
		}

//		if (debug) {
//			printf("\nhá %d Ts\n", cont_t);
//		}

//		if (cont_t == 0) {
//			if (debug)
//				printf("\nS nao contem um T! \n");
//			return false;
//		}

		if (!violado) {
			// 	printf("\n(7) nao violado\n");
			return false;
		}

		// INCIDENTES A S A QUALQUER VertexS FORA DE s.

		for (unsigned u = 0; u < s.size(); u++) {
			int a = s[u];
			for (int v = 0; v < n; v++) {
				if (a == v || Vertexs[v].tipo == 2 || Vertexs[v].reduzido) {
					continue;
				}
				std::vector<int>::iterator it;
				it = std::find(s.begin(), s.end(), v);
				if (it != s.end()) {
					continue;
				}
				if (a < v)
					lhs += vars[a][v];
				else
					lhs += vars[v][a];
			}
		}

//		for (unsigned u = 0; u < s.size(); u++) {
//			int a = s[u];
//			for (unsigned i = 0; i < sBarra.size(); i++) {
//				int v = sBarra[i];
//				if (a < v)
//					lhs += vars[a][v];
//				else
//					lhs += vars[v][a];
//			}
//		}

		if (debug) {
			cout << "numero de Ts: " << cont_t << endl;
			cout << "\nmip sol 7: " << lhs << " >= 2" << endl;
			cout << "\n\nViolado 7 \n" << endl;
		}
		mipsol7++;
		addCut(lhs >= 2.0);
		lhs = 0;
//		cout << "S_interseccao_T: " << S_interseccao_T << endl;
//		cin.get();

		delete g_linha;
		for (i = 0; i < n; i++)
			delete[] x[i];
		delete[] x;

		// if (debug)
		// 	printf(
		// 			"\n\n--------------------------------------------------------\n");

		return true;
	}

//MIP SOL
	bool cut_set_MIPSOL_7(bool debug) {
		int i, j;
		GRBLinExpr lhs = 0, rhs = 0;
		double **x = new double *[n];
		int *g_linha;
		g_linha = new int[n];
		bool S_interseccao_T = false;
		vector<int> sBarra;
		vector<int> s;
		// int cont_arestas = 0;

		if (debug)
			printf("\n---------------- MIP SOL (7) -----------------\n\n");

		for (i = 0; i < n; i++) {
			x[i] = new double[n];
			g_linha[i] = 0;
		}

		double obj = getDoubleInfo(GRB_CB_MIPSOL_OBJ);
		double bb = getDoubleInfo(GRB_CB_MIPSOL_OBJBND);
		if (debug) {
			printf("\nSolução: %f ", obj);
			printf("\nBeste bound: %f \n", bb);

		}
		for (i = 0; i < n; i++) {
			if (Vertexs[i].reduzido || Vertexs[i].tipo == 2)
				continue;
			for (j = i + 1; j < n; j++) {
				if (Vertexs[j].reduzido || Vertexs[j].tipo == 2)
					continue;
				x[i][j] = getSolution(vars[i][j]);

				if (x[i][j] > 0.000001) {
					if (debug)
						printf("(\"%d\",\"%d\",\"%f\"),\t", i, j, x[i][j]);

					//	S intersecao com T != {}
					if (!S_interseccao_T) {
						if (i != r && Vertexs[i].tipo == 1) {
							S_interseccao_T = true;
						}
						if (j != r && Vertexs[j].tipo == 1) {
							S_interseccao_T = true;
						}
					}
					g_linha[i] = 1;
					g_linha[j] = 1;

				}
			}
		}

//		if (S_interseccao_T == false) {
//			if (debug) {
//				printf("\n nao ha interseccao entre S e T \n");
//			}
//			return (false);
//		}

		MinCut alg = MinCut();
		alg.build_graph(g_linha, n, x);
		double valor;
		bool violado = false;
//		double soma = 0.0;
//		bool tem_s_barra = false;

		if (debug)
			printf("r: %d (%d)\n", r, Vertexs[r].tipo);

		sBarra.push_back(r);
		int cont_t = 0;
		for (i = 0; i < n; i++) {
			if (r == i || g_linha[i] != 1 || Vertexs[i].tipo == 2)
				continue;
			valor = alg.generate_min_cut(r, i); //Calculando Corte Mínimo
			if (debug)
				printf("valor entre %d e %d (%d) =  %f \n", r, i,
						Vertexs[i].tipo, valor);
			if (valor < 2 - 1e-5) {
				violado = true;
				s.push_back(i);
				if (Vertexs[i].tipo == 1)
					cont_t++;
			} else {
				sBarra.push_back(i);
			}
		}

		if (!violado) {
			// 	printf("\n(7) nao violado\n");
			return false;
		}

		// INCIDENTES A S A QUALQUER VertexS FORA DE s.

		for (unsigned u = 0; u < s.size(); u++) {
			int a = s[u];
			for (int v = 0; v < n; v++) {
				if (a == v || Vertexs[v].tipo == 2 || Vertexs[v].reduzido) {
					continue;
				}
				std::vector<int>::iterator it;
				it = std::find(s.begin(), s.end(), v);
				if (it != s.end()) {
					continue;
				}
				if (a < v)
					lhs += vars[a][v];
				else
					lhs += vars[v][a];
			}
		}

//		for (unsigned u = 0; u < s.size(); u++) {
//			int a = s[u];
//			for (unsigned i = 0; i < sBarra.size(); i++) {
//				int v = sBarra[i];
//				if (a < v)
//					lhs += vars[a][v];
//				else
//					lhs += vars[v][a];
//			}
//		}

		if (debug) {
			cout << "numero de Ts: " << cont_t << endl;
			cout << "\nmip sol 7: " << lhs << " >= 2" << endl;
			cout << "\n\nViolado 7 \n" << endl;
//			if (obj <= 20415) {
//				getchar();
//			}
		}
		mipsol7++;
		addLazy(lhs >= 2.0);
		lhs = 0;
//		cout << "S_interseccao_T: " << S_interseccao_T << endl;
//		cin.get();

		delete g_linha;
		for (i = 0; i < n; i++)
			delete[] x[i];
		delete[] x;

		// if (debug)
		// 	printf(
		// 			"\n\n--------------------------------------------------------\n");

		return true;
		// cin.get();
	}

	bool cut_set_MIPSOL_8(bool debug) {
		if (debug)
			printf(
					"\n\n-------------- MIP SOL (8)----------------------------------------------------\n");
		int i, j;		//valor ficticio para verificacao
		GRBLinExpr lhs = 0, rhs = 0;
		double **x = new double *[n];
		int *sol;
		sol = new int[n];
		Grafo grafo(n);

		//		bool log = true;

		for (i = 0; i < n; i++) {
			x[i] = new double[n];
			sol[i] = 0;
		}

		double obj = getDoubleInfo(GRB_CB_MIPSOL_OBJ);
		if (debug) {
			printf("Solução: %f\n", obj);
		}
		for (i = 0; i < n; i++) {
			if (Vertexs[i].reduzido || Vertexs[i].tipo == 2)
				continue;
			for (j = i + 1; j < n; j++) {
				if (Vertexs[j].reduzido || Vertexs[j].tipo == 2)
					continue;
				// if (debug)printf("x_%d_%d = %.2f \t", i, j, x[i][j]);
				x[i][j] = getSolution(vars[i][j]);

				if (x[i][j] > 1e-5) {
					if (debug)
						printf("(\"%d\",\"%d\",\"%f\"),\t", i, j, x[i][j]);
//						printf("x_%d_%d=%.2f \t", i, j, x[i][j]);
					// if (debug)
					// 	printf("x_%d(%d)_%d(%d), \n", i, Vertexs[i].tipo, j,
					// 			Vertexs[j].tipo);
					sol[i] = 1;
					sol[j] = 1;
					grafo.addEdge(i, j);
				}
			}
		}

		MinCut alg = MinCut();
		alg.build_graph(sol, n, x);
		double flow = 0.0;
		double valor = 0.0;
		bool violado = false;
		vector<int> s, proibidos, sBarra;

		//montando vertices proibidos
		for (int v = 0; v < n; v++) {
			if (v == r || Vertexs[v].tipo == 2 || Vertexs[v].reduzido
					|| sol[v] != 1)
				continue;
			valor = alg.generate_min_cut(r, v);
			if (valor < 2.0 - 1e-5) {
				proibidos.push_back(v);
			}
		}

		// if (debug)
		// 	cout << "\nraiz_mincut: " << r << " (" << Vertexs[r].tipo << ") "
		// 			<< endl;

		bool teve_cut = false;
		double soma = 0.0;

		//para cada w \in W
		for (int w = 0; w < n; w++) {
			soma = 0.0;
			s.clear();
			sBarra.clear();
			bool violado = false;

			if (Vertexs[w].reduzido || Vertexs[w].tipo != 2)
				continue;

			if (debug) {
				printf("\n--------------------------------------------");
				printf("\nw: %d \n", w);
				printf("--------------------------------------------\n");

			}
			if (debug)
				cout << "coberto por " << Vertexs[w].coberto_por.size()
						<< " vértices:" << endl;

			if (debug) {
				cout << "S de " << w << ":" << endl;
//				cout << "tam de S: " << Vertexs[w].coberto_por.size() << endl;
			}
			//			if (Vertexs[w].coberto_por.size() < 1)
			//				continue;

			if (debug) {
				for (unsigned cobre = 0; cobre < Vertexs[w].coberto_por.size();
						cobre++) {
					int v = Vertexs[w].coberto_por[cobre];
					if (Vertexs[v].reduzido || sol[v] != 1)
						continue;
					printf("%d\t", v);
				}
			}

			if (debug)
				printf("\n");

			// para cada i \in R que cobre W
			for (unsigned cobre = 0; cobre < Vertexs[w].coberto_por.size();
					cobre++) {
				int v = Vertexs[w].coberto_por[cobre];
				if (Vertexs[v].reduzido || sol[v] != 1 || Vertexs[v].tipo == 2)
					continue;
				if (debug) {
					printf("%d (%d) \t", v, Vertexs[v].tipo);
				}

				if (r == v) {
					if (debug)
						printf("coberto pera raiz \n");
					continue;
				}

				if (debug)
					printf("r:%d v:%d \n", r, v);
				valor = alg.generate_min_cut(r, v);

				if (debug)
					printf("\nvalor entre %d e %d (%d) [%d] = %.2f", r, v,
							Vertexs[v].tipo, Vertexs[v].cobre.size(), valor);
				// printf("depois min cut \n");
				if (debug)
					printf("\nvalor: %f \n", valor);
				soma += valor;

				if (valor >= 2 - 1e-5) {
					sBarra.push_back(v);
				}

				// if (valor < 2 - 1E-5) {
				// violado = true;
				s.push_back(v);
				// if (debug)printf("%d(%d) em sBarra\t", v, Vertexs[v].tipo);
				// } else {
				// s.push_back(v);
				// if (debug)printf("%d(%d) em s\t", v, Vertexs[v].tipo);
				// }
				// cin.get();
			}

			if (debug) {
				cout << "\nsoma: " << soma << endl;
				cout << "tam de S: " << s.size() << endl;
			}

			if (soma >= 2.0 - 1e-5) {
				if (debug)
					printf("\nNao violado. Exiting... \n");
				continue;
			}

			for (unsigned u = 0; u < s.size(); u++) {
				int a = s[u];
				bool add = true;
				for (int v = 0; v < n; v++) {
					if (a == v || Vertexs[v].tipo == 2 || Vertexs[v].reduzido)
						continue;
					//v nao pode estar contido em s, senao a solucao inviavel nao sera cortada
					std::vector<int>::iterator it;
					it = std::find(s.begin(), s.end(), v);
					if (it != s.end())
						continue;
					if (a < v)
						lhs += vars[a][v];
					else
						lhs += vars[v][a];
				}
			}

//			sBarra.push_back(r);
//			for (unsigned u = 0; u < s.size(); u++) {
//				int a = s[u];
//				printf("sBarra de %d: \n", a);
//				for (int v = 0; v < sBarra.size(); v++) {
//					int b = sBarra[v];
//					printf("%d \t", b);
//					if (a < b)
//						lhs += vars[a][b];
//					else
//						lhs += vars[b][a];
//				}
//			}

			if (debug) {
				cout << "\nMip sol 8: " << lhs << " >= 2" << endl;
			}
//			if (violado) {
			if (debug) {
				printf("\nviolado 8 \n");
			}
			mipsol8++;
			addLazy(lhs >= 2.0);
//			}

			if (debug)
				printf("\n---------------------------------------------\n");

			lhs = 0;

			// if (debug) cin.get();
		}

		delete sol;
		for (i = 0; i < n; i++)
			delete[] x[i];
		delete[] x;

		if (debug)
			printf("\n-------------------------------------\n\n");
		//		cin.get();
		return teve_cut;
	}

	bool cut_set(bool debug, int tipo) {

		if (debug)
			if (tipo == 1) {
				printf("\n-------------\nMIP SOL: \n");
				cout << "solucao:" << getDoubleInfo(GRB_CB_MIPSOL_OBJ) << endl;
			} else {
				printf("\n-------------\nMIP NODE: \n");
			}

		int R = 0;
		GRBLinExpr lhs = 0, rhs = 0;
		double **x = new double *[n];
		int *sol;
		sol = new int[n];
		double* y = new double[n];
		unsigned cont_cuts = 0;
		vector<int> s, s_barra;

//		printf("valores de y na solucao: \n");
		for (int i = 0; i < n; i++) {
			x[i] = new double[n];
			sol[i] = 0;
			y[i] = 0.0;
			if (Vertexs[i].reduzido || Vertexs[i].tipo == 2)
				continue;
			if (tipo == 1)
				y[i] = getSolution(varsY[i]);
			else
				y[i] = getNodeRel(varsY[i]);

			if (debug)
				printf("y[%d] = %.2f \t", i, y[i]);

		}

		//montando o grafo
//		printf("Montando grafo: \n");
		for (int i = 0; i < n; i++) {
			for (int j = i + 1; j < n; j++) {
				// if (debug)printf("x_%d_%d = %.2f \t", i, j, x[i][j]);
				if (tipo == 1)
					x[i][j] = getSolution(vars[i][j]);
				else
					x[i][j] = getNodeRel(vars[i][j]);

				if (x[i][j] >= 1e-5) {
					//					printf("x_%d_%d=%.2f \t", i, j, x[i][j]);
					if (debug) {
						printf("(\"%d\",\"%d\",\"%.2f\"), \n", i, j, x[i][j]);
					}
					//					if (Vertexs[i].tipo == 0)
					sol[i] = 1;
					//					if (Vertexs[j].tipo == 0)
					sol[j] = 1;
				}
			}
		}

//		getchar();

		MinCut alg = MinCut();
		alg.build_graph(sol, n, x);
		double valor;
		//		bool violado = false;

		if (debug)
			cout << "raiz: " << r << endl;

		bool violado = false;
		s.push_back(r);

		for (int i = 0; i < n; i++) {
			if (sol[i] != 1 || i == r)
				continue;

			if (y[i] < 0.0001)//0.1 valor para consideramos que um t \in S está na solucao
				continue;

			// if (debug)
			// 	printf("y[%d]=%.2f \t", i, y[i]);

			valor = alg.generate_min_cut(r, i);

			if (debug)
				printf("sol[%d]=%d - valor entre %d e %d = %f \n", i, sol[i], r,
						i, valor);
			if (valor < 2 - 0.0001) {
				violado = true;
				s_barra.push_back(i);
			} else {
				s.push_back(i);
			}
		}

		if (!violado)
			return false;

		lhs = rhs = 0;

		for (unsigned u = 0; u < s_barra.size(); u++) {
			int a = s_barra[u];

			for (int v = 0; v < s.size(); v++) {
				int b = s[v];

				if (a == b)
					continue;

				//v nao pode estar contido em s, senao a solucao inviavel nao sera cortada
//				std::vector<int>::iterator it;
//				it = std::find(s_barra.begin(), s_barra.end(), v);
//				if (it != s_barra.end())
//					continue;

				if (a < b) {
					lhs += vars[a][b];
				} else {
					lhs += vars[b][a];
				}

//				rhs = 2 * varsY[a];

			}
		}

		for (unsigned u = 0; u < s_barra.size(); u++) {
			int a = s_barra[u];
			rhs = 0;
			rhs = 2* varsY[a];
			if (tipo == 1) {
				addLazy(lhs >= rhs);
				mipsol9++;
				if (debug) {
					//				cout << "teste: " << slhs << " >= " << rhs << endl;
					cout << "corte adicionado: " << lhs << ">=" << rhs << endl;
					getchar();
				}
//				break;
			} else {
				addCut(lhs >= rhs);
				mipnode9++;
				if (debug) {
					//				cout << "teste: " << slhs << " >= " << rhs << endl;
					cout << "corte adicionado: " << lhs << ">=" << rhs << endl;
//					getchar();
				}
//				break;
			}

		}

//		for (unsigned u = 0; u < s.size(); u++) {
//			int a = s[u];
//
//			string slhs, srhs;
//
////			for (unsigned v = 0; v < s_barra.size(); v++) {
////				int b = s_barra[v];
////
////				if (a < b) {
////					lhs += vars[a][b];
//////					slhs += "x_" + itos(a) + "_" + itos(v);
////				} else {
////					lhs += vars[b][a];
//////					slhs += "x_" + itos(a) + "_ \t" + itos(v);
////				}
////			}
//
//			for (int v = 0; v < n; v++) {
//				if (a == v)
//					continue;
//				if (a < v) {
//					lhs += vars[a][v];
//					//					slhs += "x_" + itos(a) + "_" + itos(v);
//				} else {
//					lhs += vars[v][a];
//					//					slhs += "x_" + itos(a) + "_ \t" + itos(v);
//				}
//			}
//
//			rhs = 2.0;
//
//		}
//
//		cout << "tam de S: " << s.size() << endl;
//		cout << "tam de S_barra: " << s_barra.size() << endl;
//
//		if (debug) {
//			//				cout << "teste: " << slhs << " >= " << rhs << endl;
//			cout << "corte adicionado: " << lhs << ">=" << rhs << endl;
////			getchar();
//
//		}

		delete y;
		delete sol;

		for (int i = 0; i < n; i++)
			delete[] x[i];

		delete[] x;

		return true;
	}

	void callback() {
		try {
			switch (where) {
			case GRB_CB_MIPNODE:
				if (getIntInfo(GRB_CB_MIPNODE_STATUS) == GRB_OPTIMAL) {
//					cut_set(false,0);
				}

				break;

			case GRB_CB_MIPSOL:
				cut_set(false, 1);
				break;
			}
		} catch (GRBException &e) {
			cout << "Error number: " << e.getErrorCode() << endl;
			cout << e.getMessage() << endl;
			printf("DEU MERDA !\n");
			getchar();
		} catch (...) {
			cout << "Error during callback" << endl;
		}

	}
}
;
