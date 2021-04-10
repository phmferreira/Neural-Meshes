#ifndef _FACE_H_
#define _FACE_H_

#include "stdafx.h"
#include "Aresta.h"
using namespace std;

class Face
{
public:
	Aresta* aresta_1;
	Aresta* aresta_2;
	Aresta* aresta_3;
	int a, b, c;
	bool ehFaceCorreta;
	Face(Aresta* aresta1,Aresta* aresta2,Aresta* aresta3);
	virtual ~Face();
	bool equals(Face* face);
	bool equals(Aresta* aresta1,Aresta* aresta2,Aresta* aresta3);
	bool equals(int a, int b, int c);
	bool verificar_eh_face(Aresta* aresta1,Aresta* aresta2,Aresta* aresta3);
	bool possui_aresta(Aresta* aresta);
	bool possui_vertice(Vertice* vertice);
	Vertice* vertice_que_falta(Aresta* aresta);
	void atualizar(int a, Vertice* novo_vertice);
	void atualizar(Vertice* a, Vertice* novo_vertice);
	void atualizar_indices();
};

#endif // #ifndef _FACE_H_