#include "laptop.h"

Laptop::Laptop(){
	Laptop::port = 0;
}

Laptop::Laptop(string n0, int p0){
	Laptop::name = n0;
	Laptop::port = p0;
}

int Laptop::getPort() const{
	return Laptop::port;
}

void Laptop::setPort(int p0){
	Laptop::port = p0;
}
	
string Laptop::getName() const{
	return Laptop::name;
}

void Laptop::setName(string n0){
	 Laptop::name = n0;
}
