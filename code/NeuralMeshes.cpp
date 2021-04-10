// NeuralMeshes.cpp : Defines the entry point for the console application.
//

#include "stdafx.h"
#include "Ponto.h"
#include "Vertice.h"
#include "Amostragem.h"
#include "Mesh.h"
#include <iostream>
using namespace std;

void basic_step(Amostragem *amostragem, Mesh* NeuralMesh)
{
	Vertice* vencedor;

	//basic step

	for(int i = 0; i< 50; i++)
	{
		Ponto* ponto_aleatorio = amostragem->SortearAleaoriamente();
		/*
		cout << "Ponto Sorteado" << endl;

		cout << "x = " << ponto_aleatorio->coord_x << endl;
		cout << "y = " << ponto_aleatorio->coord_y << endl;
		cout << "z = " << ponto_aleatorio->coord_z << endl;
		*/
		vencedor = NeuralMesh->Encontrar_Vencedor(ponto_aleatorio);

		vencedor->Atualizar_Posicao(ponto_aleatorio);
		
		for(int i = 0; i < vencedor->Vizinhos.size(); i++)
		{
			int indice_vizinho = vencedor->Vizinhos[i];
			Vertice* vizinho_i = NeuralMesh->MeshNetworking[indice_vizinho];
			vizinho_i->Atualizar_Posicao_vizinho(ponto_aleatorio);
		}
		
		/*
		for(int k = 0; k < 1; k++)
		{
		//analisar o codigo e a definição dessa suavização
			NeuralMesh->Suavizacao_Laplaciana(vencedor);
		}
		*/
		NeuralMesh->Atualizar_Contador_Sinal(vencedor);
	}
}

int _tmain(int argc, _TCHAR* argv[])
{
	cout << "Neural Meshes 1.0!" << endl;

	//teste
	/*
	Amostragem* amostragem = new Amostragem("entrada.txt");

	for(int i = 0; i < amostragem->Numero_pontos; i++)
	{
		Ponto* ponto = amostragem->NuvemPontos[i];
		cout << "x = " << ponto->coord_x << endl;
		cout << "y = " << ponto->coord_y << endl;
		cout << "z = " << ponto->coord_z << endl;
	}

	Ponto* ponto = amostragem->SortearAleaoriamente();

	cout << "Ponto Sorteado" << endl;

	cout << "x = " << ponto->coord_x << endl;
	cout << "y = " << ponto->coord_y << endl;
	cout << "z = " << ponto->coord_z << endl;
	
	Vertice* v0 = new Vertice(1,0,0);

	Ponto* normal =  v0->Normal();

	v0->Inserir_Vizinhos(1);

	cout << "vi = " << v0->Vizinhos.at(0) << endl;
	cout << "vid = " << v0->Vizinhos[0] << endl;
	/*
	cout << "Mesh" << endl;

	Mesh* mesh = new Mesh();

	for(int i = 0; i < mesh->MeshNetworking.size(); i++)
	{
		Vertice* vet = mesh->MeshNetworking[i];
		cout << "x = " << vet->coord_x << endl;
		cout << "y = " << vet->coord_y << endl;
		cout << "z = " << vet->coord_z << endl << endl;
	}
	*/

	Amostragem* amostragem = new Amostragem("amostragem_out_max.ply");
	Mesh* NeuralMesh = new Mesh();
	int i = 0;
	while(NeuralMesh->qnt_vertices() < 2000)
	{
		basic_step(amostragem, NeuralMesh);
				
		//Duplicação do Vértice
		Vertice* v = NeuralMesh->Vertice_Maior_Sinal();		
		NeuralMesh->Dividir_Regiao(v);
		
		if(((++i) % 5) == 0)
		{
			//Half_edge_Collapse
			NeuralMesh->half_edge_collapse();
		}		
	}		

	NeuralMesh->imprimir_ply();

	system("PAUSE");
	return 0;
}

