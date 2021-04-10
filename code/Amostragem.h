#ifndef _AMOSTRAGEM_H_
#define _AMOSTRAGEM_H_

#include "stdafx.h"
#include "Ponto.h"
#include <iostream>
#include <fstream>
#include <string>
using namespace std;

class Amostragem
{
	
public:
	int Numero_pontos;
	fstream Arquivo;
	Ponto** NuvemPontos;
	Amostragem(string arq);
	void LerAmostragem();
	Ponto* SortearAleaoriamente();
	

};

#endif // #ifndef _AMOSTRAGEM_H_