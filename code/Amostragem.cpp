#include "stdafx.h"
#include "Amostragem.h"
#include <ctime>

Amostragem::Amostragem(string arq)
{
	Arquivo.open (arq);
	this->LerAmostragem();
	srand(time(0));
	Arquivo.close();

}

void Amostragem::LerAmostragem()
{
	cout << "Ler Todos os Pontos Amostrais" << endl;
	
	//Codigo para ler uma nuvem qualquer de pontos
	//[Números de Pontos]
	//[Lista de Pontos (coord x, coord y, coord z)]

	//Era bom ter um LerArquivoPLY

	string lixo;
	int n_triangulo;
	int num_pontos;
	double coor_x1;
	double coor_y1;
	double coor_z1;
	Arquivo >> num_pontos;
	this->Numero_pontos = num_pontos;
	this->NuvemPontos = new Ponto*[num_pontos];
	int k = 0;
	for(k = 0; k < num_pontos; k++){
		Arquivo >> coor_x1;
		Arquivo >> coor_y1;
		Arquivo >> coor_z1;
		this->NuvemPontos[k] = new Ponto(coor_x1,coor_y1,coor_z1);  
	}

}

Ponto* Amostragem::SortearAleaoriamente()
{		
	int indice_sorteio = (rand() % this->Numero_pontos);
	return this->NuvemPontos[indice_sorteio];
}