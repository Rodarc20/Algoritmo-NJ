#ifndef NODO_H
#define NODO_H

#include<iostream>
#include<stdio.h>
#include<string>

using namespace std;

class Nodo {
    public:
        int Id;
        bool Valido;
        string Nombre;//nombre del elemento o dato
        int Orden;
        int * HijosId;
        Nodo ** Hijos;
        float * DistanciasHijos;//deberianser vectores
        int PadreId;
        Nodo * Padre;
        float DistanciaPadre;
        int * HojasId;
        int NumeroHojas;//solo para saber a quienes representa
        void AgregarHoja(int n);
        Nodo();
        ~Nodo();
};

#endif // NODO_H
