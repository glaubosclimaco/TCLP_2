/*
 * Regras.h
 *
 *  Created on: 13/10/2017
 *      Author: Glaubos
 */

#ifndef REGRAS_H_
#define REGRAS_H_

#include <iostream>

//Funções auxiliares
bool teste_dois_em_Rw(int n, vector<Vertex> &v, int ** g, int removido);
//bool teste_t_rw(int n, vector<Vertex> &Vertexs, Grafo g);
bool teste_dois_t(int n, vector<Vertex> &Vertexs, int** g);
bool contemM(vector<int> a, vector<int> b);
bool ha_interseccaoM(vector<int> a, vector<int> b);
bool cobertura_vazia(vector<Vertex> v, int u);
bool teste_t_rw(int n, vector<Vertex> &v, int ** g, int removido, int*);

#define MAX_NOS 1000
//

/**
 * Se $\exists w \in W$, onde $|R_w| = 1$, então $T = T \cup R_w$.
 */

int regra_R1(vector<Vertex> &Vertexs, double &prize, int &qnd_vt, int &qnd_t) {
	bool ativada = false;
	int cont = 0;
	for (unsigned i = 0; i < Vertexs.size(); i++) {
		if (Vertexs[i].reduzido || Vertexs[i].tipo != 2)
			continue;

		//checando a conbertura de w
		int contCobertura = 0;
		for (int j = 0; j < Vertexs[i].coberto_por.size(); j++) {
			int a = Vertexs[i].coberto_por[j];
			if (!Vertexs[a].reduzido) {
				++contCobertura;
				if (contCobertura > 1)
					return 0;
			}
		}

		if (Vertexs[i].coberto_por.size() == 1) {
			int j = Vertexs[i].coberto_por[0];
			// printf("unico que cobre %d: %d \n", i, j);
			if (Vertexs[j].tipo == 0 && !Vertexs[j].reduzido) {
				Vertexs[j].tipo = 1;
				qnd_vt--;
				qnd_t++;
				ativada = true;
				cont++;
				// prize = prize - Vertexs[j].premio;
			}

		}
	}
	return cont;
}

/*
 Se $\exists j \in R$, onde $p_j > 0$ e $\sum_{i \in V \setminus \{ j \}} p_i < P$,
 então $T = T \cup \{ j \}$.
 */

int regra_R2(vector<Vertex> &Vertexs, double &prize, int &qnd_vt, int &qnd_t) {
	bool ativada = false;
	int cont = 0;
	for (unsigned j = 0; j < Vertexs.size(); j++) {
		if (Vertexs[j].reduzido || Vertexs[j].tipo != 0
				|| Vertexs[j].premio <= 0)
			continue;
		double sum_pi = 0.0;
		for (unsigned i = 0; i < Vertexs.size(); i++) {
			if (Vertexs[i].reduzido || Vertexs[i].tipo == 2)
				continue;
			sum_pi += Vertexs[i].premio;
		}
		double sum_pi_menos_pj = 0.0;
		sum_pi_menos_pj = sum_pi - Vertexs[j].premio;
		if (sum_pi_menos_pj < prize) {
			Vertexs[j].tipo = 1;
			qnd_t++;
			qnd_vt--;
			ativada = true;
			cont++;
		}
	}
	return cont;
}

/*
 INICIO: regras com c_ij satisfazendo a desigualdade triangular
 */

/*
 Se $\triangle$ e $\sum_{i \in T} >= P$, então todos vértice de
 $R \setminus R_W$ podem ser removidos de $G$.
 */

int regra_R3(vector<Vertex> &Vertexs, double &prize, int &qnd_vt, int &qnd_t,
		int &qnd_w, int grafoOriginal) {
	double sum = 0.0;
	bool ativada = false;
	bool debug = false;
	int contR = 0;
	//	checando se o grafo é original
	if (grafoOriginal) {
		if (debug) {
			printf("retornando false, o grafo é original :( \n");
		}
		return false;
	}

	for (unsigned i = 0; i < Vertexs.size(); i++) {
		if (Vertexs[i].tipo != 1)
			continue;
		sum += Vertexs[i].premio;
	}
	if (sum >= prize) {
		for (unsigned j = 0; j < Vertexs.size(); j++) {
			if (Vertexs[j].reduzido || Vertexs[j].tipo != 0)
				continue;
			if (Vertexs[j].cobre.size() == 0) {
				Vertexs[j].reduzido = true;
				qnd_vt--;
				ativada = false;
				contR++;
			}
		}
	}
	return contR;
}

/*
 Se $\triangle$, $\sum_{i \in T} >= P$ e $\exists r \in R$ onde $R_r
 \subseteq \bigcup_{t \in T} R_t$, então podemos remover o vértice $r$ de $G$.
 */

int regra_R4(vector<Vertex> &Vertexs, double &prize, int &qnd_vt, int &qnd_t,
		int &qnd_w, int grafoOriginal) {
	double sum = 0.0;
	int Rr = 0, r = 0;
	int cont_cobriu_Rr = 0;
	bool ativada = false;
	bool debug = false;
	int contR = 0;

	//	checando se o grafo é original
	if (grafoOriginal) {
		if (debug) {
			printf("retornando false, o grafo é original :( \n");
		}
		return false;
	}

	for (int t = 0; t < Vertexs.size(); t++) {
		if (Vertexs[t].tipo != 1)
			continue;
		sum += Vertexs[t].premio;
	}

	if (sum < prize)
		return ativada;

	for (r = 0; r < Vertexs.size(); r++) { //Vertexs r em VT
		if (Vertexs[r].tipo != 0 || Vertexs[r].reduzido)
			continue;
		for (Rr = 0; Rr < Vertexs[r].cobre.size(); Rr++) { //lista de Vertexs que r cobre
			int Rrw = Vertexs[r].cobre[Rr];
			if (Vertexs[Rrw].reduzido)
				continue;
			// for (int Rwt = 0; Rwt < Vertexs[Rrw].coberto_por.size(); Rwt++) {
			// 	int cobre_wr = Vertexs[Rrw].coberto_por[Rwt];
			// 	if (Vertexs[cobre_wr].tipo == 1) { //achou um t que cobre w (coberto tbm por r)
			// 		Vertexs[r].reduzido = true;
			// 		// qnd_vt--;
			// 		break;
			// 	}
			// }
			for (int t = 0; t < Vertexs.size(); t++) {
				if (Vertexs[t].tipo != 1)
					continue;
				for (int Rt = 0; Rt < Vertexs[t].cobre.size(); Rt++) {
					int Rtw = Vertexs[t].cobre[Rt];
					if (Rrw == Rtw) {
						cont_cobriu_Rr++;
						break;
					}
				}

			}
		}
		if (cont_cobriu_Rr == Vertexs[r].cobre.size()) {
			Vertexs[r].reduzido = true;
			qnd_vt--;
			contR++;
			ativada = true;
			cont_cobriu_Rr = 0;
		}

	}
	return ativada;
}

/*
 Se $\triangle$, $i,j \in R$, $i \neq j$, $\sum_{k \in V \setminus \{i \}} p_k \geq P$,
 $R_i \subseteq R_j$ e $c_{ik} > c_{jk}$, $\forall k \in V \setminus \{ i \}$,
 então podemos remover o vértice $i$ de $G$.
 */

int regra_R5(vector<Vertex> &Vertexs, double &prize, int &qnd_vt, int &qnd_t,
		double **matriz_custo) {
	bool ativada = false;
	double prize_k = 0.0;
	int contR = 0;
	for (int i = 0; i < Vertexs.size(); i++) {
		//se o prize total precisa do premio de i
		if (Vertexs[i].reduzido)
			continue;
		for (int k = 0; k < Vertexs.size(); k++) {
			if (k == i || Vertexs[k].reduzido)
				continue;
			prize_k += Vertexs[k].premio;
		}

		if (prize_k < prize)
			continue;
		//

		bool j_cobre_wi = true;
		for (int j = 0; j < Vertexs.size(); j++) {

			//se Ri esta contido em Rj
			if (Vertexs[j].reduzido || j == i)
				continue;
			bool j_cobre_wi = true;
			for (int Ri = 0; Ri < Vertexs[i].cobre.size(); Ri++) {
				int wi = Vertexs[i].cobre[Ri];
				std::vector<int>::iterator it;
				it = find(Vertexs[j].cobre.begin(), Vertexs[j].cobre.end(), wi);
				if (it == Vertexs[j].cobre.end()) { //j cobre o w de i
					j_cobre_wi = false;
					break;
				}
			}

			//se ik > jk
			// int tamVT = 0;
			// int cont_maior = 0;
			bool ik_lq_jk = false;
			if (j_cobre_wi) {
				for (unsigned k = 0; k < Vertexs.size(); k++) {
					if (k == i || Vertexs[k].reduzido)
						continue; // nao esta 100% correto. Era pra ser V\{i} apenas
					// tamVT++;
					if (matriz_custo[i][k] <= matriz_custo[j][k]) {
						ik_lq_jk = true;
						break;
					}
				}
			} else
				continue;

			if (!ik_lq_jk) {
				// cont_maior = tamVT = 0;
				qnd_vt--;
				ativada = true;
				Vertexs[i].reduzido = true;
				contR++;
				break;
			}
			//
		}
	}
	return contR;
}

/*
 FIM: regras com c_ij satisfazendo a desigualdade triangular
 */

/*
 Se $\exists t \in T$, tal que $R_t \neq \{ \emptyset \}$,
 então $W = W \setminus R_t$.
 */
int regra_R6(vector<Vertex> &Vertexs, double &prize, int &qnd_vt, int &qnd_t,
		int &qnd_w) {
	bool ativada = false;
	int contR = 0;
	for (int i = 0; i < Vertexs.size(); i++) {
		if (Vertexs[i].tipo != 1 || Vertexs[i].cobre.size() < 1)
			continue;
		for (int rt = 0; rt < Vertexs[i].cobre.size(); rt++) {
			int w = Vertexs[i].cobre[rt];
			if (Vertexs[w].tipo != 2 || Vertexs[w].reduzido)
				continue;
			qnd_w--;
			contR++;
			ativada = true;
			Vertexs[w].reduzido = true;
		}

	}
	return contR;
}

/*
 A regra 7 não é exatamente uma regra de redução. Se $\exists w \in W$ tal que $R_w = R$ então
 $W = W \setminus \{ w \}$ e tem que exigir que pelo menos um vértice de $R$ esteja na solução.
 */
int regra_R7(vector<Vertex> &Vertexs, double &prize, int &qnd_vt, int &qnd_t,
		int &qnd_w, bool &exigir_r, vector<int> R_set) {
	int cont_r = 0;
	bool ativada = false;
	exigir_r = false;
	int contR = 0;
	for (int w = 0; w < Vertexs.size(); w++) {
		if (Vertexs[w].reduzido || Vertexs[w].tipo != 2)
			continue;
		cont_r = 0;
		for (int rw = 0; rw < Vertexs[w].coberto_por.size(); rw++) {
			int r = Vertexs[w].coberto_por[rw];
			if (Vertexs[r].reduzido || Vertexs[r].tipo != 0)
				continue;
			cont_r++;
		}
		if (cont_r != qnd_vt) {
			break;
		} else {
			//check if all this cont_r belongs to R set
			bool contem = contemM(Vertexs[w].coberto_por, R_set);
			if (contem) {
				qnd_w--;
				ativada = true;
				Vertexs[w].reduzido = true;
				exigir_r = true;
				contR++;
			}

		}
	}

	return contR;
}

/**
 Se $\exists i \in T$, onde $p_i > 0$ então $P = P - p_i$ e $p_i = 0$.
 */
int regra_R8(vector<Vertex> &Vertexs, double & prize) {
//printf("\n prize antes da R8: %f\n", prize);
	clock_t begin = clock();
	double tempo = 0.0;
	bool ativada = false;
	double reducao_premio = 0;

	for (unsigned i = 0; i < Vertexs.size(); i++) {
		if (Vertexs[i].tipo != 1 || Vertexs[i].premio <= 0)
			continue;
		// if ((prize - Vertexs[i].premio) > 0) {
		// printf("tipo de %d: %d   Prêmio:%f \n", i,Vertexs[i].tipo, Vertexs[i].premio);
		prize = prize - Vertexs[i].premio;

		// reducao_premio += Vertexs[i].premio;
		Vertexs[i].premio = 0;
		// }
	}
//printf("prize depois da r8: %f\n", prize);
// cin.get();
// return reducao_premio;
	clock_t end = clock();
	tempo = double(end - begin) / CLOCKS_PER_SEC;
	return 0;
}

/**
 * Generalização de {\bf R3} e {\bf R4} - Se $\exists r \in R$ tal que $p_r = 0$ ou
 * $P \leq 0$, $\exists v \in R$ onde $R_r \subseteq R_v$ e $\forall (r,i), (r,j)
 * \in \delta(r)$,  $i,j \in V \setminus \{ v \}$ e $i \neq j$ $c_{ir} + c_{rj}
 * \geq c_{iv} + c_{vj}$ e $c_{ij} \leq c_{ir} + c_{rj}$, caso $(i,j) \notin E$ para
 * esse teste usamos $c_{ij} = \infty$, então $R = R \setminus \{ r \}$
 *
 * Ideia: remover um r \in R, que nao seja mais necessario. P pode ser <=0 após os
 * descontos de pi, i \in T.
 */

int regra_R9(vector<Vertex> &Vertexs, double & prize, double **matriz_custo,
		int &qnd_vt) {
	clock_t begin = clock();
	double tempo = 0.0;
//	bool pr_zero = false;
	bool remove_r;
	bool ativada = false;
	int contR = 0;
	for (unsigned r = 0; r < Vertexs.size(); r++) {
		remove_r = false;
		if (Vertexs[r].reduzido || Vertexs[r].tipo != 0)
			continue;

		// 	Se $\exists r \in R$ tal que $p_r = 0$ ou $P \leq 0$
		if (Vertexs[r].premio != 0 && prize > 0.0)
			continue;

		/*
		 $\exists v \in R$ onde $R_r \subseteq R_v$ e $\forall (r,i), (r,j)
		 * \in \delta(r)$,  $i,j \in V \setminus \{ v \}$ e $i \neq j$ $c_{ir} + c_{rj}
		 * \geq c_{iv} + c_{vj}$ e $c_{ij} \leq c_{ir} + c_{rj}$
		 */
		for (unsigned v = 0; v < Vertexs.size(); v++) {
			if (Vertexs[v].reduzido || Vertexs[v].tipo != 0
					|| Vertexs[v].id == Vertexs[r].id)
				continue;
			if (!contemM(Vertexs[v].cobre, Vertexs[r].cobre))
				continue;
			remove_r = true;
			for (unsigned i = 0; i < Vertexs.size(); i++) {
				if (Vertexs[i].reduzido || Vertexs[i].tipo == 2)
					continue;
				for (unsigned j = 0; j < Vertexs.size(); j++) {
					if (Vertexs[j].reduzido || Vertexs[j].tipo == 2)
						continue;
					if (i == j)
						continue;
					if (matriz_custo[i][r] + matriz_custo[r][j]
							< matriz_custo[i][v] + matriz_custo[v][j]
							|| matriz_custo[i][r] + matriz_custo[r][j]
									< matriz_custo[i][j]) {
						remove_r = false;
						break;
					}
				}
				if (!remove_r)
					break;
			}
			if (!remove_r)
				break;
		}
		// $R = R \setminus \{ r \}$
		if (remove_r) {
			Vertexs[r].reduzido = true;
			qnd_vt--;
			contR++;
			ativada = true;
			//printf("Ativada \n");
		}
	}
//	return ativada;
//	clock_t end = clock();
//	tempo = double(end - begin) / CLOCKS_PER_SEC;
	return contR;
}

//Se $\exists r \in R$ tal que em $G' = (V', E')$, onde $V = V \setminus \{ r \}$ e
//$E = E \setminus \delta(r)$, não exista dois caminhos disjuntos em vértices entre
//dois vértices terminais, um terminal e nenhum vértice de $R_w$, $w \in W$, ou entre
//nenhum par de vértice $i, j$ onde $i \in R_w$ e $j \in R_{\bar{w}}$, $w \neq \bar{w}$
// e $w, \bar{w} \in W$, então $T = T \cup \{r \}$.
int regra_10(vector<Vertex> &Vertexs, double & prize, int &qnd_vt, int &qnd_t,
		int &qnd_w, double** m, double inf, int grafoOriginal) {

	clock_t begin = clock();
	double tempo = 0.0;
	bool debug = false;
	int contR = 0;

	//	checando se o grafo é original
	if (grafoOriginal) {
		if (debug) {
			printf("retornando false, o grafo é original :( \n");
		}
		return 0;
	}

	bool ativada = false;
	if (debug)
		cout << "--------------\nRegra 10 aplicada\n--------------";

//	if (debug)
//		printf("\n\nEntrou regra 10\n");

	int n = Vertexs.size();
//	bool ativada = false;

	int *rMovidos = new int[n];

	for (int k = 0; k < n; k++) {
		if (Vertexs[k].reduzido || Vertexs[k].tipo != 0)
			continue;
		int r = Vertexs[k].id;

		//criando G' sem r
		if (debug) {
//			cin.get();
			printf("\nCriando G' sem %d \n", r);
		}

		Grafo g(n);
		int **grafoFlow;
		grafoFlow = new int*[n];

		for (int i = 0; i < n; i++) {
			grafoFlow[i] = new int[n];
		}

		MinCut maxflow = MinCut();

		for (int i = 0; i < n; i++)
			for (int j = 0; j < n; j++)
				grafoFlow[i][j] = 0;

		for (int q = 0; q < n; q++) {
			if (Vertexs[q].tipo == 2 || Vertexs[q].reduzido
					|| Vertexs[q].id == r)
				continue;
			for (int w = q + 1; w < n; w++) {
				if (Vertexs[w].tipo == 2 || Vertexs[w].reduzido
						|| Vertexs[w].id == r)
					continue;
				if (m[q][w] < inf) {
					grafoFlow[q][w] = 1;
					grafoFlow[w][q] = 1;
				}
			}
		}

		if (debug)
			printf("\nMontou o grafo sem %d \n", r);

		//fazendo os testes de relevancia de r
		bool teste1 = true, teste2 = true, teste3 = true;

		//verificar se existe dois caminhos disjuntos entre dois v \in T
		teste1 = teste_dois_t(n, Vertexs, grafoFlow);
		if (debug)
			printf("fez teste1\n");

		if (teste1) {
			if (debug) {
				printf("Passou no teste 1 \n");
			}
			teste2 = teste_t_rw(n, Vertexs, grafoFlow, r, rMovidos);
			if (debug)
				printf("\nfez teste2\n");
//
			if (teste2) {
				if (debug)
					printf("Passou no teste 2 \n");
				teste3 = teste_dois_em_Rw(n, Vertexs, grafoFlow, r);
			}
		}

		if (debug)
			printf("teste 1: %d \t teste 2: %d \t teste 3: %d \n", teste1,
					teste2, teste3);
		//se r for indispensavel, T <- T U {r}
//		if (!teste1 or !teste2 or !teste3) {
		if (!teste1 or !teste2 or !teste3) {
			Vertexs[r].tipo = 1;
			rMovidos[r] = 1;
//			Vertexs[r].reduzido = true;
			qnd_vt--;
			qnd_t++;
			contR++;
//			printf("R10 Ativada \n");
			ativada = true;
			if (debug) {
				printf("%d foi movido para T\n", r);
//				cin.get();
			}
		}
//		if (debug) {
//			cin.get();
//		}
//		cin.get();
	}
//return ativada;
//	clock_t end = clock();
//	tempo = double(end - begin) / CLOCKS_PER_SEC;
	if (debug)
		cout << "\nFim da regra 10\n";
//	return tempo;
	return contR;
}

/*
 \item {\bf R11} - Se $\exists r_1, r_2 \in R$ tal que $G' = (V', E')$, onde
 $V = V \setminus \{ r_1,r_2 \}$ e $E = E \setminus ( \delta(r_1) \cup \delta(r_2) )$,
 é desconexo. Chamaremos cada componente conexo de $G'$ por
 $G'_i = (V'_i, E'_i)$, $i \in \{ 1, \ldots nc \}$.
 Se $\nexists i \in \{ 1, \ldots nc \}$ tal que $T \subseteq V'_i$ e
 $V'_i \cap R_w \neq \{ \emptyset \}$, $\forall w \in W$, então $T = T \cup \{ r_1, r_2 \}$
 */

int regra_11(vector<Vertex> &Vertexs, int &qnd_vt, int &qnd_t, int &qnd_w,
		int** matriz_BL, int grafoOriginal) {
	clock_t begin = clock();
	double tempo = 0.0;
	int n = Vertexs.size();
// vector<vector<int> > componentes;

	bool ativada = false;
	bool debug = false;
	int contR = 0;

	//	checando se o grafo é original
	if (grafoOriginal) {
		if (debug) {
			printf("retornando false, o grafo é original :( \n");
		}
		return 0;
	}

	vector<int> conjunto_T;

	for (int t = 0; t < n; t++) {
		if (Vertexs[t].tipo != 1)
			continue;
		conjunto_T.push_back(t);
	}

	if (debug)
		printf("entrou na regra 11 \n");

	if (debug)
		printf("n = %d \n", n);

// para cada r1 e r2 \in R
	for (int r1 = 0; r1 < n; r1++) {
		if (Vertexs[r1].tipo != 0 || Vertexs[r1].reduzido)
			continue;
		for (int r2 = r1 + 1; r2 < n; r2++) {
			if (Vertexs[r2].tipo != 0 || Vertexs[r2].reduzido || r1 == r2)
				continue;

			if (debug)
				printf("Removidos de G: %d e %d\n", r1, r2);

			//criando G' / {r1,r2}
			Grafo g(n);
			int cont_solucao = 0;
			for (int q = 0; q < n; q++) {
				if (Vertexs[q].tipo == 2 || Vertexs[q].reduzido
						|| Vertexs[q].id == r1 || Vertexs[q].id == r2) {
					continue;
				}
				cont_solucao++;
				for (int w = q + 1; w < n; w++) {
					if (Vertexs[w].tipo == 2 || Vertexs[w].reduzido
							|| Vertexs[w].id == r1 || Vertexs[w].id == r2) {
						continue;
					}
					if (matriz_BL[q][w] != 0 and matriz_BL[w][q] != 0) {
						g.addEdge(q, w);
						g.addEdge(w, q);
					}
				}
			}

			// g.print_Grafo();

			//verificando se G' é deconexo
			//

			//------teste aqui-------
			// list<int> nodes;
			// nodes.push_back(1);
			// g.remove_edges(nodes);
			// Vertexs[1].reduzido=true;
			// cont_solucao--;

			//-----------------------

			if (debug)
				printf("\n\nVerificando se G'/{%d,%d} é desconexo... \n", r1,
						r2);

			int restantes;
			bool *visited;
			vector<int> v_linha;
			bool terminais_r1_r2 = true;

			restantes = cont_solucao;
			visited = new bool[n];

			for (int i = 0; i < n; i++)
				visited[i] = false;

			visited[r1] = true;		//para desconsiderar visita em r1
			visited[r2] = true;		//para desconsiderar visita em r2

			// if (debug) printf("# nos = %d \n", restantes);
			//
			int comp_cont = 1;

			if (debug)
				printf("----------------- Componente %d ------------------\n",
						comp_cont);

			if (debug)
				printf("restantes  antes = %d \n", restantes);

			g.connected(Vertexs, restantes, visited, v_linha);

			if (debug)
				printf("restantes  depois = %d \n", restantes);

			comp_cont++;

			// if (debug) printf("restantes removendo r1:%d e r2:%d = %d \n", r1, r2, restantes);

			if (restantes <= 0) {
				if (debug)
					printf("o grafo permaneceu conexo \n");
//				g.print_Grafo();
//				getchar();
				continue;
			} else {
				if (debug)
					printf("Grafo desconexo! \n");
//				g.print_Grafo();
			}

			//se for desconexo, vamos encontrar suas componentes conexas (C)

			//teste T para c_1
			bool teste_t = contemM(v_linha, conjunto_T);

			// teste Rw para c_1
			bool teste_Rw = true;
			for (int i = 0; i < n; i++) {
				if (Vertexs[i].reduzido || Vertexs[i].tipo != 2)
					continue;
				int w = i;
				bool w_coberto = false;
				for (int j = 0; j < Vertexs[w].coberto_por.size(); j++) {
					int u = Vertexs[w].coberto_por[j];
					if (Vertexs[u].reduzido)
						continue;
					w_coberto = find(v_linha.begin(), v_linha.end(), u)
							!= v_linha.end();
					if (w_coberto)
						break;
				}
				if (!w_coberto) {
					teste_Rw = false;
					break;
				}
			}

			if (teste_t && teste_Rw) {
				//nao ha reducao, passou no teste T e teste Rw
				if (debug)
					printf("passou no teste já da V_1 \n");
				//eturn ativada;
				clock_t end = clock();
				tempo = double(end - begin) / CLOCKS_PER_SEC;
				return tempo;
			}

			//teste T e Rw para as componentes restantes

			while (restantes > 0) {
				if (debug)
					printf(
							"----------------- Componente %d ------------------\n",
							comp_cont);
				g.connected(Vertexs, restantes, visited, v_linha);

				comp_cont++;

				if (debug)
					printf("restantes = %d \n", restantes);

				bool teste_t = contemM(v_linha, conjunto_T);
				bool teste_Rw = true;

				for (int i = 0; i < n; i++) {
					if (Vertexs[i].reduzido || Vertexs[i].tipo != 2)
						continue;
					int w = i;
					bool w_coberto = false;
					for (unsigned j = 0; j < Vertexs[w].coberto_por.size();
							j++) {
						int u = Vertexs[w].coberto_por[j];
						if (Vertexs[u].reduzido)
							continue;
						w_coberto = find(v_linha.begin(), v_linha.end(), u)
								!= v_linha.end();
						if (w_coberto)
							break;
					}
					if (!w_coberto) {
						if (debug)
							printf("w = %d nao foi coberto \n", w);
						teste_Rw = false;
						break;
					}
				}

				if (teste_t && teste_Rw) {
					//nao ha reducao, passou no teste T e teste Rw.
					if (debug)
						printf("passou nos testes t e Rw \n");
					//return ativada;
					clock_t end = clock();
					tempo = double(end - begin) / CLOCKS_PER_SEC;
					return tempo;
				}
				if (debug)
					printf("--------------------------------------\n");
//				cin.get();
			}

			//caso nao haja "return", adicionamos r1 e r2 em T.
			Vertexs[r1].tipo = 1;
			qnd_t++;
			qnd_vt--;
			++contR;
			Vertexs[r2].tipo = 1;
			qnd_t++;
			qnd_vt--;
			++contR;
			ativada = true;
//			printf("\nR11 Ativada \n");

		}
	}

//	clock_t end = clock();
//	tempo = double(end - begin) / CLOCKS_PER_SEC;
	return contR;

//return ativada;
}

/*
 \item {\bf R12} - Se $\exists i \in V$ tal que $|\delta(i)| \leq 1$,
 então $V = V \setminus \{ i \}$. (se $i \in T$ o problema é inviável)
 */
int regra_12(vector<Vertex> &v, int &qnd_vt, int &qnd_t, int qnd_w,
		int grafoOriginal) {
	bool ativada = false;
	int n = v.size();
	Grafo g(n);
	clock_t begin = clock();
	double tempo = 0.0;
	bool debug = false;
	int contR = 0;

	//	checando se o grafo é original
	if (grafoOriginal) {
		if (debug) {
			printf("retornando false, o grafo é original :( \n");
		}
		return false;
	}

//motando o grafo
	for (int i = 0; i < n; i++) {
		if (v[i].reduzido || v[i].tipo == 2)
			continue;
		for (int j = i + 1; j < n; j++) {
			if (v[j].reduzido || v[j].tipo == 2)
				continue;
			g.addEdge(i, j);
			g.addEdge(j, i);

		}
	}

//verificando o grau dos Vertexs
	for (int k = 0; k < n; k++) {
		if (v[k].reduzido || v[k].tipo == 2)
			continue;
		if (g.degree(k) <= 1) {
			if (v[k].tipo == 1) {
				printf("\n\n PROBLEMA INVIAVEL !!! \n\n");
				return true;
				cin.get();
			}
			v[k].reduzido = true;
			qnd_vt--;
			++contR;
			ativada = true;
			if (debug)
				printf("\nR12 Ativada \n");
		}
	}
//return ativada;
	clock_t end = clock();
	tempo = double(end - begin) / CLOCKS_PER_SEC;
	return ativada;
}

/*
 Se $\exists w, w' \in W$, $w \neq w'$, tal que $R_{w'} \subseteq R_w$,
 então $W = W \setminus \{ w \}$.
 */

//nao estou atualizando automaticamente a cobertura dos W
int regra_13(vector<Vertex> &v, int& qnd_w) {
	bool ativada = false;
	int n = v.size();
	bool debug = false;
	clock_t begin = clock();
	double tempo = 0.0;
	int contR = 0;

	if (debug)
		printf("\nRegra 13: \n");
	for (int w1 = 0; w1 < n; w1++) {
		if (v[w1].reduzido || v[w1].tipo != 2)
			continue;
		vector<int> cobertura_w1;
		if (debug)
			printf("\ncobertura de w1:%d\n", w1);
		for (int i = 0; i < v[w1].coberto_por.size(); i++) {
			int j = v[w1].coberto_por[i];
			//atualizando a cobertura de w1
			if (!v[j].reduzido)
				cobertura_w1.push_back(j);
			if (debug)
				printf("%d ", j);
		}
		if (debug)
			printf("\n");
		for (int w2 = w1 + 1; w2 < n; w2++) {
			if (v[w2].reduzido || v[w2].tipo != 2 || w1 == w2)
				continue;
			vector<int> cobertura_w2;
			if (debug)
				printf("\ncobertura de w2:%d \n", w2);
			for (int i = 0; i < v[w2].coberto_por.size(); i++) {
				int j = v[w2].coberto_por[i];
				if (!v[j].reduzido)
					cobertura_w2.push_back(j);
				if (debug)
					printf("%d ", j);
			}
			if (debug)
				printf("\n");
			bool Rw2_in_Rw1 = contemM(cobertura_w1, cobertura_w2);
			if (Rw2_in_Rw1 && v[w1].reduzido == false) {
				if (debug)
					printf("Rw_%d contemM Rw_%d \n", w1, w2);
				v[w1].reduzido = true;
				qnd_w--;
				contR++;
				ativada = true;
				//printf("Ativada 13\n");
				if (debug)
					printf("w = %d  removido pela regra 13 \n", w1);
				break;
			}
		}
	}
//	clock_t end = clock();
//	tempo = double(end - begin) / CLOCKS_PER_SEC;
	return contR;
//return ativada;
}

//R8 na tese
int regra_14(vector<Vertex> &v, int &qnd_vt, double** &matriz_custo,
		double &prize, int& qnd_t, int& qnd_w, int grafoOriginal) {
	int n = v.size();
	bool ativada = false;
	double prize_temp = prize;
	clock_t begin = clock();
	double tempo = 0.0;
	bool debug = false;
	int contR = 0;

	//	checando se o grafo é original
	if (grafoOriginal) {
		if (debug) {
			printf("retornando false, o grafo é original :( \n");
		}
		return false;
	}

//verificar se o prize ja é atendido
//	for (int i = 0; i < n; i++) {
//		if (v[i].tipo == 1) {
//			prize_temp -= v[i].premio;
//		}
//	}

	for (int r = 0; r < n; r++) {
		if (v[r].tipo != 0 || v[r].reduzido || v[r].premio > 0)
			continue;
		if (!cobertura_vazia(v, r))
			continue;

		//vamos remover r de R
		v[r].reduzido = true;
		//printf("\nr:%d sera removido\n", r);
		qnd_vt--;
		++contR;
		ativada = true;
//		printf("Ativada 14\n");
		for (int i = 0; i < n; i++) {
			if (v[i].reduzido || v[i].tipo == 2 || i == r)
				continue;
			for (int j = 0; j < n; j++) {
				if (v[j].reduzido || v[j].tipo == 2 || j == r || i == j)
					continue;
				double umCusto = matriz_custo[i][r] + matriz_custo[r][j];
				// if (matriz_pre[i][j] == 0) {
				// matriz_pre[i][j] = 1;
				// matriz_custo[i][j] = umCusto;
				// } else {
				if (matriz_custo[i][j] > umCusto)
					matriz_custo[i][j] = umCusto;
				// }
			}
		}
	}
//return ativada;
//	clock_t end = clock();
//	tempo = double(end - begin) / CLOCKS_PER_SEC;
	return contR;
}

bool teste_dois_t(int n, vector<Vertex> &v, int** g) {
	bool debug = false;
	for (int i = 0; i < n; i++) {
		if (v[i].tipo != 1 || v[i].reduzido)
			continue;
		for (int j = i + 1; j < n; j++) {
			if (v[j].tipo != 1 || v[j].reduzido)
				continue;
			int max_flow = fordFulkerson(g, i, j, n);
			if (max_flow < 2) {
				if (debug) {
					printf("falha entre %d e %d \n", i, j);
				}
				return false;
			}
//			printf("max flow entre %d e %d: %d\n", i, j, max_flow);
		}
//		cin.get();
	}

	return true;
}

bool teste_t_rw(int n, vector<Vertex> &v, int ** g, int removido,
		int *rMovido) {

	bool debug = false;
	if (debug)
		printf("\n teste_t_rw:\n");
	for (int w = 0; w < n; w++) { //w
		if (v[w].tipo != 2 or v[w].reduzido)
			continue;
		if (debug)
			printf("\n\n|Rw_%d|: %d\t", w, v[w].coberto_por.size());
		for (unsigned i = 0; i < v[w].coberto_por.size(); i++) { //rw
			int rwi = v[w].coberto_por[i];
			if (v[rwi].reduzido || rwi == removido || v[rwi].tipo != 1)
				continue;
			if (debug)
				printf("%d (%d) \t  ", rwi, v[rwi].tipo);
			bool bicon_Rw_t = false;
			int t = 0;
			for (t = 0; t < n; t++) { //t
				if (v[t].tipo != 1 || t == rwi)
					continue;

				if (debug) {
					printf("\nt_%d: \t", t);
				}

				int valor = fordFulkerson(g, rwi, t, n);
				if (valor >= 2) {
					bicon_Rw_t = true;
					break;
				}
			}
			if (!bicon_Rw_t) {
				if (debug) {
					printf("nao ha uma biconexao entre Rw_%d e t_%d", w, t);
					cin.get();
				}
				return false;
			}
		}
	}
	return true;
}

//bool teste_t_rw(int n, vector<Vertex> &v, int ** g, int removido,
//		int *rMovido) {
//	bool debug = false;
//	for (int t = 0; t < n; t++) { //t
//		if (v[t].tipo != 1 || rMovido[t] == 1)
//			continue;
//		for (int w = 0; w < n; w++) { //w
//			if (v[w].tipo != 2 or v[w].reduzido)
//				continue;
//			bool tRw = false;
//			if (debug) {
//				printf("checando t:%d com Rw_%d \n", t, w);
//			}
//			for (int i = 0; i < v[w].coberto_por.size(); i++) { //rw
//				int rwi = v[w].coberto_por[i];
//				if (v[rwi].reduzido || t == rwi || rwi == removido)
//					continue;
//				int valor = fordFulkerson(g, t, rwi, n);
//				if (debug)
//					printf("\t\tfluxo entre t_%d(%d) e rwi_%d(%d) -> %d\n", t,
//							v[t].tipo, rwi, v[rwi].tipo, valor);
//				if (valor >= 2) {
//					tRw = true;
//					break;
//				}
//			}
//			if (tRw == false) { //nao biconexao entre rw e t
//				if (debug) {
//					printf("<< Teste 2: Falha em t:%d e Rw:%d >>\n", t, w);
//					cin.get();
//				}
//				return false;
//			}
//		}
//	}
//	return true;
//}

//true: após remover r de G, existe dois caminhos disjuntos (em Vertexs) entre
// pelo menos algum t e algum Rw
//bool teste_t_rw(int n, vector<Vertex> &Vertexs, Grafo g) {
//	bool debug = false;
//	for (int i = 0; i < n; i++) { //t
//		if (Vertexs[i].tipo != 1)
//			continue;
//		for (int j = 0; j < n; j++) { //rW
//			if (Vertexs[j].reduzido || Vertexs[i].id == Vertexs[j].id
//					|| Vertexs[j].tipo != 0 || Vertexs[j].cobre.size() < 1)
//				continue;
//			int s, t;
//			s = Vertexs[i].id;
//			t = Vertexs[j].id;
//			list<int> para_remover;
//			int *pred = new int[n];
//
//			int n = g.getV();
//			Grafo gTemp(n);
//			gTemp.copy(g);
//
//			if (debug)
//				cout << "caminho 1" << endl;
//			if (debug)
//				printf("PAR %d(%d)  %d(%d) \n", s, Vertexs[s].tipo, t,
//						Vertexs[t].tipo);
//			bool reachable = gTemp.isReachable(s, t, pred);
//			if (debug)
//				cout << "isReachable =" << reachable << endl;
//
//			int key = t;
//
//			while (pred[key] != -1) {
//				// if (pred[key] == s) continue;
//				if (pred[key] != s) {
//					if (debug)
//						printf("%d \n", pred[key]);
//					para_remover.push_back(pred[key]);
//				}
//				key = pred[key];
//				// para_remover.push_back(pred[key]);
//			}
//
//			if (debug) {
//				printf("Vertexs a serem removidos: \n");
//				for (list<int>::iterator e = para_remover.begin();
//						e != para_remover.end(); e++)
//					printf("%d \t", *e);
//			}
//
//			delete[] pred;
//
//			int* pred2 = new int[n];
//			// nao ha caminho entre s e t
//			if (!reachable)
//				continue;
//
//			//removendo os Vertexs do grafo
//			if (debug)
//				cout << "removendo Vertexs" << endl;
//			gTemp.remove_edges(para_remover);
//			para_remover.clear();
//
//			//encontrando o segundo caminho entre s e t
//			if (debug)
//				cout << "caminho 2" << endl;
//			if (debug)
//				printf("s: %d  t: %d \n", s, t);
//
//			reachable = gTemp.isReachable(s, t, pred2);
//			if (debug)
//				cout << "isReachable =" << reachable << endl;
//
////			key = t;
////			while (pred2[key] != -1) {
////				if (pred2[key] != s)
////					if (debug)
////						printf("%d \n", pred2[key]);
////				key = pred2[key];
////				// para_remover.push_back(pred2[key]);
////			}
////			// cout << "isReachable done" << endl;
//			delete[] pred2;
//
//			if (reachable)
//				return true;
//			else
//				continue;
//
//		}
//	}
//
//	return false;
//}

bool cobertura_vazia(vector<Vertex> v, int u) {
	bool vazia = true;
	for (int i = 0; i < v[u].cobre.size(); i++) {
		int a = v[u].cobre[i];
		if (!v[a].reduzido) {
			vazia = false;
			break;
		}
	}
	return vazia;
}

//nenhum par de vértice $i, j$ onde $i \in R_w$ e
//$j \in R_{\bar{w}}$, $w \neq \bar{w}$
bool teste_dois_em_Rw(int n, vector<Vertex> &v, int ** g, int removido) {

	bool debug = false;

	for (int w1 = 0; w1 < n; w1++) { //w1
		if (v[w1].tipo != 2 or v[w1].reduzido)
			continue;
		if (debug)
			printf("\n\nRw1_%d: \t", w1);
		for (int i = 0; i < v[w1].coberto_por.size(); i++) { //rw1
			int rw1i = v[w1].coberto_por[i];
			if (v[rw1i].reduzido || rw1i == removido)
				continue;
			if (debug)
				printf("%d \t", rw1i);
			for (int w2 = w1 + 1; w2 < n; w2++) { //w2
				if (v[w2].tipo != 2 or v[w2].reduzido or w1 == w2)
					continue;
				bool bicon_Rw1_Rw2 = false;
				if (debug) {
					printf("\nRw2_%d: \t", w2);
				}
				for (int j = 0; j < v[w2].coberto_por.size(); j++) { //rw2
					int rw2j = v[w2].coberto_por[j];
					if (v[rw2j].reduzido || rw2j == removido || rw1i == rw2j)
						continue;
//					if (rw1i == rw2j) {
//						bicon_Rw1_Rw2 = true;
//						break;
//					}
					if (debug)
						printf("%d \t", rw1i);
					int valor = fordFulkerson(g, rw1i, rw2j, n);
					if (valor >= 2) {
						bicon_Rw1_Rw2 = true;
						break;
					}
				}
				if (!bicon_Rw1_Rw2) {
					if (debug) {
						printf("nao ha uma biconexao entre Rw_%d e Rw_%d", w1,
								w2);
						cin.get();
					}
					return false;
				}
			}
		}
	}
	return true;
}

#endif /* REGRAS_H_ */
