#include "stdafx.h"
#include "Mesh.h"
#include <math.h>
#include <iostream>
#include <fstream>
using namespace std;

Mesh::Mesh()
{
	//Inicialmente a neural mesh é um cubo
	//eh saber qual deve ser o tamanho desse cubo
	
	Vertice* v0 = new Vertice(0.5,0.5,0.5);
	v0->Set_Indice(0);
	Vertice* v1 = new Vertice(-0.5,0.5,0.5);
	v1->Set_Indice(1);
	Vertice* v2 = new Vertice(0.5,-0.5,0.5);
	v2->Set_Indice(2);
	Vertice* v3 = new Vertice(-0.5,-0.5,0.5);
	v3->Set_Indice(3);
	Vertice* v4 = new Vertice(0.5,0.5,-0.5);
	v4->Set_Indice(4);
	Vertice* v5 = new Vertice(-0.5,0.5,-0.5);
	v5->Set_Indice(5);
	Vertice* v6 = new Vertice(0.5,-0.5,-0.5);
	v6->Set_Indice(6);
	Vertice* v7 = new Vertice(-0.5,-0.5,-0.5);
	v7->Set_Indice(7);

	//vizinhos v0
	v0->Inserir_Vizinhos(1);
	v0->Inserir_Vizinhos(2);
	v0->Inserir_Vizinhos(4);
	//vizinhos v1
	v1->Inserir_Vizinhos(0);
	v1->Inserir_Vizinhos(2);
	v1->Inserir_Vizinhos(3);
	v1->Inserir_Vizinhos(4);
	v1->Inserir_Vizinhos(5);
	//vizinhos v2
	v2->Inserir_Vizinhos(0);
	v2->Inserir_Vizinhos(1);
	v2->Inserir_Vizinhos(3);
	v2->Inserir_Vizinhos(4);
	v2->Inserir_Vizinhos(6);
	//vizinhos v3
	v3->Inserir_Vizinhos(1);
	v3->Inserir_Vizinhos(2);
	v3->Inserir_Vizinhos(5);
	v3->Inserir_Vizinhos(6);
	v3->Inserir_Vizinhos(7);
	//vizinhos v4
	v4->Inserir_Vizinhos(0);
	v4->Inserir_Vizinhos(1);
	v4->Inserir_Vizinhos(2);
	v4->Inserir_Vizinhos(5);
	v4->Inserir_Vizinhos(6);
	//vizinhos v5
	v5->Inserir_Vizinhos(1);
	v5->Inserir_Vizinhos(3);
	v5->Inserir_Vizinhos(4);
	v5->Inserir_Vizinhos(6);
	v5->Inserir_Vizinhos(7);
	//vizinhos v6
	v6->Inserir_Vizinhos(2);
	v6->Inserir_Vizinhos(3);
	v6->Inserir_Vizinhos(4);
	v6->Inserir_Vizinhos(5);
	v6->Inserir_Vizinhos(7);	
	//vizinhos v7
	v7->Inserir_Vizinhos(3);
	v7->Inserir_Vizinhos(5);
	v7->Inserir_Vizinhos(6);

	//add na lista de vertices

	this->MeshNetworking.push_back (v0);
	this->MeshNetworking.push_back (v1);
	this->MeshNetworking.push_back (v2);
	this->MeshNetworking.push_back (v3);
	this->MeshNetworking.push_back (v4);
	this->MeshNetworking.push_back (v5);
	this->MeshNetworking.push_back (v6);
	this->MeshNetworking.push_back (v7);
	
}

void Mesh::Suavizacao_Laplaciana(Vertice* vencedor)
{
	//Para implementar o Laplaciana smoothing (tem que testar)
	
	for(int i = 0; i < vencedor->Vizinhos.size(); i++)
	{
		int indice_vizinho = vencedor->Vizinhos[i];
		Vertice* vizinho_i = this->MeshNetworking[indice_vizinho];
		
		Ponto* vetorLaplaciano = new Ponto();

		for(int k = 0; k < vizinho_i->Vizinhos.size(); k++)
		{
			int indice_vizinho_k = vizinho_i->Vizinhos[k];
			Vertice* vizinho_k = this->MeshNetworking[indice_vizinho_k];

			Ponto* vetor_subt = vizinho_k->Subtracao(vizinho_i);
			vetorLaplaciano->soma(vetor_subt);
		}
		double valencia = vizinho_i->Vizinhos.size();
		vetorLaplaciano = vetorLaplaciano->produtoComEscalar((1/valencia));
		Ponto* Normal = vizinho_i->Normal();
		double prod_escalar = vetorLaplaciano->produtoEscalar(Normal);
		Ponto* temp = Normal->produtoComEscalar(prod_escalar);
		vetorLaplaciano = vetorLaplaciano->subtracao(temp);

		vizinho_i->Atualizar_Posicao_suave(vetorLaplaciano);
		//M' = M + alpha_n*vetorLaplaciano

	}

}

Vertice* Mesh::Encontrar_Vencedor(Ponto* ponto)
{
	//no artigo original, a busca é feita através de
	//estrutura de dados chamada octree
	
	//de início vou implementar uma busca linear (binária será que dá??)

	Vertice* vencedor = this->MeshNetworking[0];

	double min_dist = this->MeshNetworking[0]->distancia(ponto);
	
	for(int i = 1; i < this->MeshNetworking.size(); i++)
	{
		double tempDist = this->MeshNetworking[i]->distancia(ponto);

		if(tempDist < min_dist)
		{			
			vencedor = this->MeshNetworking[i];
			min_dist = tempDist;
		}

	}
	return vencedor; 
}

void Mesh::Atualizar_Contador_Sinal(Vertice* vencedor)
{
	double gamma = 6;
	double expoente = 1/(gamma*this->MeshNetworking.size());
	double alpha_v = pow (0.5,expoente);

	for(int i = 0; i < this->MeshNetworking.size(); i++)
	{
		if(i != vencedor->Indice_vertice)
		{
			this->MeshNetworking[i]->Signal_counter *= alpha_v;
		}
		else
		{
			this->MeshNetworking[i]->Signal_counter += 1;
		}
	}
}

Vertice* Mesh::Vertice_Maior_Sinal()
{
	Vertice* retorno = this->MeshNetworking[0];

	double maior_sinal = this->MeshNetworking[0]->Signal_counter;
	
	for(int i = 1; i < this->MeshNetworking.size(); i++)
	{
		double temp_sinal = this->MeshNetworking[i]->Signal_counter;

		if(maior_sinal < temp_sinal)
		{			
			retorno = this->MeshNetworking[i];
			maior_sinal = temp_sinal;
		}

	}
	return retorno; 
}

Ponto* Mesh::Novo_Vertice(Vertice* vertice_pai)
{
	double max_dist = 0;
	Vertice* vizinho_max_dist;

	//encontrando vizinho mais distante (maior aresta)
	for(int i = 0; i < vertice_pai->Vizinhos.size(); i++)
	{
		int indice_vizinho = vertice_pai->Vizinhos[i];
		Vertice* vizinho_i = this->MeshNetworking[indice_vizinho];
		
		double temp_dist = vizinho_i->distancia(vertice_pai);
		
		if(max_dist < temp_dist)
		{			
			vizinho_max_dist = vizinho_i;
			max_dist = temp_dist;
		}
	}

	Ponto* aresta = vizinho_max_dist->Subtracao(vertice_pai);
	
	Ponto* meta_aresta = aresta->produtoComEscalar(0.5);

	Ponto* retorno = vertice_pai->soma_ret(meta_aresta);

	return retorno;
}

void Mesh::Dividir_Regiao(Vertice* vertice_pai, Ponto* novo_ponto)
{
	//vetor do vertice pai ao novo vertice
	Ponto* vetor_e = vertice_pai->subtracao(novo_ponto);
	vetor_e = vetor_e->produtoComEscalar(-1);

	double pri_menor = 999999;
	double seg_menor = 999999;

	int pri_vizinho = 0;
	int seg_vizinho = 0;

	int* array_vizinhos = new int[vertice_pai->Vizinhos.size()];
	int quant_vizinhos = vertice_pai->Vizinhos.size();

	for(int i = 0; i < vertice_pai->Vizinhos.size(); i++)
	{		
		//vizinho i
		int indice_vizinho = vertice_pai->Vizinhos[i];
		Vertice* vizinho_i = this->MeshNetworking[indice_vizinho];

		//vetor de vizinhos
		array_vizinhos[i] = indice_vizinho;

		//vetor (vizinho_i - vertice_pai)
		Ponto* vetor_temp = vizinho_i->subtracao(vertice_pai);

		//produto escalar entre o vetor (vetor_e)*(vetor_temp)
		double prod_escalar = vetor_temp->produtoEscalar(vetor_e);

		//valor absoluto
		if(prod_escalar < 0)
		{
			prod_escalar *= -1;
		}

		//escolhendo os dois menores produto escalares
		if(prod_escalar < seg_menor)
		{
			if(prod_escalar < pri_menor)
			{
				seg_menor = pri_menor;
				seg_vizinho = pri_vizinho;
				pri_menor = prod_escalar;
				pri_vizinho = indice_vizinho;
			}
			else
			{
				seg_menor = prod_escalar;
				seg_vizinho = indice_vizinho;
			}
		}
	}
	//criando novo vertice propriamente dito
	Vertice* vert_novo = new Vertice(novo_ponto->coord_x,novo_ponto->coord_y,novo_ponto->coord_z);
	vert_novo->Set_Indice(this->MeshNetworking.size());
	vert_novo->Inserir_Vizinhos(pri_vizinho);
	vert_novo->Inserir_Vizinhos(seg_vizinho);
	vert_novo->Inserir_Vizinhos(vertice_pai->Indice_vertice);
	//era bom fazer um metodo de inserir na neuralmesh
	//para garantir que o novo vertice está consistente
	this->MeshNetworking.push_back(vert_novo);

	//apagando todos os vizinhos do vertice pai

	vertice_pai->Remover_todos_vizinho();
	vertice_pai->Inserir_Vizinhos(pri_vizinho);
	vertice_pai->Inserir_Vizinhos(seg_vizinho);
	vertice_pai->Inserir_Vizinhos(vert_novo->Indice_vertice);

	for(int i = 0; i < quant_vizinhos; i++)
	{

	}
	
	//dividir a região propriamente dita e atualizar o contador de sinal

}

void Mesh::Imprimir_Mesh()
{
	/*
	 cout << "Criando arquivo PLY" << endl;     
//Arquivos
     fstream arquivoPLY;
     
//Abrindo os Arquivos
     arquivoPLY.open ("tools\\arquivoVisualisacao.ply");

//Numero de pontos amostrais
     int num_vertices;

//Numero de faces
     int num_faces;
     aresta >> num_faces;

//Cabeçalho
     arquivoPLY << "ply" << "\n";
     arquivoPLY << "format ascii 1.0" << "\n";
     arquivoPLY << "element vertex " << num_vertices << "\n";
     arquivoPLY << "property float x" << "\n" << "property float y" << "\n" << "property float z" << "\n";
     arquivoPLY << "element face " << numTriangulos << "\n";
     arquivoPLY << "property list uchar int vertex_indices" << "\n";
     arquivoPLY << "end_header" << "\n";

//Escrita da coordenadas das amostras

     int count =  num_vertices_Amostras_polos; // vai mudar (ja modifiquei)
     double coor_x, coor_y, coor_z;
     int i;
     int novo_indice = 0;
     int* novos_indices = new int[count];
     
     
     for(i = 0; i < count; i++){
           pontos >> coor_x;
           pontos >> coor_y;
           pontos >> coor_z;
           
           if(eh_amostras[i]){
                  novos_indices[i] = novo_indice;
                  novo_indice++;
                  arquivoPLY << coor_x << " " << coor_y << " " << coor_z << "\n";
           }
     }

//Escrita das arestas
     
     int i_ponto;
     
     for(i=0;i < num_faces; i++){
           int coun_i;    
           aresta >> coun_i;
           
           if(coun_i < 3){
                     
                int j;
                for(j = 0; j < coun_i; j++){
                 
                      aresta >> i_ponto;
                }
           }else{
                 arquivoPLY << coun_i << " ";
                 int j;
                 for(j = 0; j < coun_i; j++){
                 
                       aresta >> i_ponto;
                       arquivoPLY << novos_indices[i_ponto] << " ";
                 }

                 arquivoPLY << "\n";     
           }
     }
     
     pontos.close();
     aresta.close();
     arquivoPLY.close();
     amostragem.close();
	 */
}
