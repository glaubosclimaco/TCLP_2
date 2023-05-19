#include <iostream>
#include "SampleDecoder.h"
#include "MTRand.h"
#include "BRKGA.h"
#include "model/Mycallback.h"
#include "utils.h"

std::string get_str_between_two_str(const std::string &s,
		const std::string &start_delim, const std::string &stop_delim);
void get_instancia_pctsp_2(char *file_name, Vertice*& vertices_tsp, Matriz*& C,
		int& qtd_n, int& premio_total, int& pen_total, double**& matriz_custo);
void get_instancia_pctsp(char *file_name, Vertice*& vertices_tsp, Matriz*& C,
		int qtd_n, int& premio_total, int& pen_total, double**& matriz_custo);
double solver(int** matriz_BL, int n, vector<Vertex> vertices,
		double **matriz_custo, int prize, double limPrimal);

int main(int argc, char* argv[]) {

	//parametros do problema

	int seed, qtd_n, premio_total = 0, premio_solicitado = 0, pen_total;
	double rho, **matriz_custo;
	int lorena = 0;

	char file_name[100];

	if (argc != 4) {
		printf("./executavel <data/nome_instancia> <sed> <rho> \n");
		return -1;
	}

	strcpy(file_name, argv[1]);
	seed = atoi(argv[2]);
	rho = atof(argv[3]);

	//Pegar qtd de vertices para inicializar estruturas auxiliares
	std::fstream fin(file_name);

	if (lorena) {
		string nos = get_str_between_two_str(file_name, "_", "_");

		qtd_n = atoi(nos.c_str());
	} else {
		if (rho != 0.75) {
			printf("parametro errado! \n");
			return -1;
		}
		fin >> qtd_n;
	}

	Matriz *C;
	Vertice* vertices_instancia = (Vertice*) malloc(qtd_n * sizeof(Vertice));

	if (lorena) {
		//Carregar instancia do arquivo
		get_instancia_pctsp(file_name, vertices_instancia, C, qtd_n,
				premio_total, pen_total, matriz_custo);
	} else {
		get_instancia_pctsp_2(file_name, vertices_instancia, C, qtd_n,
				premio_total, pen_total, matriz_custo);
	}

	premio_solicitado = premio_total * rho;

	printf("premio_total: %d \n", premio_total);
	printf("rho: %f \n", rho);
	printf("premio_solicitado: %d \n", premio_solicitado);

	const unsigned n = qtd_n;		// size of chromosomes
	printf("tam do cromossomo: %d \n\n", n);

	const unsigned p = 100;	// size of population
	const double pe = 0.20;		// fraction of population to be the elite-set
	const double pm = 0.10;	// fraction of population to be replaced by mutants
	const double rhoe = 0.70;// probability that offspring inherit an allele from elite parent
	const unsigned K = 1;		// number of independent populations
	const unsigned MAXT = 1;	// number of threads for parallel decoding

	SampleDecoder decoder(premio_solicitado, pen_total, vertices_instancia,
			matriz_custo);	// initialize the decoder

//	const long unsigned rngSeed = 0;	// seed to the random number generator
	MTRand rng(seed);			// initialize the random number generator

	// initialize the BRKGA-based heuristic
	BRKGA<SampleDecoder, MTRand> algorithm(vertices_instancia, matriz_custo, n,
			p, pe, pm, rhoe, decoder, rng, K, MAXT);

	unsigned generation = 0;		// current generation
	const unsigned X_INTVL = 100;// exchange best individuals at every 100 generations
	const unsigned X_NUMBER = 2;	// exchange top 2 best
	const unsigned MAX_GENS = 300;	// run for 1000 gens

	do {
		algorithm.evolve();	// evolve the population for one generation

		if ((++generation) % X_INTVL == 0) {
			algorithm.exchangeElite(X_NUMBER);	// exchange top individuals
		}
		std::cout << generation<<" :  "<<algorithm.getBestFitness() << std::endl;
	} while (generation < MAX_GENS);

	std::cout << "Best solution found has objective value = "
			<< algorithm.getBestFitness() << std::endl;

	return 0;
}

double solver(int** matriz_BL, int n, vector<Vertex> vertices,
		double **matriz_custo, int prize, double limPrimal) {
	try {
		GRBEnv env = GRBEnv();

		//env.set(GRB_IntParam_Presolve, 0);
		env.set(GRB_IntParam_Cuts, 0);
		env.set(GRB_IntParam_PreCrush, 1); //necessario quando se utiliza seus próprios cortes
		env.set(GRB_IntParam_Threads, 1);
		//env.set(GRB_DoubleParam_Heuristics, 0.0);
		env.set(GRB_DoubleParam_TimeLimit, 3600.0);
//		env.set(GRB_IntParam_OutputFlag, 0);

		// Must set LazyConstraints parameter when using lazy constraints
		env.set(GRB_IntParam_LazyConstraints, 1);

		GRBModel model = GRBModel(env);

		GRBVar *y = 0; //representa os vertices
		GRBVar **x = 0; //representa as arestas
		GRBLinExpr lhs, rhs, obj;

		char varName[100];

		//cadastrando variavies
		y = new GRBVar[n];
		for (unsigned i = 0; i < n; i++) {
			sprintf(varName, "y_%d", i);


		}

		x = new GRBVar*[n];
		for (int i = 0; i < n; i++) {
			x[i] = new GRBVar[n];
		}

		for (int i = 0; i < n; i++) {
			for (int j = i + 1; j < n; j++) {
				sprintf(varName, "x_%d_%d", i, j);
				//				cout<<varName<<endl;
				//				cin.get();
				x[i][j] = model.addVar(0.0, 1.0, matriz_custo[i][j],
				GRB_BINARY, varName);
			}
		}

		model.update();

		/* ***************************************************
		 *
		 * 				Restrições
		 *
		 * ***************************************************/

		//restricao do prize
		for (unsigned k = 0; k < n; ++k) {
			// cout << "indice: " << indice << endl;
			lhs += vertices[k].premio * y[k];
		}
		model.addConstr(lhs >= prize, "1:");
		lhs = 0;

		//\sum_{e \in \delta(i)} x_e = 2 y_i,   \forall i \in V
		for (int r = 0; r < n; r++) {
			for (int i = 0; i < n; i++) {
				for (int j = i + 1; j < n; j++) {
					if (vertices[i].id == r || vertices[j].id == r) {
						lhs += x[i][j];
					}
				}
			}
			rhs = 2 * y[r];
			sprintf(varName, "2.%d", r);
			model.addConstr(lhs == rhs, varName);
			lhs = rhs = 0;
		}

		// x_e <= y_v   \forall e = (i,j) \in E, v \in \{ i, j \}, v \in R
		for (int i = 0; i < n; i++) {
			for (int j = i + 1; j < n; j++) {
				lhs = x[i][j];
				rhs = y[i];
				sprintf(varName, "3.%d.%d.%d", i, j, i);
				model.addConstr(lhs <= rhs, varName);

				rhs = y[j];
				sprintf(varName, "3.%d.%d.%d", i, j, j);
				model.addConstr(lhs <= rhs, varName);

				lhs = rhs = 0;
			}
		}

		// vertice origem na solucao
		lhs = y[0];
		model.addConstr(lhs == 1);

		/*
		 * FO
		 * */

		for (int i = 0; i < n; i++) {
			for (int j = i + 1; j < n; j++) {
				obj += x[i][j] * matriz_custo[i][j];
			}
		}

		for (int i = 0; i < n; i++) {
			obj += vertices[i].penalidade * (1 - y[i]);
		}

		model.setObjective(obj, GRB_MINIMIZE);

//		//call_back
		Mycallback cb = Mycallback(x, 0, n, matriz_custo, vertices, y, &model,
				prize);

		model.setCallback(&cb);

		model.update();

		model.write("model.lp");

		model.optimize();

//		cout << model.get(GRB_DoubleAttr_ObjVal) << endl;

		for (int i = 0; i < n; i++) {
			for (int j = i + 1; j < n; j++) {
				if (x[i][j].get(GRB_DoubleAttr_X) <= 0.9)
					continue;
				printf("x_%d_%d = %0.2f \t", i, j,
						x[i][j].get(GRB_DoubleAttr_X));
			}
		}

		printf("\n");

		for (int i = 0; i < n; i++) {
			if (y[i].get(GRB_DoubleAttr_X) > 0) {
				printf("y_%d_(%d)_(%.0f)\t", i, vertices[i].penalidade,
						vertices[i].premio);
			}
		}

		return model.get(GRB_DoubleAttr_ObjVal);

	} catch (GRBException e) {
		cout << "Error code = " << e.getErrorCode() << endl;
		cout << e.getMessage() << endl;
		return (limPrimal);
	} catch (...) {
		cout << "Exception during optimization" << endl;
		return (limPrimal);
	}

}

void get_instancia_pctsp(char *file_name, Vertice*& vertices_tsp, Matriz*& C,
		int qtd_n, int& premio_total, int& pen_total, double**& matriz_custo) {

	//Variaveis
	int itoken = 0;
	float ftoken = 0.0;
	int premio = 0;
	int count_t = 0;
	float d = 0.0;
	int valor_otimo = 0;

	std::fstream fin(file_name);

//	cout << file_name << endl;

	matriz_custo = new double*[qtd_n];

	for (int i = 0; i < qtd_n; i++) {
		matriz_custo[i] = new double[qtd_n];
	}

	if (!fin) {
		printf("Erro ao abrir o arquivo!\n");
		getchar();
		return;
	}

	//premio dos vertices
	for (int i = 0; i < qtd_n; ++i) {
		fin >> itoken;
		vertices_tsp[i].id = i;
//		cout << itoken << endl;
		vertices_tsp[i].premio = itoken;
		premio_total += itoken;
	}

	//penalidade dos vertices
	for (int i = 0; i < qtd_n; ++i) {
		fin >> itoken;
		vertices_tsp[i].penalidade = itoken;
//		if (i != 0)
		pen_total += itoken;
	}

	//Matriz de Distancias
	for (int i = 0; i < qtd_n; ++i) {
		for (int j = 0; j < qtd_n; ++j) {
			fin >> ftoken;
			if (i == j) {
				ftoken = FLT_MAX;

			}
			matriz_custo[i][j] = ftoken;
//			printf("matriz_custo[%d][%d] = %f", i,j,ftoken);
//			getchar();
		}
	}

}

void get_instancia_pctsp_2(char *file_name, Vertice*& vertices_tsp, Matriz*& C,
		int& qtd_n, int& premio_total, int& pen_total, double**& matriz_custo) {

	//Variaveis
	int itoken = 0;
	float ftoken = 0.0;
	int premio = 0;
	int count_t = 0;
	float d = 0.0;
	int valor_otimo = 0;

	std::fstream fin(file_name);

//	cout << file_name << endl;

	fin >> qtd_n;

	matriz_custo = new double*[qtd_n];

	for (int i = 0; i < qtd_n; i++) {
		matriz_custo[i] = new double[qtd_n];
	}

	if (!fin) {
		printf("Erro ao abrir o arquivo!\n");
		getchar();
		return;
	}

	//premio dos vertices
	for (int i = 0; i < qtd_n; ++i) {
		fin >> itoken;
		vertices_tsp[i].id = i;
//		cout << itoken << endl;
		vertices_tsp[i].premio = itoken;
		premio_total += itoken;
	}

	//penalidade dos vertices
	for (int i = 0; i < qtd_n; ++i) {
		fin >> itoken;
		vertices_tsp[i].penalidade = itoken;
//		if (i != 0)
		pen_total += itoken;
	}

	//Matriz de Distancias
	for (int i = 0; i < qtd_n; ++i) {
		for (int j = 0; j < qtd_n; ++j) {
			fin >> ftoken;
			if (i == j) {
				ftoken = FLT_MAX;

			}
			matriz_custo[i][j] = ftoken;
//			printf("matriz_custo[%d][%d] = %f", i,j,ftoken);
//			set(C, i, j, ftoken);
//			getchar();
		}
	}

}

int premio_total(Vertice* vertices_tsp, int qtd_n) {
	int premio = 0;
	for (int i = 0; i < qtd_n; ++i) {
		if (strcmp(vertices_tsp[i].tipo, "w") != 0) {
			premio += vertices_tsp[i].premio;
		}
	}

	return premio;
}

void free_matriz(Matriz* mat) {
	/*
	 printf("a\n");
	 for (int i = 0; i < mat->linha; ++i){
	 printf("b[%d]\n",i);
	 free(mat->v[i]);
	 }
	 printf("c\n");
	 free(mat->v);
	 printf("d\n");
	 //free(mat);
	 printf("e\n");
	 */
	free(mat->v);
	free(mat);
}

std::string get_str_between_two_str(const std::string &s,
		const std::string &start_delim, const std::string &stop_delim) {
	unsigned first_delim_pos = s.find(start_delim);
	unsigned end_pos_of_first_delim = first_delim_pos + start_delim.length();
	unsigned last_delim_pos = s.find(stop_delim);

	return s.substr(end_pos_of_first_delim,
			last_delim_pos - end_pos_of_first_delim);
}

