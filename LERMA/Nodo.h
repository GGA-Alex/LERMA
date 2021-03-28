#ifndef NODO_H
#define NODO_H
#include <iostream>

using namespace std;

class Nodo
{
private:
    string id;
    int peso;
public:
    Nodo();
    string getId() const;
    void setId(const string &value);
    int getPeso() const;
    void setPeso(int value);
    struct comparador{
        bool operator() (Nodo* n1, Nodo* n2){
            return n1->getPeso() < n2->getPeso();
        }
    };
};

#endif // NODO_H
