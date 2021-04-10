#ifndef _ARESTA_H_
#define _ARESTA_H_

#include "stdafx.h"
#include "Ponto.h"
#include "Vertice.h"
using namespace std;

class Aresta
{
public:
	Vertice* vertice_1;
	Vertice* vertice_2;
	Aresta(Vertice* vertice1,Vertice* vertice2);
	bool equals(int indice1, int indice2);
	bool equals(Aresta* aresta);
	bool ehConectadas(Aresta* aresta);
	bool ehConectadas(int indice1, int indice2);
	int intersecao(Aresta* aresta);
	double tamanho();
	bool temVertice(int indice);
	bool temVertice(Vertice* vertice);
	Ponto* ponto_central();
	Vertice* diferenca(Aresta* aresta);
	void remover_vizinhos();
	void atualizar(int a, Vertice* novo_vertice);
	void atualizar(Vertice* a, Vertice* novo_vertice);
	bool eh_conectada();
};

#endif // #ifndef _ARESTA_H_