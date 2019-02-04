#include<iostream>
#include<stdio.h>
#include<stdlib.h>
#include<string>
#include<ctime>
#include"nj.h"
#include"nodo.h"

using namespace std;

void ImprimirNodos(Nodo ** nodos, int n){
    for(int i = 0; i < n; i++){
        cout << "Id: " << nodos[i]->Id << endl;
        cout << "Nombre: " << nodos[i]->Nombre << endl;
        cout << "Padre: " << nodos[i]->PadreId << " -- " << nodos[i]->DistanciaPadre << endl;
        cout << "Hijo0: " << nodos[i]->HijosId[0] << " -- " << nodos[i]->DistanciasHijos[0] << endl;
        cout << "Hijo1: " << nodos[i]->HijosId[1] << " -- " << nodos[i]->DistanciasHijos[1] << endl;
    }
}

void ImprimirNodosPex(Nodo ** nodos, int n){
    cout << "<?xml version=\"1.0\" encoding=\"ISO-8859-1\" ?>" << endl;
    cout << "<graph description=\"\">" << endl;

    for(int i = 0; i < n; i++){
        cout << "<vertex id=\"" << nodos[i]->Id << "\">" << endl;
        cout << "<valid value=\"" << nodos[i]->Valido << "\"/>" << endl;
        cout << "<x-coordinate value=\"" << i*6 << "\"/>" << endl;
        cout << "<y-coordinate value=\"" << i*6 << "\"/>" << endl;
        cout << "<url value=\"" << "\"/>" << endl;
        cout << "<order value=\"" << nodos[i]->Orden << "\"/>" << endl;//no esta en el pex
        cout << "<scalars>" << endl;
        cout << "<scalar name=\"cdata\" value=\"" << 0.0 << "\"/>" << endl;
        cout << "</scalars>" << endl;
        cout << "<labels>" << endl;
        cout << "<label name=\"title\" value=\"" << nodos[i]->Nombre << "\"/>" << endl;
        cout << "<label name=\"file name\" value=\"" << "\"/>" << endl;
        cout << "</labels>" << endl;
        if(nodos[i]->HijosId[0] != -1){
            cout << "<son value=\"" << nodos[i]->HijosId[0] << "\" distance=\"" << nodos[i]->DistanciasHijos[0] << "\"/>"<< endl;
        }
        if(nodos[i]->HijosId[1] != -1){
            cout << "<son value=\"" << nodos[i]->HijosId[1] << "\" distance=\"" << nodos[i]->DistanciasHijos[1] << "\"/>"<< endl;
        }
        cout << "<parent value=\"" << nodos[i]->PadreId << "\" distance=\"" << nodos[i]->DistanciaPadre << "\"/>"<< endl;
        cout << "</vertex>" << endl;
    }
    cout << "</graph>" << endl;
}

void LeerDatos(float ** & m, string * & d, int & n){//lee matrices cuadrasdas con  todos su valores, primero el numero de datos, luego los nombres de los datos y luego la matriz
    cin >> n;
    m = new float * [n];
    d = new string [n];
    for(int i = 0; i < n; i++){
        cin >> d[i];
        m[i] = new float [n];
    }
    for(int i = 0; i < n; i++){
        for(int j = 0; j < n; j++){
            cin >> m[i][j];
        }
    }
}

void LeerDatosSinNombre(float ** & m, string * & d, int & n){//lee matrices cuadrasdas con  todos su valores, primero el numero de datos, luego los nombres de los datos y luego la matriz
    cin >> n;
    m = new float * [n];
    d = new string [n];
    for(int i = 0; i < n; i++){
        d[i] = i;
        m[i] = new float [n];
    }
    for(int i = 0; i < n; i++){
        for(int j = 0; j < n; j++){
            cin >> m[i][j];
        }
    }
}
void LeerDatosPex(float ** & m, string * & d, int & n){//lee matrices cuadrasdas con  todos su valores, primero el numero de datos, luego los nombres de los datos, luego la clase de los datos para el cdata y luego la matriz
    
    cin >> n;
    m = new float * [n];
    d = new string [n];
    string * clase = new string [n];
    for(int i = 0; i < n; i++){
        cin >> d[i];
        m[i] = new float [n];
    }
    for(int i = 0; i < n; i++){
        cin >> clase[i];
    }
    for(int i = 1; i < n; i++){
        for(int j = 0; j < i; j++){
            cin >> m[i][j];
            m[j][i] = m[i][j];//esto se queita si solo quiero almacenar la matriz inferior
        }
    }
    //devuelvo matrices cuadradas, con los datos reflejados
    delete [] clase;
}

int main(){
    unsigned t0,t1;
    int n;
    float ** m;
    string * d;
    /*cin >> n;
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
    }*/
    //LeerDatosPex(m, d, n);
    LeerDatosSinNombre(m, d, n);
    NJ nj;
    Nodo ** result;
    nj.DatosIniciales(d, n);
    t0 = clock();
    int tam = nj.GenerarArbol(m, n, result);//en toroia no encesitorecibir n ya que ya lo recibo en datos iniciales
    t1 = clock();
    //ImprimirNodos(result, tam);
    ImprimirNodosPex(result, tam);
    double time = (double(t1-t0)/CLOCKS_PER_SEC);
    printf("Finalizado: %f\n", time);
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
