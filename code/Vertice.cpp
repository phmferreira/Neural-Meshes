#include "stdafx.h"
#include "Vertice.h"
#include "Constantes.h"

Vertice::Vertice(double x, double y, double z)
{
	this->coord_x = x;
	this->coord_y = y;
	this->coord_z = z;
	this->Signal_counter = 0;
	this->foiVencedor = false;	
}

Vertice::Vertice()
{
	this->coord_x = 0;
	this->coord_y = 0;
	this->coord_z = 0;
	this->Signal_counter = 0;
	this->foiVencedor = false;
}

void Vertice::Atualizar_Posicao(Ponto amostra)
{

	this->coord_x = (1-alpha_w)*this->coord_x + alpha_w*amostra.coord_x;
	this->coord_y = (1-alpha_w)*this->coord_y + alpha_w*amostra.coord_y;
	this->coord_z = (1-alpha_w)*this->coord_z + alpha_w*amostra.coord_z;

}

void Vertice::Atualizar_Posicao(Ponto* amostra)
{
	this->coord_x = (1-alpha_w)*this->coord_x + alpha_w*amostra->coord_x;
	this->coord_y = (1-alpha_w)*this->coord_y + alpha_w*amostra->coord_y;
	this->coord_z = (1-alpha_w)*this->coord_z + alpha_w*amostra->coord_z;

}

void Vertice::Atualizar_Posicao_suave(Ponto amostra)
{

	this->coord_x = this->coord_x + alpha_n*amostra.coord_x;
	this->coord_y = this->coord_y + alpha_n*amostra.coord_y;
	this->coord_z = this->coord_z + alpha_n*amostra.coord_z;

}

void Vertice::Atualizar_Posicao_suave(Ponto* amostra)
{
	this->coord_x = this->coord_x + alpha_n*amostra->coord_x;
	this->coord_y = this->coord_y + alpha_n*amostra->coord_y;
	this->coord_z = this->coord_z + alpha_n*amostra->coord_z;

}

void Vertice::Atualizar_Posicao_vizinho(Ponto* amostra)
{
	this->coord_x = (1-alpha_n)*this->coord_x + alpha_n*amostra->coord_x;
	this->coord_y = (1-alpha_n)*this->coord_y + alpha_n*amostra->coord_y;
	this->coord_z = (1-alpha_n)*this->coord_z + alpha_n*amostra->coord_z;

}

void Vertice::Inserir_Vizinhos(int indice)
{
	//acho que tem que verificar se ja existe esse vizinho
	if(!(indice == this->Indice_vertice || (this->eh_vizinho(indice))))
	{
		this->Vizinhos.push_back(indice);
	}
}

void Vertice::Inserir_Vizinhos(Vertice* vertice)
{
	this->Inserir_Vizinhos(vertice->Indice_vertice);
}

void Vertice::Set_Indice(int i)
{

	this->Indice_vertice = i;
}

Ponto* Vertice::Normal()
{
	double norma = this->norma();

	return this->produtoComEscalar(1/norma);
}

Ponto* Vertice::Subtracao(Vertice* vertice)
{
	Ponto* ponto = new Ponto(vertice->coord_x, vertice->coord_y, vertice->coord_z);
	return this->subtracao(ponto);
}

void Vertice::Remover_vizinho(int indice)
{
	//verificar se existe o vizinho a ser removido
	for(int i = 0; i < this->Vizinhos.size(); i++)
	{
		if(this->Vizinhos[i] == indice)
		{
			this->Vizinhos.erase(Vizinhos.begin() + i);
			break;
		}
	}
}

void Vertice::Remover_todos_vizinho()
{
	this->Vizinhos.clear();
}

bool Vertice::eh_vizinho(int indice_vert)
{
	bool retorno = false;
	bool parou = (this->Vizinhos.size()==0);
	int i = 0;

	while(!parou)
	{
		if(this->Vizinhos[i] == indice_vert)
		{
			retorno = true;
			parou = true;
		}
		else
		{
			i++;
		}
		if(i == this->Vizinhos.size())
		{
			parou = true;
		}
	}

	return retorno;
}

void Vertice::atualizar_indices_vizinhos(int indice)
{
	for(int i = 0; i < this->Vizinhos.size(); i++)
	{
		if(this->Vizinhos[i] > indice)
		{
			this->Vizinhos[i]--;
		}
	}
}

bool Vertice::equals(Vertice* vertice)
{
	return (this->Indice_vertice == vertice->Indice_vertice);
}

bool Vertice::equals(int indice_vertice)
{
	return (this->Indice_vertice == indice_vertice);
}