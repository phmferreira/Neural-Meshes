#include "stdafx.h"
#include "Aresta.h"

using namespace std;

Aresta::Aresta(Vertice* vertice1,Vertice* vertice2)
{
	this->vertice_1 = vertice1;
	this->vertice_2 = vertice2;
	this->vertice_1->Inserir_Vizinhos(vertice2);
	this->vertice_2->Inserir_Vizinhos(vertice1);
}

bool Aresta::equals(int indice1, int indice2)
{
	return ((this->vertice_1->equals(indice1) && this->vertice_2->equals(indice2)) 
		|| (this->vertice_1->equals(indice2) && this->vertice_2->equals(indice1)));
}

bool Aresta::equals(Aresta* aresta)
{
	return this->equals(aresta->vertice_1->Indice_vertice, aresta->vertice_2->Indice_vertice);
}

bool Aresta::ehConectadas(int indice1, int indice2)
{
	return ((this->vertice_1->equals(indice1) || this->vertice_2->equals(indice2)) 
		|| (this->vertice_1->equals(indice2) || this->vertice_2->equals(indice1)));
}

bool Aresta::ehConectadas(Aresta* aresta)
{
	return this->ehConectadas(aresta->vertice_1->Indice_vertice, aresta->vertice_2->Indice_vertice);
}

int Aresta::intersecao(Aresta* aresta)
{
	if (this->vertice_1->equals(aresta->vertice_1))
	{
		return this->vertice_1->Indice_vertice;
	}
	else if (this->vertice_2->equals(aresta->vertice_2))
	{
		return this->vertice_2->Indice_vertice;
	}
	else if(this->vertice_1->equals(aresta->vertice_2))
	{
		return this->vertice_1->Indice_vertice;
	}
	else if (this->vertice_2->equals(aresta->vertice_1))
	{
		return this->vertice_2->Indice_vertice;
	}
	else
	{
		return -1;
	}

}

double Aresta::tamanho()
{
	return this->vertice_1->distancia(this->vertice_2);
}

bool Aresta::temVertice(int indice)
{
	return (this->vertice_1->equals(indice) || this->vertice_2->equals(indice));
}

bool Aresta::temVertice(Vertice* vertice)
{
	return (this->temVertice(vertice->Indice_vertice));
}

Ponto* Aresta::ponto_central()
{
	// BA = B-A
	Ponto* aresta = this->vertice_2->Subtracao(this->vertice_1);
	
	Ponto* meta_aresta = aresta->produtoComEscalar(0.5);
	//A + meia_aresta
	Ponto* retorno = this->vertice_1->soma_ret(meta_aresta);

	return retorno;
}

Vertice* Aresta::diferenca(Aresta* aresta)
{
	int indice1 = aresta->vertice_1->Indice_vertice;
	int indice2 = aresta->vertice_2->Indice_vertice;
	if(this->vertice_1->equals(indice1))
	{
		return this->vertice_2;
	}
	else if(this->vertice_2->equals(indice2)) 
	{
		return this->vertice_1;
	}
	else if(this->vertice_1->equals(indice2))
	{
		return this->vertice_2;
	}
	else if(this->vertice_2->equals(indice1))
	{
		return this->vertice_1;
	}
	else
	{
		return NULL;
	}
}

void Aresta::remover_vizinhos()
{
	this->vertice_1->Remover_vizinho(this->vertice_2->Indice_vertice);
	this->vertice_2->Remover_vizinho(this->vertice_1->Indice_vertice);
}

void Aresta::atualizar(int a, Vertice* novo_vertice)
{
	this->remover_vizinhos();
	if(this->vertice_1->equals(a))
	{
		this->vertice_1 = novo_vertice;
	}
	else
	{
		this->vertice_2 = novo_vertice;
	}
	this->vertice_1->Inserir_Vizinhos(vertice_2);
	this->vertice_2->Inserir_Vizinhos(vertice_1);
}


void Aresta::atualizar(Vertice* a, Vertice* novo_vertice)
{
	this->remover_vizinhos();
	if(this->vertice_1->equals(a))
	{
		this->vertice_1 = novo_vertice;
	}
	else
	{
		this->vertice_2 = novo_vertice;
	}
	this->vertice_1->Inserir_Vizinhos(vertice_2);
	this->vertice_2->Inserir_Vizinhos(vertice_1);
}

bool Aresta::eh_conectada()
{
	return (this->vertice_1->eh_vizinho(this->vertice_2->Indice_vertice) &&
		this->vertice_2->eh_vizinho(this->vertice_1->Indice_vertice));
}