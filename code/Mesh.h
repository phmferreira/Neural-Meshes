#ifndef _MESH_H_
#define _MESH_H_

#include "stdafx.h"
#include "Vertice.h"
#include "Face.h"
#include "Aresta.h"
#include <vector>
using namespace std;

class Mesh
{
public:
	
	vector<Vertice*> MeshNetworking;
	vector<Aresta*> Arestas;
	vector<Face*> Faces;
	Mesh();
	Vertice* Encontrar_Vencedor(Ponto* ponto);
	void Suavizacao_Laplaciana(Vertice* vencedor);
	void Atualizar_Contador_Sinal(Vertice* vencedor);

	void Imprimir_Mesh();
	void imprimir_ply();
	void imprimir_vrml();

	Vertice* Vertice_Maior_Sinal();
	Ponto* Novo_Vertice(Vertice* vertice_pai);
	void Dividir_Regiao(Vertice* vertice_pai, Ponto* novo_ponto);
	void Dividir_Regiao(Vertice* vertice_pai);

	//Half-edge collapse
	Vertice* vertice_menor_sinal();
	void half_edge_collapse();
	vector<int> encontar_a_b_c_d(Vertice* vertice);
	double funcao_minimizar(double a, double b, double c, double d);
	double funcao_minimizar_borda(double a, double b, double c);
	void remover_vertice(Vertice* vertice);


	void atualizar_indices(int indice);	
	Aresta* get_aresta(Vertice* vertice_a, Vertice* vertice_b);
	Face* get_face(Aresta* aresta1, Aresta* aresta2, Aresta* aresta3);
	int verificar_aresta(Vertice* vertice_a, Vertice* vertice_b);
	int verificar_face(Aresta* aresta1, Aresta* aresta2, Aresta* aresta3);
	
	void dividir_signal_counter(Vertice* vertice_pai,Vertice* vert_novo);

	//Vertice
	Vertice* criar_novo_vertice_na_rede(Ponto* ponto);
	vector<Face*> buscar_faces_do_vertice(Vertice* vertice);
	vector<Aresta*> buscar_aresta_do_vertice(Vertice* vertice);

	//Arestas
	void gerar_arestas();
	Aresta* buscar_maior_aresta(Vertice* vertice);
	void remover_aresta(Aresta* aresta);
	void remover_aresta(int indice1, int indice2);

	//Fases
	vector<Face*> buscar_faces_da_aresta(Aresta* aresta);
	void remover_face(Face* face);
	bool existe_face(int a, int b, int c);
	int qnt_vertices();
	
};

#endif // #ifndef _MESH_H_