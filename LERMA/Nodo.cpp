#include "Nodo.h"

string Nodo::getId() const
{
    return id;
}

void Nodo::setId(const string &value)
{
    id = value;
}

int Nodo::getPeso() const
{
    return peso;
}

void Nodo::setPeso(int value)
{
    peso = value;
}

Nodo::Nodo()
{

}
