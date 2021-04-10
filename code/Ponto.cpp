#include "stdafx.h"
#include <math.h>
#include "Ponto.h"

Ponto::Ponto(double x, double y, double z){
	this->coord_x = x;
	this->coord_y = y;
	this->coord_z = z;
}

Ponto::Ponto(){
	this->coord_x = 0;
	this->coord_y = 0;
	this->coord_z = 0;
}

void Ponto::setCoord_x(double x) {
	this->coord_x = x;
}

void Ponto::setCoord_y(double y) {
	this->coord_y = y;
}

void Ponto::setCoord_z(double z) {
	this->coord_z = z;
}

double Ponto::distancia(Ponto outro){
  double d;
   d = sqrt((coord_x-outro.coord_x)*(coord_x-outro.coord_x) +
            (coord_y-outro.coord_y)*(coord_y-outro.coord_y) +
            (coord_z-outro.coord_z)*(coord_z-outro.coord_z) );
  return d;
}

double Ponto::distancia(Ponto *outro){
  double d;
   d = sqrt((coord_x-outro->coord_x)*(coord_x-outro->coord_x) +
            (coord_y-outro->coord_y)*(coord_y-outro->coord_y) +
            (coord_z-outro->coord_z)*(coord_z-outro->coord_z) );
  return d;
}

double Ponto::produtoEscalar(Ponto* outro){
  double product;
  product = (coord_x * outro->coord_x) + (coord_y * outro->coord_y) + (coord_z * outro->coord_z);
  return product;
}

Ponto* Ponto::produtoComEscalar(double K){

  Ponto* product = new Ponto();
  product->coord_x = (coord_x * K);
  product->coord_y = (coord_y * K);
  product->coord_z = (coord_z * K);
  return product;
}

Ponto* Ponto::subtracao(Ponto* outro){
  Ponto* sub = new Ponto();
  sub->coord_x = coord_x - outro->coord_x;
  sub->coord_y = coord_y - outro->coord_y;
  sub->coord_z = coord_z - outro->coord_z;
  return sub;
}

Ponto* Ponto::soma_ret(Ponto* outro){
  Ponto* sub = new Ponto();
  sub->coord_x = coord_x + outro->coord_x;
  sub->coord_y = coord_y + outro->coord_y;
  sub->coord_z = coord_z + outro->coord_z;
  return sub;
}

void Ponto::soma(Ponto* outro){
  this->coord_x = coord_x + outro->coord_x;
  this->coord_y = coord_y + outro->coord_y;
  this->coord_z = coord_z + outro->coord_z;
}

Ponto* Ponto::produtoVetorial(Ponto* outro){
  Ponto* product = new Ponto();
  product->coord_x = coord_y*outro->coord_z - coord_z*outro->coord_y;
  product->coord_y = coord_z*outro->coord_x - coord_x*outro->coord_z;
  product->coord_z = coord_x*outro->coord_y - coord_y*outro->coord_x;
  return product;
}

double Ponto::norma(){
  double norma;
  norma = sqrt(coord_x*coord_x + coord_y*coord_y + coord_z*coord_z);
  return norma;
}

void Ponto::projecao(Ponto* ponto)
{
	this->coord_x = this->coord_x*ponto->coord_x;
	this->coord_y = this->coord_y*ponto->coord_y;
	this->coord_z = this->coord_z*ponto->coord_z;
}
