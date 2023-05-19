//#include "mycallback.h"
//#include <ctime>
//
//using namespace std;
//
//void read_instance(char* nome_instancia, int &root, int &qnd_w, int &qnd_vt,
//		int &qnd_t, int &qtd_n, double &prize, double &D, double &valor_otimo,
//		vector<Vertex> &vertices, double **&matriz_custo, double& prize_V);
////void solver(int root, int qtd_t, int qnd_vt, int qtd_n,
//vector<Vertex> vertices, double **matriz_custo, double prize,
//bool exigir_r, double prize_V);
//
//int main(int argc, char **argv) {
//	char *file_name;
//	int qtd_n = 0, qnd_w = 0, qnd_t = 0, qnd_vt = 0;
//	double valor_otimo, prize, prize_i, D;
//	vector<Vertex> vertices;
//	double** matriz_custo;
//	int** matriz_pre;
//	double* tempos;
//	double tempo_r12 = 0.0;
//	int i, j, root = 0;
//	bool exigir_r = false;
//	bool desigualdade_triangular = true;
//	bool debug = false;
//	int aplicar_regras = 0;
//	double obj = 0.0;
//	double prize_V = 0.0;
//
//	if (argc != 4) {
//		printf(
//				"./rrcp_fluxo data/nome_instancia aplicar_regras<0/1> aplicar cortes <0/1>\n");
//		return -1;
//	}
//
//	file_name = argv[1];
//	aplicar_regras = atoi(argv[2]);
//	do_cuts = atoi(argv[3]);
//
//	bool *ativadas = new bool[7];
//	tempos = new double[7];
//	int *nr = new int[7];
//
//	read_instance(file_name, root, qnd_w, qnd_vt, qnd_t, qtd_n, prize, D,
//			valor_otimo, vertices, matriz_custo, prize_V);
//
//	int qnd_w_i, qnd_vt_i, qnd_t_i;
//	prize_i = prize;
//	qnd_w_i = qnd_w;
//	qnd_vt_i = qnd_vt;
//	qnd_t_i = qnd_t;
//
//	if (debug) {
//		//printf("qtd_n: %d \n", qtd_n);
//		//printf("D: %f \n", D);
//		printf("valor_otimo: %.1f \n", valor_otimo);
//		// cout << "root: " << root << " tipo: " << vertices[root].tipo << endl;
//
//		cout << "\n---- PRE REDUCOES -----\n";
//		printf("prize: %0.f \n", prize);
//		cout << "t: " << qnd_t << endl;
//		cout << "vt: " << qnd_vt << endl;
//		cout << "w: " << qnd_w << endl;
//
//		printf("\nvertices T: \n");
//		for (i = 0; i < vertices.size(); i++)
//			if (vertices[i].tipo == 1)
//				printf("%d ", i);
//
//	}
//
//	clock_t begin = clock();
//
//	if (aplicar_regras) {
//		//verificando desigualdade triangular
//		for (i = 0; i < vertices.size(); i++) {
//			for (j = 0; j < vertices.size(); j++) {
//				if (i == j)
//					continue;
//				for (int k = 0; k < vertices.size(); k++) {
//					if (k == j)
//						continue;
//					if (matriz_custo[i][j]
//							> matriz_custo[i][k] + matriz_custo[k][j]) {
//						desigualdade_triangular = false;
//						// printf("(%d%d)=%d > (%d%d,%d%d)=%d", i, j, matriz_custo[i][j], i, k, k, j, matriz_custo[i][k] + matriz_custo[k][j]);
//						break;
//					}
//					if (!desigualdade_triangular)
//						break;
//				}
//				if (!desigualdade_triangular)
//					break;
//			}
//			if (!desigualdade_triangular)
//				break;
//		}
//
//		//regras
//
//		//regra_R0(vertices, qnd_w);
////		regra_R1(vertices, prize, qnd_vt, qnd_t);
////		regra_R2(vertices, prize, qnd_vt, qnd_t);
//
//		if (desigualdade_triangular) {
//			if (debug)
//				printf("\nDesigualdade triangular respeitada\n");
////			regra_R3(vertices, prize, qnd_vt, qnd_t);
////			regra_R4(vertices, prize, qnd_vt, qnd_t);
////			regra_R5(vertices, prize, qnd_vt, qnd_t, matriz_custo);
//			if (debug)
//				printf("Regras R3, R4 e R5 aplicadadas");
//		}
//
////		regra_R6(vertices, prize, qnd_vt, qnd_t, qnd_w);
////		exigir_r = regra_R7(vertices, prize, qnd_vt, qnd_t, qnd_w, exigir_r);
//
//		tempos[0] = regra_14(vertices, qnd_vt, matriz_custo, prize);	//r8
//		tempos[1] = regra_R9(vertices, prize, matriz_custo, qnd_vt);	//r9
//		tempos[2] = regra_10(vertices, prize, qnd_vt, qnd_t);	//r10
//		tempos[3] = regra_11(vertices, qnd_vt, qnd_t);	// r11
//
//		//dado que sempre |T|>1 entao R8 é sempre ativada
//		tempos[4] = regra_12(vertices, qnd_vt);	//r13
//		tempos[5] = regra_13(vertices, qnd_w);	//r14
//		// printf("\nANTES regra 13 \n");
//		// printf("DEPOIS regra 13 \n");
//
//	}
//
//	tempo_r12 = regra_R8(vertices, prize); // reducao do prize //r12
//
//	if (debug) {
//		cout << "\n\n---- POS REDUCOES -----\n";
//		printf("prize: %.0f\n", prize);
//		cout << "t: " << qnd_t << endl;
//		cout << "vt: " << qnd_vt << endl;
//		cout << "w: " << qnd_w << endl;
//
//		// printf("\nvertices T: \n");
//		// for (i = 0; i < vertices.size(); i++)
//		// 	if (vertices[i].tipo == 1)
//		// 		printf("%d ", i);
//
//		// printf("\n");
//
//		// printf("\nvertices W: \n");
//		// for (i = 0; i < vertices.size(); i++)
//		// 	if (!vertices[i].reduzido && vertices[i].tipo == 2)
//		// 		printf("%d ", i);
//
//		// printf("\n");
//
//		// printf("\nvertices R: \n");
//		// for (i = 0; i < vertices.size(); i++)
//		// 	if (!vertices[i].reduzido &&  vertices[i].tipo == 0)
//		// 		printf("%d ", i);
//
//		// printf("\n");
//	}
//
////	printf("%s \t %.2f\n", file_name, (prize_i - prize) / prize_i);
////	return 1;
//
////	printf("%s \t ", file_name);
//
//	clock_t end = clock();
//	double elapsed_secs = double(end - begin) / CLOCKS_PER_SEC;
//
////printf("%.3f \t %d \t %d \t %d\n", elapsed_secs, qnd_vt - qnd_vt_i, qnd_t - qnd_t_i, qnd_w - qnd_w_i);
////	 cout << ativadas[0] << "\t" << ativadas[1] << "\t" << ativadas[2] << "\t" << ativadas[3] << "\t" << ativadas[4] << "\t" <<
////      ativadas[5] << endl;
//
////printf(
////		"r8: %.3f \t r9: %.3f \t r10: %.3f \t r11: %.3f \t r13:%.3f \t r14: %.3f \t r12:%.3f \n",
////		tempos[0], tempos[1], tempos[2], tempos[3], tempos[4], tempos[5],
////		tempo_r12);
//
////return 0;
//
//	solver(root, qnd_t, qnd_vt, qtd_n, vertices, matriz_custo, prize, exigir_r,
//			prize_V);
//
//	printf("\nÓtimo:%.0f \n", valor_otimo);
//
//// clock_t end = clock();
//// double elapsed_secs = double(end - begin) / CLOCKS_PER_SEC;
//
//// cout<<"obj main: " <<obj;
//
//// printf("%.2f \t %d \t %d \t %d\n", elapsed_secs, qnd_vt, qnd_t, qnd_w);
//
//	printf("cover: %d\n", cover_cuts);
//	printf("cutsets: %d\n", cut_sets);
//
//	return (0);
//}
//
////void solver(int root, int qnd_t, int qnd_vt, int qtd_n,
//vector<Vertex> vertices, double **matriz_custo, double prize,
//bool exigir_r, double prize_V) {
//	clock_t begin = clock();
//
//	try {
//		GRBEnv env = GRBEnv();
//		env.set(GRB_IntParam_Presolve, 0);
//		env.set(GRB_IntParam_Cuts, 0);
//		//necessario quando se utiliza seus próprios cortes
//		env.set(GRB_IntParam_PreCrush, 1);
//		env.set(GRB_IntParam_Threads, 1);
//		env.set(GRB_DoubleParam_Heuristics, 0.0);
//		env.set(GRB_DoubleParam_TimeLimit, 3600.0);
//		//env->set(GRB_IntParam_OutputFlag, 0);
//
//		GRBModel model = GRBModel(env);
//
//		// Must set LazyConstraints parameter when using lazy constraints
//		model.getEnv().set(GRB_IntParam_LazyConstraints, 1);
//
//		GRBVar *y = 0;//representa os vertices
//		GRBVar **x = 0;//representa as arestas
//		// GRBVar** z = 0;
//		GRBLinExpr lhs, rhs, obj;
//		char varName[100];
//		int i, j, tamVT;
//
//		//cadastrando variavies
//		y = new GRBVar[qtd_n];
//		for (i = 0; i < vertices.size(); i++) {
//			if (vertices[i].reduzido || vertices[i].tipo == 2)
//			continue;
//			// cout << "indice Y: " << indiceY << endl;
//			sprintf(varName, "y_%d", i);
//			// if (vertices[indiceY].reduzido)
//			// 	y[i] = model.addVar(0.0, 0.0, 0.0, GRB_BINARY, varName);
//			// else
//			y[i] = model.addVar(0.0, 1.0, 0.0, GRB_BINARY, varName);
//			// cout << "y_" << i << ":" << vertices[i].tipo << "  ";
//		}
//
//		// printf("\n");
//
//		x = new GRBVar*[qtd_n];
//		for (i = 0; i < qtd_n; i++) {
//			x[i] = new GRBVar[qtd_n];
//		}
//
//		for (i = 0; i < qtd_n; i++) {
//			if (vertices[i].reduzido || vertices[i].tipo == 2)
//			continue;
//			for (j = i + 1; j < qtd_n; j++) {
//				if (vertices[j].reduzido || vertices[j].tipo == 2)
//				continue;
//				sprintf(varName, "x_%d_%d", i, j);
//				// printf("i j = %f", matriz_custo[i][j]);
//				// cin.get();
//				x[i][j] = model.addVar(0.0, 1.0, matriz_custo[i][j], GRB_BINARY,
//				varName);
//			}
//		}
//
//		model.update();
//
//		// printf("aqui 1 \n");
//		/* Restrições */
//
//		//restricao obtida pela regra R7:
//		//\sum_{i \in VT} yi >= 1
//		// if (exigir_r) {
//		// 	for (i = 0; i < vertices.size(); i++) {
//		// 		if (vertices[i].reduzido || vertices[i].tipo != 0) continue;
//		// 		lhs += y[i];
//		// 	}
//		// 	model.addConstr(lhs >= 1.0);
//		// 	lhs = 0;
//		// }
//		//(2)
//		for (i = 0; i < qtd_n; i++) {
//			if (vertices[i].reduzido || vertices[i].tipo == 2)
//			continue;
//			for (j = i + 1; j < qtd_n; j++) {
//				if (vertices[j].reduzido || vertices[j].tipo == 2)
//				continue;
//				lhs += x[i][j];
//			}
//		}
//		for (int r = 0; r < qtd_n; r++) {
//			if (vertices[r].reduzido || vertices[r].tipo != 0)
//			continue;
//			rhs += y[r];
//		}
//		rhs += qnd_t;
//		model.addConstr(lhs == rhs, "2");
//		lhs = rhs = 0;
//
//		// printf("aqui 1.2 \n");
//
//		//(3)
//		for (int k = 0; k < vertices.size(); ++k) {
//			// cout << "indice: " << indice << endl;
//			if (vertices[k].reduzido || vertices[k].tipo == 2)
//			continue;
//			lhs += vertices[k].premio * y[k];
//		}
//		model.addConstr(lhs >= prize, "3");
//		lhs = 0;
//
//		// printf("aqui 2 \n");
//
//		// (4) ∑_{k ∈ S_l}yk ≥ 1 (∀l ∈ W )
//		for (int l = 0; l < vertices.size(); l++) {
//			if (vertices[l].tipo != 2 || vertices[l].reduzido)
//			continue;
//			// if (vertices[indiceL].reduzido) continue;
//			// cout << "w_" << w << endl;
//			if (vertices[l].coberto_por.size() > 0) {
//				for (j = 0; j < vertices[l].coberto_por.size(); j++) {
//					int indiceK = vertices[l].coberto_por[j];
//					if (vertices[indiceK].reduzido
//					|| vertices[indiceK].tipo == 2)
//					continue;
//					// cout << "indice K: " << indiceK << endl;
//
//					lhs += y[indiceK];
//				}
//				// cout << lhs << endl;
//				sprintf(varName, "4.%d", l);
//				model.addConstr(lhs >= 1.0, varName);
//				lhs = 0;
//			}
//		}
//
//		// printf("aqui 3 \n");
//
//		// (5)
//		for (int t = 0; t < qtd_n; t++) {
//			if (vertices[t].reduzido || vertices[t].tipo != 1)
//			continue;
//			for (i = 0; i < qtd_n; i++) {
//				if (vertices[i].reduzido || vertices[i].tipo == 2)
//				continue;
//				for (j = i + 1; j < qtd_n; j++) {
//					if (vertices[j].reduzido || vertices[j].tipo == 2)
//					continue;
//					if (vertices[i].id == t || vertices[j].id == t) {
//						lhs += x[i][j];
//					}
//				}
//			}
//			sprintf(varName, "5.%d", t);
//			model.addConstr(lhs == 2.0, varName);
//			lhs = 0;
//		}
//
//		// printf("aqui 4 \n");
//
//		// (6)
//		for (int r = 0; r < qtd_n; r++) {
//			if (vertices[r].reduzido || vertices[r].tipo != 0)
//			continue;
//			for (i = 0; i < qtd_n; i++) {
//				if (vertices[i].reduzido || vertices[i].tipo == 2)
//				continue;
//				for (j = i + 1; j < qtd_n; j++) {
//					if (vertices[j].reduzido || vertices[j].tipo == 2)
//					continue;
//					if (vertices[i].id == r || vertices[j].id == r) {
//						lhs += x[i][j];
//					}
//				}
//			}
//			rhs = 2 * y[r];
//			sprintf(varName, "6.%d", r);
//			model.addConstr(lhs == rhs, varName);
//			lhs = rhs = 0;
//		}
//
//		// (10)
//		for (i = 0; i < qtd_n; i++) {
//			if (vertices[i].reduzido || vertices[i].tipo == 2)
//			continue;
//			for (j = i + 1; j < qtd_n; j++) {
//				if (vertices[j].reduzido || vertices[j].tipo == 2)
//				continue;
//				lhs = x[i][j];
//				rhs = y[i];
//				sprintf(varName, "10.%d.%d.%d", i, j, i);
//				model.addConstr(lhs <= rhs, varName);
//
//				rhs = y[j];
//				sprintf(varName, "10.%d.%d.%d", i, j, j);
//				model.addConstr(lhs <= rhs, varName);
//
//				lhs = rhs = 0;
//			}
//		}
//
//		// printf("aqui 6 \n");
//		printf("prize antes = %.0f\n", prize);
//
//		// Callback
//		// GRBVar **xvars, int _r, int qnd_n, double **_matriz_custo, std::vector<Vertice> _vertices, GRBVar* _y
//		mycallback cb = mycallback(x, root, qtd_n, matriz_custo, vertices, y, &model, prize, prize_V);
//		model.setCallback(&cb);
//
//		model.update();
//		// model.write("nao_dir.lp");
//		model.optimize();
//
//		clock_t end = clock();
//		double elapsed_secs = double(end - begin) / CLOCKS_PER_SEC;
//
//		cout << model.get(GRB_DoubleAttr_ObjVal) << "\t"
//		<< model.get(GRB_DoubleAttr_ObjBound) << "\t" << elapsed_secs
//		<< endl;
//
////		cout << "Obj: " << model.get(GRB_DoubleAttr_ObjVal) << endl;
//
////		//print solucao
////		int *sol;
////		sol = new int[qtd_n];
////		int max = 100000;
////		int *checagem;
////		checagem = new int[qtd_n];
////
////		bool tem_subciclo = false;
//
//		double **x_sol = new double *[qtd_n];
//		int *sol;
//		sol = new int[qtd_n];
//
//		for (i = 0; i < qtd_n; i++) {
//			x_sol[i] = new double[qtd_n];
//			sol[i] = 0;
//		}
////
//		for (i = 0; i < qtd_n; i++) {
//			if (vertices[i].reduzido || vertices[i].tipo == 2)
//			continue;
//			for (j = i + 1; j < qtd_n; j++) {
//				if (vertices[j].reduzido || vertices[j].tipo == 2)
//				continue;
//				if (x[i][j].get(GRB_DoubleAttr_X) > 1e-5) {
////					 cout << x[i][j].get(GRB_StringAttr_VarName) << "(" << vertices[i].tipo << "_" << vertices[j].tipo << ") = "
////					 << x[i][j].get(GRB_DoubleAttr_X) << endl;
////					printf("('%d','%d'), \n", i, j);
//					x_sol[i][j] = 1.0;
//					sol[i] = sol[j] = 1;
//				}
//			}
//		}
//
//		/*MinCut alg2 = MinCut();
//		 alg2.run_maxflow(sol, qtd_n, x_sol);
//		 int fora_ciclo = 0;
//		 double valor = 0.0;
//		 for (i = 0; i < qtd_n; i++) {
//		 if (root == i || sol[i] != 1)
//		 continue;
//		 valor = 0.0;
//		 valor = alg2.generate_min_cut(root, i); //Calculando Corte Mínimo
//		 //			if (debug)
//		 //			printf("valor entre %d e %d (%d) =  %f \n", root, i,
//		 //					vertices[i].tipo, valor);
//		 cout << "valor entre " << root << " e " << i << " = " << valor
//		 << endl;
//		 if (valor < 2 - 1e-5) {
//		 fora_ciclo++;
//		 }
//
//		 }
//
//		 printf("\nfora ciclo: %d \n", fora_ciclo); */
//
////
////		cout << endl;
////
////		for (i = 0; i < qtd_n; i++) {
////			if (vertices[i].reduzido || vertices[i].tipo == 2)
////				continue;
////			for (j = i + 1; j < qtd_n; j++) {
////				if (vertices[j].reduzido || vertices[j].tipo == 2)
////					continue;
////
////				if (x[i][j].get(GRB_DoubleAttr_X) > 0.0001) {
////					cout << x[i][j].get(GRB_StringAttr_VarName) << "("
////							<< vertices[i].tipo << "_" << vertices[j].tipo
////							<< ") = " << x[i][j].get(GRB_DoubleAttr_X) << endl;
////					// printf("('%d','%d'), \n", i,j);
////					sol[i] = sol[j] = 1;
////
////				}
////			}
////		}
////
////		if (tem_subciclo)
////			cout << "subciclo encontrado! \n";
////		else
////			cout << "Ciclo: OK" << endl;
////
////		double premio_acumulado = 0;
////		for (i = 0; i < qtd_n; i++) {
////			if (sol[i] == 1) {
////				premio_acumulado += vertices[i].premio;
////			}
////		}
////		// cout << "premio acumulado: " << premio_acumulado << endl;
////		if (prize > premio_acumulado)
////			cout << "Premio: NAO atingido!" << endl;
////		else
////			cout << "Premio: OK" << endl;
////
////		//verificar se todo w foi coberto
////
////		for (i = 0; i < qtd_n; i++) {
////			if (vertices[i].tipo == 2 || sol[i] == 0)
////				continue;
////			for (j = 0; j < vertices[i].cobre.size(); j++) {
////				int umw = vertices[i].cobre[j];
////				vertices[umw].reduzido = true;
////			}
////		}
////
////		bool todoscobertos = true;
////		;
////		for (int w = 0; w < qtd_n; w++) {
////			if (vertices[w].tipo != 2 || vertices[w].reduzido)
////				continue;
////			printf("w: %d nao foi coberto \n", w);
////			todoscobertos = false;
////			break;
////		}
////
////		if (todoscobertos)
////			cout << "Cobertura: Ok" << endl;
////		else
////			cout << "Cobertura: INCOMPLETA" << endl;
////		// cout << endl;
////
////		// for (i = 0; i < qtd_n; i++) {
////		// 	if (vertices[i].reduzido || vertices[i].tipo == 2) continue;
////		// 	for (j = 0; j < qtd_n; j++) {
////		// 		if (vertices[j].reduzido || vertices[j].tipo == 2) continue;
////		// 		if (i < j) {
////		// 			if (z[i][j].get(GRB_DoubleAttr_X) > 0.0001) {
////		// 				cout << z[i][j].get(GRB_StringAttr_VarName) << " = "
////		// 				     << z[i][j].get(GRB_DoubleAttr_X) << endl;
////		// 			}
////		// 		}
////		// 	}
////		// }
////
////		cout << endl;
//	} catch (GRBException e) {
//		cout << "Error code = " << e.getErrorCode() << endl;
//		cout << e.getMessage() << endl;
//	} catch (...) {
//		cout << "Exception during optimization" << endl;
//	}
//}
//
//void read_instance(char* nome_instancia, int &root, int &qnd_w, int &qnd_vt,
//		int &qnd_t, int &qtd_n, double & prize, double & D,
//		double & valor_otimo, vector<Vertex> &vertices, double **&matriz_custo,
//		double& prize_V) {
//	ifstream fin(nome_instancia);
//	int itoken;
//	double jtoken, umCusto;
//	int i, j;
//	double prizeT = 0.0;
//
//	if (fin == NULL) {
//		std::cout << "Nao abriu o arquivo!" << std::endl;
//		exit(1);
//	}
//
//// printf("instancia: %s \n", nome_instancia);
//
//	fin >> qtd_n;
//// printf("qtd_n: %d \n", qtd_n);
//	fin >> prize;
//	fin >> D;
//	fin >> valor_otimo;
//// printf("prize: %f \n", prize);
//// printf("D: %f \n", D);
//// printf("valor_otimo: %f \n", valor_otimo);
//
//	matriz_custo = new double*[qtd_n];
//
//	for (i = 0; i < qtd_n; i++) {
//		matriz_custo[i] = new double[qtd_n];
//	}
//// lendo os tipos dos vertices
//	for (i = 0; i < qtd_n; i++) {
//		fin >> itoken;
//		Vertex umVertice;
//		umVertice.id = i;
//		umVertice.reduzido = false;
//		switch (itoken) {
//		case 0:
//			//vertices opcionais
//			umVertice.tipo = 0;
//			vertices.push_back(umVertice);
//			qnd_vt++;
//
//			break;
//		case 1:
//			//vertices obrigatorio
//			umVertice.tipo = 1;
//			vertices.push_back(umVertice);
//			qnd_t++;
//			if (!root)
//				root = umVertice.id;	// 's' da formulacao do rogerio
//			break;
//		case 2:
//			//vertices que devem ser cobertos por algum vertice de V que faca parte da solucao
//			umVertice.tipo = 2;
//			vertices.push_back(umVertice);
//			qnd_w++;
//			break;
//		default:
//			break;
//		}
//
//	}
//
//// lendo os premios dos vertices
//	for (i = 0; i < qtd_n; ++i) {
//		fin >> itoken;
//		vertices[i].premio = itoken;
//		if (vertices[i].tipo == 0) //apena premios de R
//			prize_V += vertices[i].premio;
//	}
//
////lendo a matriz de custo
//	for (i = 0; i < qtd_n; ++i) {
//		for (j = 0; j < qtd_n; ++j) {
//			fin >> umCusto;
//			matriz_custo[i][j] = umCusto;
//			// Define qual nó W está no raio de cobertura dos nós T e VT.
//			if (vertices[i].tipo == 1 || vertices[i].tipo == 0) {
//				if (vertices[j].tipo == 2 && umCusto <= D) {
//					vertices[i].cobre.push_back(j);
//					// vertices[j].coberto_por.push_back(i);
//				}
//
//			} else {
//				// Define quais nós T e VT estão no raio de cobertura dos nós W.
//				if (vertices[j].tipo == 1 || vertices[j].tipo == 0)
//					if (umCusto <= D) {
//						vertices[i].coberto_por.push_back(j);
//						// vertices[j].cobre.push_back(i);
//					}
//			}
//		}
//		// printf("\n");
//	}
//
//// for (i = 0; i < qtd_n; i++) {
//// 	if (vertices[i].tipo == "t") {
//// 		prizeT += vertices[i].premio;
//// 		// cout << vertices[i].premio << endl;
//// 	}
//// }
//
//// cout << "prize dos t's: " << prizeT << endl;
//	fin.close();
//}
//
