/*
 * SampleDecoder.cpp
 *
 *  Created on: Jan 14, 2011
 *      Author: rtoso
 */

#include "SampleDecoder.h"

SampleDecoder::SampleDecoder(int _premio_requerido, int _pen_total,
		Vertice* _vertices, double** _matriz_custo) :
		premio_requerido(_premio_requerido), pen_total(_pen_total), vertices(
				_vertices), matriz_custo(_matriz_custo) {

}

SampleDecoder::~SampleDecoder() {
}

// Runs in \Theta(n \log n):
double SampleDecoder::decode(std::vector<double>& chromosome) {
	std::vector<std::pair<double, unsigned> > ranking(chromosome.size());
	int premio_solucao = 0;
	int penalidades_solucao = 0;
	double custo = 0.0, fo = 0.0;
	std::vector<std::pair<double, unsigned> > temp;
	int nv = 0;
	int debug = 0;

	for (unsigned i = 0; i < chromosome.size(); ++i) {
		ranking[i] = std::pair<double, unsigned>(chromosome[i], i);
	}

	// Here we sort 'permutation', which will then produce a permutation of [n] in pair::second:
	std::sort(ranking.begin(), ranking.end());

	// permutation[i].second is in {0, ..., n - 1}; a permutation can be obtained as follows
	std::list<unsigned> permutation;
	int antecessor = ranking[0].second;
	penalidades_solucao += vertices[antecessor].penalidade;
	premio_solucao += vertices[antecessor].premio;
	nv++;

//	printf("premio requerido: %d \n", premio_requerido);
//	printf("solucao: \n");

////	cout << chromosome[0] << endl;
//	getchar();

//	printf("%d(%d)[%d] ->\t", antecessor, vertices[antecessor].premio,
//			vertices[antecessor].penalidade);
	for (int j = 1; j < ranking.size(); j++) {
		int v = ranking[j].second;

		premio_solucao += vertices[v].premio;
		penalidades_solucao += vertices[v].penalidade;
		custo += matriz_custo[antecessor][v];
//		printf("- %0.f - ", matriz_custo[antecessor][v]);

		antecessor = v;
//		printf("%d(%d)[%d]-> ", antecessor, vertices[antecessor].premio,
//				vertices[antecessor].penalidade);

		nv++;

		if (premio_solucao >= premio_requerido) {
			break;
		}

	}

//	printf("\n");
////
//	printf("premio coletado: %d \n", premio_solucao);

//custo de voltar ao vertice de origem
	custo += matriz_custo[ranking[0].second][antecessor];

//	printf("custo da rota construida: %0.f \n", custo);
//	printf("numero de vertices na sol: %d \n", nv);
//	printf("pen total: %d \n", pen_total);
//	printf("penalidades_solucao: %d \n", penalidades_solucao);

	fo = custo + (pen_total - penalidades_solucao);
//
//	printf("a soma no custo: %d \n", (pen_total - penalidades_solucao));
//
	if (debug)
		printf("fo construida: %0.f \n", fo);

//	getchar();

	/*
	 * busca local:
	 *
	 * */
	temp = ranking;

//	printf("sol construida: \n");

//	print(nv, ranking);

	for (int i = 0; i < nv; i++) {
		for (int j = 0; j < nv; j++) {
			if (i == j)
				continue;
			std::vector<std::pair<double, unsigned> > aux;
			aux = temp;
			swap(aux, i, j);
			double c1 = custo_rota(nv, matriz_custo, aux);
			double c2 = custo_rota(nv, matriz_custo, temp);

//			printf("c1= %f e  c2= %f", c1, c2);
//
//			getchar();

			if (c1 < c2) {
				temp = aux;
//				printf("melhorou de %f para %f", c1, c2);
//				getchar();

			}
		}
	}

	double cbl = custo_rota(nv, matriz_custo, temp);
//	printf("\ncusto rota bl: %f \n", cbl);

	double nova_fo = cbl + (pen_total - penalidades_solucao);

	if (debug)
		printf("fo bl_1 = %f\n", nova_fo);
//
//	printf("sol BL: \n");
//
//	print(nv, temp);

//	ajustar_cromossomo

	/*
	 * busca local 2: a partir da sol construida
	 *
	 * */

	std::vector<std::pair<double, unsigned> > bl2;

	bl2 = temp;

	for (int i = 0; i < nv; i++) {
		for (int j = nv + 1; j < ranking.size(); j++) {
			if (i == j)
				continue;
			std::vector<std::pair<double, unsigned> > aux;
			aux = bl2;
			swap(aux, i, j);
			double c1 = custo_rota(nv, matriz_custo, aux);
			double c2 = custo_rota(nv, matriz_custo, bl2);

			//			printf("c1= %f e  c2= %f", c1, c2);
			//
			//			getchar();

			if (c1 < c2) {
				bl2 = aux;
				//				printf("melhorou de %f para %f", c1, c2);
				//				getchar();

			}
		}
	}
	double cbl2 = custo_rota(nv, matriz_custo, bl2);
	double fo_bl2 = cbl2 + (pen_total - penalidades_solucao);

	if (debug)
		printf("fo bl2: %f \n", fo_bl2);
//	getchar();

	if (cbl2 < cbl) {
		ajustar_cromossomo(nv, chromosome, bl2);
	} else {
		ajustar_cromossomo(nv, chromosome, bl2);
	}
	return fo;
}

double SampleDecoder::custo_rota(int nv, double **m,
		std::vector<std::pair<double, unsigned> > r) {
//	printf("funcao de custo \n");
	double c = 0.0;
	int antecessor = r[0].second;
//	printf("%d -> ", antecessor);
	for (int i = 1; i < nv; i++) {
		int v = r[i].second;
		c += m[antecessor][v];

		antecessor = v;
//		printf("%d -> ", antecessor);
	}
//	printf("\n");
	c += m[antecessor][r[0].second];
	return c;
}

void SampleDecoder::swap(std::vector<std::pair<double, unsigned> >& r, int a,
		int b) {
	pair<double, unsigned> aux;
	aux = r[a];
	r[a] = r[b];
	r[b] = aux;
}

void SampleDecoder::ajustar_cromossomo(int nv, std::vector<double>& chromosome,
		std::vector<std::pair<double, unsigned> > r) {

	for (unsigned i = 0; i < nv; i++) {
		unsigned index = r[i].second;
		chromosome[index] = r[i].first;
	}
}
//
void SampleDecoder::print(int nv, std::vector<std::pair<double, unsigned> > r) {
	for (unsigned i = 0; i < nv; i++) {
		printf("%d -> ", r[i].second);
	}
	printf("\n");
}

