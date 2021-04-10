#ifndef _VERTICE_H_
#define _VERTICE_H_

#include "stdafx.h"
#include "Ponto.h"
#include <vector>
using namespace std;


class Vertice : public Ponto
{
	
public:
	
	int Indice_vertice;
	vector<int> Vizinhos;
	double Signal_counter;
	bool foiVencedor;

	void Atualizar_Posicao(Ponto amostra);
	void Set_Indice(int i);
	void Atualizar_Posicao(Ponto* amostra);
	void Atualizar_Posicao_suave(Ponto amostra);
	void Atualizar_Posicao_suave(Ponto* amostra);
	void Atualizar_Posicao_vizinho(Ponto* amostra);
	void Inserir_Vizinhos(int indice);
	void Inserir_Vizinhos(Vertice* vertice);
	Vertice();
	Vertice(double x, double y, double z);
	Ponto* Normal();
	Ponto* Subtracao(Vertice* vertice);
	void Remover_vizinho(int indice);
	void Remover_todos_vizinho();
	bool eh_vizinho(int indice_vert);
	void atualizar_indices_vizinhos(int indice);
	bool equals(Vertice* vertice);
	bool equals(int indice_vertice);
};

#endif // #ifndef _VERTICE_H_