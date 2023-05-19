/*
 * utils.h
 *
 *  Created on: 30 de ago de 2018
 *      Author: user
 */

#ifndef UTILS_H_
#define UTILS_H_


/*
 * instancia.cpp
 *
 *  Created on: 16/07/2012
 *      Author: rogerio
 */
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <fstream>
#include <string>
#include <iostream>
#include <float.h>

using namespace std;

//Tipo Estruturado para vertice
typedef struct vertice {
	int id;
	int x;
	int y;
	char tipo[4];
	int premio;
	int *cobre; //conjunto de vertices W, que este vertice V cobre
	int *coberto_por; //conjunto de vertices V, que cobre o este W
	int qtd_cobre;
	int qtd_coberto_por;
	int reduzido;
	int penalidade;
} Vertice;

//Tipo Estruturado para Matriz e funcoes relacionadas
typedef struct matriz {
	int linha;
	int coluna;
	float** v;
} Matriz;

float get(Matriz* mat, int i, int j);
void set(Matriz* mat, int i, int j, float valor);




#endif /* UTILS_H_ */
