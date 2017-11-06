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
        int Orden;
        //matriz de distancia inicial, del tamaño de los nodos reales
        float * MatrizDistancias;
        float * MatrizDistanciasNueva;
        float * MatrizModificada;//esta como vector
        //matriz de distancia para las iteraciones
        int * ArregloId;
        int * ArregloIdNuevo;
        float * Divergencias;//en realidad es solo para evitar alcular algo dos veces
        int DimensionMatrizI;//Ponerle nombre n para que se entienda con el algoritmo esta se va reduciendo en cada itaracion
        int DimensionMatrizIN;//Ponerle nombre n para que se entienda con el algoritmo esta se va reduciendo en cada itaracion

        //matriz que contine la evalaucaion para esocger los outs mas similares
        float ** MatrizDistanciasModificadas;
        int DimensionMatrizSB;//esta se va reduciendo en cada itaracion
        int GenerarArbol(float ** MatrizDistancia, int NumeroElementos, Nodo ** & Arbol);//devolvera un arreglo con todos los nodos, incluyendo el tamaño
        void NuevaMatrizDistancias(int i, int j, float * MatrizDistanciasDevice);
        float Mij(int i, int j);//este es el Sij, solo que con otro nombre
        void CrearNodoVirtual(int i, int j);
        //void MezclarHojas(int k, int i, int j);
        void ImprimirMatriz(float * Matriz, int n);
        void ImprimirArreglo(int * Arreglo, int n);
        void ImprimirDivergencias(float * Divergencia, int n);
        void DatosIniciales(string * Datos, int n);//el verctor de datos// los nombres de los documentos
        NJ();
        ~NJ();
};

#endif // NJ_H
