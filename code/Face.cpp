#include "stdafx.h"
#include "Face.h"

using namespace std;

Face::Face(Aresta* aresta1,Aresta* aresta2,Aresta* aresta3)
{
	this->aresta_1 = aresta1;
	this->aresta_2 = aresta2;
	this->aresta_3 = aresta3;
	this->a = aresta1->intersecao(aresta2);
	this->b = aresta1->intersecao(aresta3);
	this->c = aresta2->intersecao(aresta3);
	this->ehFaceCorreta = this->verificar_eh_face(aresta1,aresta2,aresta3);
}

Face::~Face()
{
}

bool Face::equals(Aresta* aresta1,Aresta* aresta2, Aresta* aresta3)
{
	bool retorno = false;
	
	if(aresta_1->equals(aresta1) || aresta_1->equals(aresta2)  || aresta_1->equals(aresta3))
	{
		if(aresta_2->equals(aresta1) || aresta_2->equals(aresta2)  || aresta_2->equals(aresta3))
		{
			if(aresta_3->equals(aresta1) || aresta_3->equals(aresta2)  || aresta_3->equals(aresta3))
			{
				retorno = true;
			}
		}
	}
	
	return retorno;
}

bool Face::equals(int a, int b, int c)
{
	this->atualizar_indices();
	bool retorno = false;
	
	if(this->a == a|| this->a == b  || this->a == c)
	{
		if(this->b == a|| this->b == b  || this->b == c)
		{
			if(this->c == a|| this->c == b  || this->c == c)
			{
				retorno = true;
			}
		}
	}
	
	return retorno;
}

bool Face::equals(Face* face)
{
	return (this->equals(face->aresta_1, face->aresta_2, face->aresta_3));
}

bool Face::verificar_eh_face(Aresta* aresta1,Aresta* aresta2,Aresta* aresta3)
{
	return (aresta1->ehConectadas(aresta2) && aresta1->ehConectadas(aresta3)
		&& aresta2->ehConectadas(aresta3));
}

bool Face::possui_aresta(Aresta* aresta)
{
	return (aresta_1->equals(aresta) || aresta_2->equals(aresta)  || aresta_3->equals(aresta));
}

Vertice* Face::vertice_que_falta(Aresta* aresta)
{
	if(aresta_1->equals(aresta))
	{
		return this->aresta_2->diferenca(aresta);
	}
	else if(aresta_2->equals(aresta))
	{
		return this->aresta_1->diferenca(aresta);
	}
	else if(aresta_3->equals(aresta))
	{
		return this->aresta_2->diferenca(aresta);
	}
	else
	{
		return NULL;
	}
}

bool Face::possui_vertice(Vertice* vertice)
{
	return (aresta_1->temVertice(vertice) || aresta_2->temVertice(vertice) || aresta_3->temVertice(vertice));
}

void Face::atualizar(int a, Vertice* novo_vertice)
{
	if(this->aresta_1->temVertice(a))
	{
		this->aresta_1->atualizar(a, novo_vertice);
	}
	if(this->aresta_2->temVertice(a))
	{
		this->aresta_2->atualizar(a, novo_vertice);
	}
	if(this->aresta_3->temVertice(a))
	{
		this->aresta_3->atualizar(a, novo_vertice);
	}

	this->a = aresta_1->intersecao(aresta_2);
	this->b = aresta_1->intersecao(aresta_3);
	this->c = aresta_2->intersecao(aresta_3);
	this->ehFaceCorreta = this->verificar_eh_face(aresta_1,aresta_2,aresta_3);

}

void Face::atualizar(Vertice* a, Vertice* novo_vertice)
{
	if(this->aresta_1->temVertice(a))
	{
		this->aresta_1->atualizar(a, novo_vertice);
	}
	if(this->aresta_2->temVertice(a))
	{
		this->aresta_2->atualizar(a, novo_vertice);
	}
	if(this->aresta_3->temVertice(a))
	{
		this->aresta_3->atualizar(a, novo_vertice);
	}

	this->a = aresta_1->intersecao(aresta_2);
	this->b = aresta_1->intersecao(aresta_3);
	this->c = aresta_2->intersecao(aresta_3);
	this->ehFaceCorreta = this->verificar_eh_face(aresta_1,aresta_2,aresta_3);

}

void Face::atualizar_indices()
{
	this->a = aresta_1->intersecao(aresta_2);
	this->b = aresta_1->intersecao(aresta_3);
	this->c = aresta_2->intersecao(aresta_3);
	this->ehFaceCorreta = this->verificar_eh_face(aresta_1,aresta_2,aresta_3);
}