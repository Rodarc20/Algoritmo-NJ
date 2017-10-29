#ifndef NJ_H
#define NJ_H

#include<iostream>
#include<stdio.h>
#include<stdlib.h>
#include<limits>
#include"nodo.h"

using namespace std;

class NJ {
    public:
        Nodo ** Nodos;//doble puntero seria mejor?
        //esos son los nodos del arbol
        //no necesito dos arreglos, todo esta almancenad en Nodos,
        //puedo ier a los nodos virtuales usando los enteros, del numero de nodos reales y virtuales
        int NumeroNodos;
        int NumeroNodosReales;
        int NumeroNodosVirtuales;
        //matriz de distancia inicial, del tamaño de los nodos reales
        float ** MatrizDistancias;
        //matriz de distancia para las iteraciones
        float ** MatrizDistanciasI;//quiza la mtriz deba tener una fila y columna mas para albergar los indices de los nodos
        int * ArregloId;
        int DimensionMatrizI;//esta se va reduciendo en cada itaracion

        //matriz que contine la evalaucaion para esocger los outs mas similares
        float ** MatrizSumaBranchs;
        int DimensionMatrizSB;//esta se va reduciendo en cada itaracion
        int GenerarArbol(float ** MatrizDistancia, int NumeroElementos, Nodo ** & Arbol);//devolvera un arreglo con todos los nodos, incluyendo el tamaño
        void NuevaMatrizDistancias(int i, int j);
        float So();
        float Sij(int i, int j);
        float CalcularDistancia(int i, int j);
        void CalcularLongitudRamas(int i, int j, float & li, float & lj);
        void CrearNodoVirtual(int iA, int jA);
        //void MezclarHojas(int k, int i, int j);
        void ImprimirMatriz(float ** Matriz, int n);
        void ImprimirArreglo(int * Arreglo, int n);
        void DatosIniciales(string * Datos, int n);//el verctor de datos// los nombres de los documentos
        NJ();
        ~NJ();
};

#endif // NJ_H
