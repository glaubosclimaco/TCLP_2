/*
 * SampleDecoder.h
 *
 * Any decoder must have the format below, i.e., implement the method decode(std::vector< double >&)
 * returning a double corresponding to the fitness of that vector. If parallel decoding is to be
 * used in the BRKGA framework, then the decode() method _must_ be thread-safe; the best way to
 * guarantee this is by adding 'const' to the end of decode() so that the property will be checked
 * at compile time.
 *
 * The chromosome inside the BRKGA framework can be changed if desired. To do so, just use the
 * first signature of decode() which allows for modification. Please use double values in the
 * interval [0,1) when updating, thus obeying the BRKGA guidelines.
 *
 *  Created on: Jan 14, 2011
 *      Author: rtoso
 */

#ifndef SAMPLEDECODER_H
#define SAMPLEDECODER_H

#include <list>
#include <vector>
#include <algorithm>
#include "utils.h"

typedef struct cromossomo {
	int indice;
	double valor;
} cromossomo;

class SampleDecoder {
public:
	SampleDecoder(int premio_requerido, int pen_total, Vertice* vertices,
			double** matriz_custo);
	~SampleDecoder();

	double decode(std::vector<double>& chromosome);

	int pen_total;
	Vertice* vertices;
	double** matriz_custo;
	int premio_requerido;

	static bool smaller_first(cromossomo a, cromossomo b) {
		return a.valor < b.valor;
	}

	double custo_rota(int nv, double **m,
			std::vector<std::pair<double, unsigned> > r);
	void swap(std::vector<std::pair<double, unsigned> >& r, int a, int b);
	void ajustar_cromossomo(int nv, std::vector<double>& chromosome,
			std::vector<std::pair<double, unsigned> > r);
	void print(int nv, std::vector<std::pair<double, unsigned> > r);

private:
};

#endif
