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
	
	gerar_arestas();
	
	//Faces Iniciais
	get_face(Arestas[0],Arestas[2],Arestas[1]);
	get_face(Arestas[2],Arestas[7],Arestas[6]);
	get_face(Arestas[1],Arestas[3],Arestas[5]);
	get_face(Arestas[5],Arestas[13],Arestas[11]);
	get_face(Arestas[3],Arestas[4],Arestas[0]);
	get_face(Arestas[8],Arestas[4],Arestas[10]);
	get_face(Arestas[14],Arestas[16],Arestas[17]);
	get_face(Arestas[10],Arestas[13],Arestas[14]);
	get_face(Arestas[12],Arestas[15],Arestas[17]);
	get_face(Arestas[7],Arestas[11],Arestas[12]);
	get_face(Arestas[16],Arestas[9],Arestas[15]);
	get_face(Arestas[6],Arestas[8],Arestas[9]);
	
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

	vencedor->foiVencedor = true;

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
	
	double pri_menor = numeric_limits<double>::max( );
	double seg_menor = numeric_limits<double>::max( );

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

		//projeção do vertor_temp sobre o plano do vetor_e
		//vetor_temp->projecao(vetor_e);

		//produto escalar entre o vetor (vetor_e)*(vetor_temp)
		double prod_escalar = vetor_temp->produtoEscalar(vetor_e);

		//valor absoluto
		if(prod_escalar < 0)
		{
			prod_escalar *= -1;
		}

		//escolhendo os dois menores produto escalares 
		//(talvez esse critério seja suficiente)
		//*mas era bom garantir que esse vetores sejam
		//*tenham mesma direção e com sentidos opostos
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

	//faltou apagar o indice pai dos vizinhos dele eu acho

	vertice_pai->Remover_todos_vizinho();
	vertice_pai->Inserir_Vizinhos(pri_vizinho);
	vertice_pai->Inserir_Vizinhos(seg_vizinho);
	vertice_pai->Inserir_Vizinhos(vert_novo->Indice_vertice);

	for(int i = 0; i < quant_vizinhos; i++)
	{
		int vizinho_atual = array_vizinhos[i];
		if(vizinho_atual != pri_vizinho && vizinho_atual != seg_vizinho)
		{
			Vertice* tempVizinho = this->MeshNetworking[vizinho_atual];
			double dist_pai = vertice_pai->distancia(tempVizinho);
			double dist_filho = vert_novo->distancia(tempVizinho);

			if(dist_filho < dist_pai)
			{
				vert_novo->Inserir_Vizinhos(vizinho_atual);
			}else{
				vertice_pai->Inserir_Vizinhos(vizinho_atual);
			}

		}
	}

	//dividir o contador de sinal

	double VoronoiRegPai = 0;
	double VoronoiRegFilho = 0;

	for(int i = 0; i < vertice_pai->Vizinhos.size(); i++)
	{
		//vizinho i
		int indice_vizinho = vertice_pai->Vizinhos[i];
		Vertice* vizinho_i = this->MeshNetworking[indice_vizinho];

		VoronoiRegPai += vertice_pai->distancia(vizinho_i);
	}

	for(int i = 0; i < vert_novo->Vizinhos.size(); i++)
	{
		//vizinho i
		int indice_vizinho = vert_novo->Vizinhos[i];
		Vertice* vizinho_i = this->MeshNetworking[indice_vizinho];

		VoronoiRegFilho += vert_novo->distancia(vizinho_i);
	}

	VoronoiRegPai = VoronoiRegPai/vertice_pai->Vizinhos.size();
	VoronoiRegFilho = VoronoiRegFilho/vert_novo->Vizinhos.size();
	double soma = VoronoiRegPai + VoronoiRegFilho;
	vert_novo->Signal_counter = (VoronoiRegFilho/soma)*(vertice_pai->Signal_counter);
	vertice_pai->Signal_counter = (VoronoiRegPai/soma)*(vertice_pai->Signal_counter);

	// não sei isso é o correto 
	// mas é o mais intuitivo
	vertice_pai->foiVencedor = true;
	vert_novo->foiVencedor = true;
}

void Mesh::Dividir_Regiao(Vertice* vertice_pai)
{
	cout << "Splite Vertice" << endl;
	Aresta* maior_aresta = this->buscar_maior_aresta(vertice_pai);
	
	//verificar se essa aresta existe de fato???

	Ponto* novo_ponto = maior_aresta->ponto_central();
	Vertice* novo_vertice = criar_novo_vertice_na_rede(novo_ponto);
	//Vertice* novo_vertice = new Vertice(novo_ponto->coord_x,novo_ponto->coord_y,novo_ponto->coord_z);
	vector<Face*> faces_aresta_maior = this->buscar_faces_da_aresta(maior_aresta);
	if(faces_aresta_maior.size() != 2)
	{
		system("PAUSE");
	}
	//intuitivamente a quantidade de faces_aresta_maior é sempre 2
	Vertice* viz_com1 = faces_aresta_maior[0]->vertice_que_falta(maior_aresta);
	Vertice* viz_com2 = faces_aresta_maior[1]->vertice_que_falta(maior_aresta);

	//arestas antigas
		//aresta a ser deletada**
	Aresta* aresta3 = get_aresta(maior_aresta->vertice_1,maior_aresta->vertice_2);
		//aresta a ser deletada**
	Aresta* aresta1 = get_aresta(maior_aresta->vertice_1,viz_com1);
	Aresta* aresta2 = get_aresta(maior_aresta->vertice_1,viz_com2);
	Aresta* aresta4 = get_aresta(maior_aresta->vertice_2,viz_com1);
	Aresta* aresta5 = get_aresta(maior_aresta->vertice_2,viz_com2);

	//novas aresta
	Aresta* nova_aresta1 = get_aresta(maior_aresta->vertice_1,novo_vertice);
	Aresta* nova_aresta2 = get_aresta(maior_aresta->vertice_2,novo_vertice);
	Aresta* nova_aresta3 = get_aresta(viz_com1,novo_vertice);
	Aresta* nova_aresta4 = get_aresta(viz_com2,novo_vertice);

	//Novas Faces
	Face* nova_face1 = new Face(aresta1,nova_aresta1,nova_aresta3);
	Face* nova_face2 = new Face(aresta2,nova_aresta1,nova_aresta4);
	Face* nova_face3 = new Face(aresta4,nova_aresta2,nova_aresta3);
	Face* nova_face4 = new Face(aresta5,nova_aresta2,nova_aresta4);
	//inserindo as faces
	Faces.push_back(nova_face1);
	Faces.push_back(nova_face2);
	Faces.push_back(nova_face3);
	Faces.push_back(nova_face4);
	
	//verificar se é faces
	//remove face
	for(int i=0;i<faces_aresta_maior.size();i++)
	{
		this->remover_face(faces_aresta_maior[i]);
		//cout << "removido i = " << i << endl;
		if(i > 1)
		{
			system("PAUSE");
		}
	}
	//remove aresta
	this->remover_aresta(maior_aresta);

	//atualizar o signal counter
	this->dividir_signal_counter(vertice_pai,novo_vertice);

}

vector<Face*> Mesh::buscar_faces_da_aresta(Aresta* aresta)
{
	vector<Face*> retorno;
	cout << "Bucando Faces da Aresta - Splite Vertice" << endl;
	for(int i=0;i<this->Faces.size();i++)
	{
		this->Faces[i]->atualizar_indices();
		if(this->Faces[i]->possui_aresta(aresta))
		{			
			retorno.push_back(Faces[i]);
		}
	}
	return retorno;
}

Aresta* Mesh::buscar_maior_aresta(Vertice* vertice)
{
	double maior_valor = 0;
	Aresta* retorno = NULL;
	cout << "Bucando maior Aresta - Splite Vertice" << endl;
	for(int i=0;i<this->Arestas.size();i++)
	{
		if(this->Arestas[i]->temVertice(vertice))
		{
			if(!this->Arestas[i]->eh_conectada())
			{
				this->Arestas.erase(Arestas.begin() + i);
				i--;
				system("PAUSE");
				continue;				
			}
			double temp = this->Arestas[i]->tamanho();
			if(temp > maior_valor)
			{
				maior_valor = temp;
				retorno = this->Arestas[i];
				//talvez eu deveria procura 
				//e verificar se é um aresta
				//valida pq está encontrando
				//aresta que deveria está excluidas
			}
		}
	}
	return retorno;
}

Vertice* Mesh::vertice_menor_sinal()
{
	cout << "Bucando vertice com maior Sinal - Splite Vertice" << endl;
	Vertice* retorno = this->MeshNetworking[0];

	double menor_sinal = this->MeshNetworking[0]->Signal_counter;
	
	for(int i = 1; i < this->MeshNetworking.size(); i++)
	{
		double temp_sinal = this->MeshNetworking[i]->Signal_counter;

		if(menor_sinal > temp_sinal)
		{			
			retorno = this->MeshNetworking[i];
			menor_sinal = temp_sinal;
		}

	}
	return retorno; 
}

void Mesh::half_edge_collapse()
{
	cout << "Half-Edge Collapse" << endl;
	// critério de remover o vertice com menor sinal
	Vertice* menor_vertice = this->vertice_menor_sinal();

	//remover vertices propriamente dito
	remover_vertice(menor_vertice);

	//critério de remover vertice menor que o limiar
	/*
	double gamma = 6;
	double miu = 10;
	double expoente = (miu)/(gamma);
	double limiar = pow (0.5,expoente);

	for(int i = 0; i < this->MeshNetworking.size(); i++)
	{
		if(this->MeshNetworking[i]->Signal_counter < limiar)
		{
			//remover vertices propriamente dito
		}
	}
	*/
}

vector<int> Mesh::encontar_a_b_c_d(Vertice* vertice)
{
	cout << "Encontrando a b c d - Half-Edge Collapse" << endl;
	double min_valor = numeric_limits<double>::max( );
	int val_a = vertice->Vizinhos.size();
	int final_b = 0;
	int final_c = 0;
	int final_d = 0;
	bool tem_d = true;
	int a = vertice->Indice_vertice;

	for(int i = 0; i < vertice->Vizinhos.size(); i++)
	{
		//vizinho i
		int b = vertice->Vizinhos[i];		
		Vertice* vizinho_i = this->MeshNetworking[b];

		int val_b = vizinho_i->Vizinhos.size();
				
		int c = -1;
		int d = -1;		
		int j = 0;
		bool parou = false;
		bool ehVizinho_c = false;
		bool ehVizinho_d = false;
		//encontrando c
		while(!parou && (j < vizinho_i->Vizinhos.size()))
		{
			c = vizinho_i->Vizinhos[j];
			//ehVizinho_c = existe_face(a,b,c);
			ehVizinho_c = vertice->eh_vizinho(c);
			//tem que verificar se exite face(a,b,c)
			parou = ehVizinho_c;
			j++;
		}
		parou = false;
		//encontrando d
		while(!parou && (j < vizinho_i->Vizinhos.size()))
		{
			d = vizinho_i->Vizinhos[j];
			//ehVizinho_d = existe_face(a,b,d);
			ehVizinho_d = vertice->eh_vizinho(d);
			//tem que verificar se exite face(a,b,d)
			parou = ehVizinho_d;
			j++;			
		}

		double temp_min = numeric_limits<double>::max( );
		int val_c = this->MeshNetworking[c]->Vizinhos.size();
		bool temp_tem_d = false;

		if(ehVizinho_c && ehVizinho_d)
		{
			int val_d = this->MeshNetworking[d]->Vizinhos.size();
			temp_min = this->funcao_minimizar(val_a,val_b,val_c,val_d);
			temp_tem_d = true;
		}
		else if(ehVizinho_c)
		{
			temp_min = this->funcao_minimizar_borda(val_a,val_b,val_c);
			temp_tem_d = false;
		}
		else
		{
			//error!!!
			cout << "Error! Remoção de vértice - a b c d" << endl;
		}

		if (min_valor > temp_min)
		{
			final_b = b;
			final_c = c;
			final_d = d;
			min_valor = temp_min;
			tem_d = temp_tem_d;
		}
	}

	vector<int> retorno;
	retorno.push_back(final_b);
	retorno.push_back(final_c);
	if(tem_d)
	{
		retorno.push_back(final_d);
	}

	return retorno;
}

vector<Face*> Mesh::buscar_faces_do_vertice(Vertice* vertice)
{
	vector<Face*> retorno;
	cout << "Buscando Faces do Vertice - Half-Edge Collapse" << endl;
	for(int i=0;i<this->Faces.size();i++)
	{
		if(this->Faces[i]->possui_vertice(vertice))
		{			
			retorno.push_back(Faces[i]);
		}
	}
	return retorno;
}

vector<Aresta*> Mesh::buscar_aresta_do_vertice(Vertice* vertice)
{
	vector<Aresta*> retorno;
	cout << "Buscando Arestas do Vertice - Half-Edge Collapse" << endl;
	for(int i=0;i<this->Arestas.size();i++)
	{
		if(this->Arestas[i]->temVertice(vertice))
		{			
			retorno.push_back(Arestas[i]);
		}
	}
	return retorno;
}

void Mesh::remover_vertice(Vertice* vertice)
{
	cout << "Removendo Faces, Arestas e Vertices - Half-Edge Collapse" << endl;
	/*
	double min_valor = numeric_limits<double>::max( );
	int val_a = vertice->Vizinhos.size();
	int final_b = 0;
	int final_c = 0;
	int final_d = 0;
	bool tem_d = true;

	for(int i = 0; i < vertice->Vizinhos.size(); i++)
	{
		//vizinho i
		int indice_vizinho = vertice->Vizinhos[i];		
		Vertice* vizinho_i = this->MeshNetworking[indice_vizinho];

		int val_b = vizinho_i->Vizinhos.size();
		
		//encontrando c e d
		int c = 0;
		int d = 0;		
		int j = 0;
		bool parou = false;
		bool ehVizinho_c = false;
		bool ehVizinho_d = false;

		while(!parou && (j < vizinho_i->Vizinhos.size()))
		{
			c = vizinho_i->Vizinhos[j];
			ehVizinho_c = vertice->eh_vizinho(c);
			parou = ehVizinho_c;
			j++;
		}
		parou = false;
		while(!parou && (j < vizinho_i->Vizinhos.size()))
		{
			d = vizinho_i->Vizinhos[j];
			ehVizinho_c = vertice->eh_vizinho(d);
			parou = ehVizinho_d;
			j++;			
		}

		double temp_min = numeric_limits<double>::max( );
		int val_c = this->MeshNetworking[c]->Vizinhos.size();
		bool temp_tem_d = false;
		if(ehVizinho_c && ehVizinho_d)
		{
			int val_d = this->MeshNetworking[d]->Vizinhos.size();
			temp_min = this->funcao_minimizar(val_a,val_b,val_c,val_d);
			temp_tem_d = true;
		}
		else if(ehVizinho_c)
		{
			temp_min = this->funcao_minimizar_borda(val_a,val_b,val_c);
		}
		else
		{
			//error!!!
		}

		if (min_valor > temp_min)
		{
			final_b = i;
			final_c = c; //talvez não precise
			final_d = d; //talvez não precise
			min_valor = temp_min;
			tem_d = temp_tem_d; //talvez não precise
		}
	}
	
	// para todos os vizinhos de a remove-lo
	//inserir os vizinhos de a em b
	for(int i = 0; i < vertice->Vizinhos.size(); i++)
	{
		//removendo do vizinho a
		this->MeshNetworking[vertice->Vizinhos[i]]->Remover_vizinho(vertice->Indice_vertice);
		//inserindo vizinho em b
		this->MeshNetworking[final_b]->Inserir_Vizinhos(vertice->Vizinhos[i]);
		//inserindo b no vizinho atual
		this->MeshNetworking[vertice->Vizinhos[i]]->Inserir_Vizinhos(final_b);
	}

	/*
	 * Preciso fazer uma modificação*
	 * Há necessidade de atualizar o
	 * indices dos vertice e consequentimento 
	 * dos vizinhos então o vector de vizinho
	 * deve se tornar um vector de Vertice*
	 * ou procurar outra solução
	 *
	 * outra solução é remover e para todos os
	 * indices maiores que o indice de é decrementado
	 * (medida apaliativa é necessário refatorar o codigo
	 * e fazer a primeira solução)
	 *

	atualizar_indices(vertice->Indice_vertice);

	//remover a da neural mesh
	this->MeshNetworking.erase(MeshNetworking.begin() + vertice->Indice_vertice);
	*/
	int a = vertice->Indice_vertice;
	
	vector<int> indices_b_c_d = this->encontar_a_b_c_d(vertice);
	vector<Face*> faces_do_vertice = this->buscar_faces_do_vertice(vertice);
	Vertice* vertice_b = this->MeshNetworking[indices_b_c_d[0]];

	bool removido_face1 = false;
	bool removido_face2 = false;

	if(indices_b_c_d.size() == 3)
	{
		//removendo a-b
		this->remover_aresta(a,indices_b_c_d[0]);
		//removendo a-c
		this->remover_aresta(a,indices_b_c_d[1]);
		//removendo a-d
		this->remover_aresta(a,indices_b_c_d[2]);

		for(int i=0;i<faces_do_vertice.size();i++)
		{
			//remover ou atualizar
			if(faces_do_vertice[i]->equals(a,indices_b_c_d[0],indices_b_c_d[1]))
			{
				//remover face
				//tem que ser primeiro deletado e depois atualizado as faces
				this->remover_face(faces_do_vertice[i]);
				removido_face1 = true;
			}
			else if(faces_do_vertice[i]->equals(a,indices_b_c_d[0],indices_b_c_d[2]))
			{
				//remover face
				this->remover_face(faces_do_vertice[i]);
				removido_face2 = true;
			}
			else if(removido_face1 && removido_face2)
			{
				//altualizar faces
				faces_do_vertice[i]->atualizar(vertice,vertice_b);
				//inserindo arestas
			}
			else
			{
				faces_do_vertice.push_back(faces_do_vertice[i]);
			}
		}		
	}
	else if(indices_b_c_d.size() == 2)
	{
		cout << "Errado! (não devia entrar aqui por enquanto) - Half-Edge Collapse" << endl;
		for(int i=0;i<faces_do_vertice.size();i++)
		{
			//remover ou atualizar
			if(faces_do_vertice[i]->equals(a,indices_b_c_d[0],indices_b_c_d[1]))
			{
				//remover face
				this->remover_face(faces_do_vertice[i]);
			}
			else
			{
				//altualizar faces
				faces_do_vertice[i]->atualizar(a,vertice_b);
			}
		}
		//removendo a-b
		this->remover_aresta(a,indices_b_c_d[0]);
		//removendo a-c
		this->remover_aresta(a,indices_b_c_d[1]);
	}
	else
	{
		cout << "Error! Remoção de vértice 2" << endl;
	}

	atualizar_indices(vertice->Indice_vertice);

	//remover a da neural mesh
	this->MeshNetworking.erase(MeshNetworking.begin() + vertice->Indice_vertice);
	
	//criando teg de removido para vertice
	vertice->Indice_vertice = -1;

	//testando se excluiu todas as aresta de vértice que foi removido
	vector<Aresta*> aresta_do_vertice = this->buscar_aresta_do_vertice(vertice);
	
	if(aresta_do_vertice.size() != 0)
	{
		system("PAUSE");
		vector<Face*> faces_do_vertice_temp = this->buscar_faces_do_vertice(aresta_do_vertice[0]->vertice_1);
		vector<Aresta*> aresta_do_vertice_temp = this->buscar_aresta_do_vertice(this->MeshNetworking[vertice_b->Indice_vertice]);
		for(int i=0;i<aresta_do_vertice.size();i++)
		{
			aresta_do_vertice[i]->atualizar(vertice,vertice_b);
		}
		aresta_do_vertice_temp = this->buscar_aresta_do_vertice(this->MeshNetworking[vertice_b->Indice_vertice]);
	}	
}

void Mesh::atualizar_indices(int indice)
{
	cout << "atualizar indices" << endl;
	for(int i = 0; i < this->MeshNetworking.size(); i++)
	{
		if(this->MeshNetworking[i]->Indice_vertice > indice)
		{
			this->MeshNetworking[i]->Indice_vertice--;
		}

		this->MeshNetworking[i]->atualizar_indices_vizinhos(indice);
	}

}

double Mesh::funcao_minimizar(double a, double b, double c, double d)
{
	double temp1 = pow ((a+b-10),2);
	double temp2 = pow ((c-7),2);
	double temp3 = pow ((d-7),2);
	double retorno = sqrt((temp1+temp2+temp3))/3;

	return retorno;
}

double Mesh::funcao_minimizar_borda(double a, double b, double c)
{	
	double temp1 = pow ((a+b-10),2);
	double temp2 = pow ((c-7),2);
	double retorno = sqrt((temp1+temp2))/3;

	return retorno;
}

void Mesh::Imprimir_Mesh()
{
	//para todos os vertices da NeuralMesh

	for(int i = 0;i < this->MeshNetworking.size();i++)
	{
		Vertice* vertice_i = this->MeshNetworking[i];
		
		//para todos os vizinho de vertice_i
		//construir uma aresta vertice_i vizinho_i_atual
		for(int j = 0;j < vertice_i->Vizinhos.size();j++)
		{
			//vizinho_i_atual
			int indice_vertice_j = vertice_i->Vizinhos[j];
			Vertice* vertice_j = this->MeshNetworking[indice_vertice_j];

			// aresta vertice_i vizinho_i_atual
			Aresta* aresta1 = get_aresta(vertice_i,vertice_j);
			
			//para todos os vizinho de vertice_j
			//construir uma aresta vertice_i vizinho_j_atual
			//construir uma aresta vertice_j vizinho_j_atual
			for(int k = 0; k < vertice_j->Vizinhos.size();k++)
			{
				//vizinho_j_atual
				int indice_vertice_k = vertice_j->Vizinhos[k];
				Vertice* vertice_k = this->MeshNetworking[indice_vertice_k];


				//vertice_k eh vizinho de 
				if(vertice_i->eh_vizinho(indice_vertice_k))
				{					
					Aresta* aresta2 = get_aresta(vertice_i,vertice_k);
					Aresta* aresta3 = get_aresta(vertice_j,vertice_k);

					Face* face = get_face(aresta1,aresta2,aresta3);

					// antes de inserir tem que ver ser já existe aresta e a face
					/*
					Arestas.push_back(aresta1);
					Arestas.push_back(aresta2);
					Arestas.push_back(aresta3);



					Faces.push_back(face);
					*/
				}
			}
		}
	}

	this->imprimir_ply();

}

void Mesh::gerar_arestas()
{
	//para todos os vertices da NeuralMesh

	for(int i = 0;i < this->MeshNetworking.size();i++)
	{
		Vertice* vertice_i = this->MeshNetworking[i];
		
		//para todos os vizinho de vertice_i
		//construir uma aresta vertice_i vizinho_i_atual
		for(int j = 0;j < vertice_i->Vizinhos.size();j++)
		{
			//vizinho_i_atual
			int indice_vertice_j = vertice_i->Vizinhos[j];
			Vertice* vertice_j = this->MeshNetworking[indice_vertice_j];

			// aresta vertice_i vizinho_i_atual
			Aresta* aresta1 = get_aresta(vertice_i,vertice_j);
			
			//para todos os vizinho de vertice_j
			//construir uma aresta vertice_i vizinho_j_atual
			//construir uma aresta vertice_j vizinho_j_atual
			for(int k = 0; k < vertice_j->Vizinhos.size();k++)
			{
				//vizinho_j_atual
				int indice_vertice_k = vertice_j->Vizinhos[k];
				Vertice* vertice_k = this->MeshNetworking[indice_vertice_k];


				//vertice_k eh vizinho de 
				if(vertice_i->eh_vizinho(indice_vertice_k))
				{					
					Aresta* aresta2 = get_aresta(vertice_i,vertice_k);
					Aresta* aresta3 = get_aresta(vertice_j,vertice_k);
				}
			}
		}
	}
}

void Mesh::imprimir_ply()
{
     cout << "Criando arquivo PLY" << endl;

//Arquivos
     fstream arquivoPLY;
     
//Abrindo os Arquivos
     arquivoPLY.open ("arquivoVisualisacao.ply");
     
//Cabeçalho
     arquivoPLY << "ply" << "\n";
     arquivoPLY << "format ascii 1.0" << "\n";
	 arquivoPLY << "element vertex " << this->MeshNetworking.size()  << "\n";
     arquivoPLY << "property float x" << "\n" << "property float y" << "\n" << "property float z" << "\n";
	 arquivoPLY << "element face " << Faces.size() << "\n";
     arquivoPLY << "property list uchar int vertex_indices" << "\n";
     arquivoPLY << "end_header" << "\n";
	 
//Escrita da coordenadas das vértice (nós)
	 for(int i = 0; i < this->MeshNetworking.size(); i++)
	 {
		 Vertice* vert_atual = this->MeshNetworking[i];
		 arquivoPLY << vert_atual->coord_x << " " << vert_atual->coord_y << " " << vert_atual->coord_z << "\n";
	 }

//Escrita das arestas
     
	 for(int i = 0; i < Faces.size(); i++)
	 {
		 Faces[i]->atualizar_indices();
		 arquivoPLY << "3 " << Faces[i]->a << " " << Faces[i]->b << " " << Faces[i]->c << "\n";
	 }
     
     arquivoPLY.close();
	 
}

Aresta* Mesh::get_aresta(Vertice* vertice_a, Vertice* vertice_b)
{
	int indice_aresta = this->verificar_aresta(vertice_a,vertice_b);
	if(indice_aresta == -1)
	{
		Aresta* aresta1 = new Aresta(vertice_a,vertice_b);
		Arestas.push_back(aresta1);
		return aresta1;
	}
	else
	{
		return Arestas[indice_aresta];
	}
}

Face* Mesh::get_face(Aresta* aresta1, Aresta* aresta2, Aresta* aresta3)
{
	int indice_face = this->verificar_face(aresta1,aresta2,aresta3);
	if(indice_face == -1)
	{
		Face* face = new Face(aresta1,aresta2,aresta3);
		Faces.push_back(face);
		return face;
	}
	else
	{
		return Faces[indice_face];
	}
}

int Mesh::verificar_aresta(Vertice* vertice_a, Vertice* vertice_b)
{
	int retorno = -1;
	bool parou = (this->Arestas.size()==0);
	int i = 0;

	int a = vertice_a->Indice_vertice;
	int b = vertice_b->Indice_vertice;

	while(!parou)
	{
		if(this->Arestas[i]->equals(a,b))
		{
			retorno = i;
			parou = true;
		}
		else
		{
			i++;
		}
		if(i == this->Arestas.size())
		{
			parou = true;
		}
	}

	return retorno;
}

int Mesh::verificar_face(Aresta* aresta1, Aresta* aresta2, Aresta* aresta3)
{
	int retorno = -1;
	bool parou = (this->Faces.size()==0);
	int i = 0;

	while(!parou)
	{
		if(this->Faces[i]->equals(aresta1,aresta2,aresta3))
		{
			retorno = i;
			parou = true;
		}
		else
		{
			i++;
		}
		if(i == this->Faces.size())
		{
			parou = true;
		}
	}

	return retorno;
}

void Mesh::imprimir_vrml()
{
	/**
	 fstream op(filePath,ios::out);
 int i, j;
  op<<"#VRML V2.0 utf8\n";
  op<<"#tempo de execucao: "<<rede.tempoExec<<"\n";
  op<<"Shape{\n"
	 <<"appearance Appearance{\n"
         <<"material Material{\n"
         <<"emissiveColor .1 .1 0\n"
         <<"diffuseColor 1 1 0\n"
         <<"}\n}\n"
         <<"geometry IndexedFaceSet {\n"
         <<"coord Coordinate{"
         <<"point [";
  for(i=0;i<rede.vertices.size();i++)
    op<<rede.vertices[i]->x<<" "
           <<rede.vertices[i]->y<<"  "
           <<rede.vertices[i]->z<<"\n";
  op<<"]\n}\ncoordIndex[";

//  cout<<"faces.size() == "<<faces.size()<<"\n";
  for(i=0; i<rede.faces.size(); i++){
    Edge *e = rede.faces[i]->e1;
    VerticeGNG *vr = rede.faces[i]->verticeRestante(e);
    op<<e->v1->posicao<<", "
      <<e->v2->posicao<<", "
      <<vr->posicao<<", -1,\n";
  }
  op<<"]\nsolid FALSE\n}\n}";
  //op<<"]}}";

  op.close();
  */
	
	cout << "Criando arquivo VRML" << endl;

	//Arquivos
     fstream arquivo_vrml;
     
//Abrindo os Arquivos
     arquivo_vrml.open ("arquivo_vrml.vrml");
     
//Cabeçalho
     arquivo_vrml << "#VRML V2.0 utf8\n";
	 arquivo_vrml <<"#tempo de execucao: ?" <<"\n";
	 arquivo_vrml <<"Shape{\n"
	 <<"appearance Appearance{\n"
         <<"material Material{\n"
         <<"emissiveColor .1 .1 0\n"
         <<"diffuseColor 1 1 0\n"
         <<"}\n}\n"
         <<"geometry IndexedFaceSet {\n"
         <<"coord Coordinate{"
         <<"point [";
	 
//Escrita da coordenadas das vértice (nós)
	 for(int i = 0; i < this->MeshNetworking.size(); i++)
	 {
		 Vertice* vert_atual = this->MeshNetworking[i];
		 arquivo_vrml << vert_atual->coord_x << " " << vert_atual->coord_y << " " << vert_atual->coord_z << "\n";
	 }

//Escrita das arestas
     /*
	 for(int i = 0; i < faces.size(); i++)
	 {
		 arquivo_vrml << "3 " << faces[i]->a << " " << faces[i]->b << " " << faces[i]->c << "\n";
	 }
     */
     arquivo_vrml.close();

}

void Mesh::remover_face(Face* face)
{
	for(int i = 0; i < this->Faces.size(); i++)
	{
		if(this->Faces[i]->equals(face))
		{
			this->Faces.erase(Faces.begin() + i);
			break;
		}
	}
}

void Mesh::remover_aresta(Aresta* aresta)
{
	aresta->remover_vizinhos();
	for(int i = 0; i < this->Arestas.size(); i++)
	{
		if(this->Arestas[i]->equals(aresta))
		{
			this->Arestas.erase(Arestas.begin() + i);
			break;
		}
	}
}

void Mesh::remover_aresta(int indice1, int indice2)
{	
	for(int i = 0; i < this->Arestas.size(); i++)
	{
		if(this->Arestas[i]->equals(indice1,indice2))
		{
			this->Arestas[i]->remover_vizinhos();
			this->Arestas.erase(Arestas.begin() + i);
			break;
		}
	}
}

void Mesh::dividir_signal_counter(Vertice* vertice_pai,Vertice* vert_novo)
{
	//dividir o contador de sinal

	double VoronoiRegPai = 0;
	double VoronoiRegFilho = 0;

	for(int i = 0; i < vertice_pai->Vizinhos.size(); i++)
	{
		//vizinho i
		int indice_vizinho = vertice_pai->Vizinhos[i];
		Vertice* vizinho_i = this->MeshNetworking[indice_vizinho];

		VoronoiRegPai += vertice_pai->distancia(vizinho_i);
	}

	for(int i = 0; i < vert_novo->Vizinhos.size(); i++)
	{
		//vizinho i
		int indice_vizinho = vert_novo->Vizinhos[i];
		Vertice* vizinho_i = this->MeshNetworking[indice_vizinho];

		VoronoiRegFilho += vert_novo->distancia(vizinho_i);
	}

	VoronoiRegPai = VoronoiRegPai/vertice_pai->Vizinhos.size();
	VoronoiRegFilho = VoronoiRegFilho/vert_novo->Vizinhos.size();
	double soma = VoronoiRegPai + VoronoiRegFilho;
	vert_novo->Signal_counter = (VoronoiRegFilho/soma)*(vertice_pai->Signal_counter);
	vertice_pai->Signal_counter = (VoronoiRegPai/soma)*(vertice_pai->Signal_counter);

	// não sei isso é o correto 
	// mas é o mais intuitivo
	vertice_pai->foiVencedor = true;
	vert_novo->foiVencedor = true;
}

Vertice* Mesh::criar_novo_vertice_na_rede(Ponto* ponto)
{
	Vertice* retorno = new Vertice(ponto->coord_x, ponto->coord_y,ponto->coord_z);
	retorno->Indice_vertice = MeshNetworking.size();
	MeshNetworking.push_back(retorno);
	return retorno;
}

int Mesh::qnt_vertices()
{
	return this->MeshNetworking.size();
}

bool Mesh::existe_face(int a, int b, int c)
{
	bool retorno = false;
	bool parou = (this->Faces.size()==0);
	int i = 0;

	while(!parou)
	{
		if(this->Faces[i]->equals(a,b,c))
		{
			retorno = true;
			parou = true;
		}
		else
		{
			i++;
		}
		if(i == this->Faces.size())
		{
			parou = true;
		}
	}

	return retorno;
}