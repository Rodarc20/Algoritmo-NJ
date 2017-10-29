#include<iostream>
#include<stdio.h>
#include<stdlib.h>
#include<string>
#include"nj.h"
#include"nodo.h"

using namespace std;

int main(){
    int n;
    cin >> n;
    float ** m = new float * [n];
    string * d = new string [n];
    for(int i = 0; i < n; i++){
        cin >> d[i];
        m[i] = new float [n];
    }
    for(int i = 0; i < n; i++){
        for(int j = 0; j < n; j++){
            cin >> m[i][j];
        }
    }
    NJ nj;
    Nodo ** result;
    nj.DatosIniciales(d, n);
    int tam = nj.GenerarArbol(m, n, result);
    cout << tam << endl;
    for(int i = 0; i < tam; i++){
        cout << "Id: " << result[i]->Id << endl;
        cout << "Nombre: " << result[i]->Nombre << endl;
        cout << "Padre: " << result[i]->PadreId << " -- " << result[i]->DistanciaPadre << endl;
        cout << "Hijo0: " << result[i]->HijosId[0] << " -- " << result[i]->DistanciasHijos[0] << endl;
        cout << "Hijo1: " << result[i]->HijosId[1] << " -- " << result[i]->DistanciasHijos[1] << endl;
    }
    for(int i = 0; i < n; i++){
        delete [] m[i];
    }
    delete [] m;
    delete [] d;
    for(int i = 0; i < tam; i++){
        delete result[i];
    }
    delete [] result;
    return 0;
}
