#include"nodo.h"

void Nodo::AgregarHoja(int n){
    int * temp = HojasId;
    HojasId = new int [NumeroHojas + 1];
    for(int i = 0; i < NumeroHojas; i++){
        *(HojasId + i) = *(temp + i);
    }//abra una mejor forma de realizar esta copia/
    HojasId[NumeroHojas] = n;
    NumeroHojas++;
    delete [] temp;
}

Nodo::Nodo(){
    //falta agregar el orden de union
    Id = 0;
    Nombre = "";
    Orden = -1;
    Padre = NULL;
    PadreId = -1;
    HijosId = new int [2];
    Hijos = new Nodo * [2];
    DistanciasHijos = new float [2];
    HijosId[0] = -1;
    HijosId[1] = -1;
    Hijos[0] = NULL;
    Hijos[1] = NULL;
    HojasId = NULL;
    NumeroHojas = 0;//en teoria este siempre deberia tener tamaño 1 ya que el nodo real deberia ser ese represante, los viruales represntanc a 2 o mas
}

Nodo::~Nodo(){
    delete [] HojasId;
    delete [] Hijos;
    delete [] HijosId;
    delete [] DistanciasHijos;
}
