#include"nj.h"

int NJ::GenerarArbol(float ** MatrizDistancia, int NumeroElementos, Nodo ** & Arbol){
    //copiar la matriz de distancias//quiza no neceiso modificarla
    //esta amtriz recibida peuede ser la matriz inicial, quiza no sea necesario copiar o almacenarla en esta clase
    NumeroNodosReales = NumeroElementos;
    DimensionMatrizI= NumeroElementos;
    ArregloId = new int [NumeroElementos];
    Divergencias = new float [NumeroElementos];
    MatrizDistancias = new float * [NumeroElementos];
    MatrizDistanciasModificadas = new float * [NumeroElementos];
    //la primera matriz de distanciasI debe ser igual
    for(int i = 0; i < NumeroElementos; i++){//quiza sea util si la matriz no esta de la forma que deseo
        MatrizDistancias[i] = new float [NumeroElementos];
        MatrizDistanciasModificadas[i] = new float [NumeroElementos];
        ArregloId[i] = i;
        for(int j = 0; j < NumeroElementos; j++){
            MatrizDistancias[i][j] = MatrizDistancia[i][j];
        }
    }
    //ImprimirArreglo(ArregloId, NumeroNodosReales);
    //ImprimirMatriz(MatrizDistancias, NumeroNodosReales);
    //ImprimirArreglo(ArregloId, NumeroNodosReales);
    //ImprimirMatriz(MatrizDistanciasModificadas, NumeroNodosReales);
    //cout << "copiado de datos correcto" << endl;
    //zona de blce, calcular la matriz de dstancias y luego calcular la matriz de sumas, escoger el menro y calcular la nueva matriz de distancias
    //creacion de matrices
    for(int it = 0; it < NumeroElementos-2; it++){//con -2 llego los hago el algoritmo hasta el final
        //bulces
        //cout << "iteracion: " << it << endl;
        for(int i = 0; i < DimensionMatrizI; i++){
            Divergencias[i] = 0;
            for(int j = 0; j < DimensionMatrizI; j++){
                if(i != j){
                    Divergencias[i] += MatrizDistancias[i][j];
                }
            }
        }
        //ImprimirDivergencias(Divergencias, DimensionMatrizI);

        int iMin = -1, jMin = -1;
        int prioridadMin = -1;
        float MMin = numeric_limits<float>::max();//valor maximo
        //copiada quiza no sea util
        for(int i = 1; i < DimensionMatrizI; i++){//la forma de recorrer hace que i > j siempre primera posicion (1,0)
            for(int j = 0; j < i; j++){//para recorrer la matriz tringula inferior
                //if(i != j){//ya no es necesario este if
                    int prioridad = (int) Nodos[ArregloId[i]]->Valido + (int) Nodos[ArregloId[j]]->Valido;
                    //MatrizDistanciasModificadas[i][j] = Mij(i,j);//no es necasrio almacenar
                    //if(MatrizDistanciasModificadas[i][j] <= MMin && prioridadMin < prioridad){//en lugar de matriz deberia se solo una varible temporal
                    float ActualMij = Mij(i,j);
                    if(ActualMij <= MMin && prioridadMin < prioridad){//en lugar de matriz deberia se solo una varible temporal
                    //if(MatrizDistanciasModificadas[i][j] < MMin){//en lugar de matriz deberia se solo una varible temporal
                        iMin = i;
                        jMin = j;
                        MMin = ActualMij;
                        //MMin = MatrizDistanciasModificadas[i][j];
                        prioridadMin = prioridad;
                    }
                //}
            }
        }
        if(it == NumeroElementos-3){//este caso deberia estar mejo estructurado
            //cout << "caso especial: " << it << endl;
            float DMin = numeric_limits<float>::max();//valor maximo
            //copiada quiza no sea util
            for(int i = 1; i < DimensionMatrizI; i++){
                for(int j = 0; j < i; j++){
                    //no calcular si i y j son iguales; y no hacer caluclo doble, es decir solo recorrel el triangulo superior o inferior
                    //if(i != j){
                        if(MatrizDistancias[i][j] < DMin){//en lugar de matriz deberia se solo una varible temporal
                            iMin = i;
                            jMin = j;
                            DMin = MatrizDistancias[i][j];
                        }
                    //}
                }
            }
        }
        //una vez seleccionados el i y j , deberia solo ahcer un cambio para que esto funcione sin mover las otras funciones, o hacer el cambio definitivo, ya que esteo siempre se cumplira!!, lo de que j sea mayo que i


        //cout << "distancias" << endl;
        //ImprimirArreglo(ArregloId, DimensionMatrizI);
        //ImprimirMatriz(MatrizDistancias, DimensionMatrizI);
        //cout << "distancias modificadas" << endl;
        //ImprimirArreglo(ArregloId, DimensionMatrizI);
        //ImprimirMatriz(MatrizDistanciasModificadas, DimensionMatrizI);
        //cout << "eleccion del sij " << ArregloId[iMin] << " " << ArregloId[jMin] << endl;//no esta mostrando el nombre, esta mostrando los i j de la matriz
        //ya tengo los nodos mas similares
        //crear un nuevo nodo virtual, reemplazar
        //cout << "eleccion del sij " << iMin << " " << jMin << endl;//no esta mostrando el nombre, esta mostrando los i j de la matriz
        CrearNodoVirtual(iMin, jMin);
        //CrearNodoVirtual(ArregloId[iMin], ArregloId[jMin]);
        //cout << "creado nodo virtual" << endl;
        //substituimos el ide del nodo virrual en el i
        
        //ArregloId[iMin] = NumeroNodos-1;//como  j tiene que ser mayo que i e cambiado
        ArregloId[jMin] = NumeroNodos-1;//como  j tiene que ser mayo que i e cambiado

        NuevaMatrizDistancias(iMin, jMin);//ya se redujo el tamaño de la matriz//aqui dentro esta el n--
        //cout << "creada nueva matriz de distancias" << endl;
    }
    //al final se calculo una nuev matriz de distancias, por lo tanto arreglo ID tiene solo 2 elementos
    //UNION: a ellos los unimos
    //cout << "final de las iteracion hasta n-2" << endl;
    //cout << "distancias" << endl;
    //ImprimirArreglo(ArregloId, DimensionMatrizI);
    //ImprimirMatriz(MatrizDistancias, DimensionMatrizI);
    //cout << "distancias modificadas" << endl;
    //ImprimirArreglo(ArregloId, DimensionMatrizI);
    //ImprimirMatriz(MatrizDistanciasModificadas, DimensionMatrizI);
    Nodos[ArregloId[0]]->Padre = Nodos[ArregloId[1]];
    Nodos[ArregloId[0]]->PadreId = ArregloId[1];//o usando -D sobre la linea anterior
    Nodos[ArregloId[0]]->DistanciaPadre= MatrizDistancias[0][1];//o usando -D sobre la linea anterior
    Nodos[ArregloId[1]]->Padre = Nodos[ArregloId[0]];
    Nodos[ArregloId[1]]->PadreId = ArregloId[0];//o usando -D sobre la linea anterior
    Nodos[ArregloId[1]]->DistanciaPadre= MatrizDistancias[0][1];//o usando -D sobre la linea anterior
    //falta calcular estas distancias
    Arbol = Nodos;
    return NumeroNodos;
}

float NJ::Mij(int i, int j){// en espacio de arreglo id 
    //como trabajo sobre la matriz inferior, esta bien lo i j que recibo, que me marque el (1,0)
    return MatrizDistancias[i][j] - (Divergencias[i] + Divergencias[j])/(DimensionMatrizI-2);
}

void NJ::NuevaMatrizDistancias(int i, int j){//i y j son los nodos que fueron desginados como similares en esa iteracion
    //antes de ahcer el corrimiento seria buno calcular las deistancias para el nuevo, y solo hacer el cambio con i
    //como el j es el mas a la izquierda, ese lo dejo, y muevo el i hacia el final de la matriz
    //cout << "Nueva matriz de distacnias " << i << ", " << j << endl;//se escojo e primero
    float Distanciaij = MatrizDistancias[i][j];//este se puede dejar asi, ya que uso la matriz inferior
    //estos i y j, estan sobre el ArrgloId
    for(int k = i+1; k < DimensionMatrizI; k++){
        float * temp = MatrizDistancias[k-1];
        MatrizDistancias[k-1] = MatrizDistancias[k];//deberia borrar la deschada? creo que no
        MatrizDistancias[k] = temp;
        int tempId = ArregloId[k-1];//solo debe modificar lo concerniete al arreglo j
        ArregloId[k-1] = ArregloId[k];//a la vez que cambio debo actulizar sus poseisiones en el arregloID
        ArregloId[k] = tempId;
        
    }
    for(int k = 0; k < DimensionMatrizI; k++){
        for(int h = i+1; h < DimensionMatrizI; h++){
            float tempD = MatrizDistancias[k][h-1];
            MatrizDistancias[k][h-1] = MatrizDistancias[k][h];
            MatrizDistancias[k][h] = tempD;
        }
    }
    //cout << "i al final" << endl;
    //ImprimirArreglo(ArregloId, DimensionMatrizI);
    //ImprimirMatriz(MatrizDistancias, DimensionMatrizI);
    DimensionMatrizI--;
    //j ahora esta en la posicion DimensionMatriz
    for(int k = 0; k < DimensionMatrizI; k++){
        //en este punto ya debe estar el nodo virtual nuevo en la posicion i, ya que no se movera alli, quiza yua este antes de entrar a esta funcion
        if(k != j){
            //cout << k << ":: " << MatrizDistancias[i][k] << "+" << MatrizDistancias[DimensionMatrizI][k] << "-" <<  Distanciaij << endl;
            MatrizDistancias[j][k] = (MatrizDistancias[j][k] + MatrizDistancias[DimensionMatrizI][k] - Distanciaij)/2;
            MatrizDistancias[k][j] = MatrizDistancias[j][k];//quiza no sea necesario
        }
        else{
            MatrizDistancias[j][k] = 0;
            MatrizDistancias[k][j] = 0;
        }
    }
    /*if(i > j){
        int temp = i;
        i = j;
        j = temp;
        //en lugar de esto, solo cambiar las variables o el uso de las variables, para que trabajn distinto
    }
    float Distanciaij = MatrizDistancias[i][j];
    //estos i y j, estan sobre el ArrgloId
    for(int k = j+1; k < DimensionMatrizI; k++){
        float * temp = MatrizDistancias[k-1];
        MatrizDistancias[k-1] = MatrizDistancias[k];//deberia borrar la deschada? creo que no
        MatrizDistancias[k] = temp;
        int tempId = ArregloId[k-1];//solo debe modificar lo concerniete al arreglo j
        ArregloId[k-1] = ArregloId[k];//a la vez que cambio debo actulizar sus poseisiones en el arregloID
        ArregloId[k] = tempId;
        
    }
    for(int k = 0; k < DimensionMatrizI; k++){
        for(int h = j+1; h < DimensionMatrizI; h++){
            float tempD = MatrizDistancias[k][h-1];
            MatrizDistancias[k][h-1] = MatrizDistancias[k][h];
            MatrizDistancias[k][h] = tempD;
        }
    }
    //cout << "j al final" << endl;
    //ImprimirArreglo(ArregloId, DimensionMatrizI);
    //ImprimirMatriz(MatrizDistancias, DimensionMatrizI);
    DimensionMatrizI--;
    //j ahora esta en la posicion DimensionMatriz
    for(int k = 0; k < DimensionMatrizI; k++){
        //en este punto ya debe estar el nodo virtual nuevo en la posicion i, ya que no se movera alli, quiza yua este antes de entrar a esta funcion
        if(k != i){
            //cout << k << ":: " << MatrizDistancias[i][k] << "+" << MatrizDistancias[DimensionMatrizI][k] << "-" <<  Distanciaij << endl;
            MatrizDistancias[i][k] = (MatrizDistancias[i][k] + MatrizDistancias[DimensionMatrizI][k] - Distanciaij)/2;
            MatrizDistancias[k][i] = MatrizDistancias[i][k];//quiza no sea necesario
        }
        else{
            MatrizDistancias[i][k] = 0;
            MatrizDistancias[k][i] = 0;
        }
    }*/
    //cout << "fin creacio nnueva matriz" << endl;
    //en que momento ubico al nuevo nodo viertual cundo lo creo?
    //
}

void NJ::ImprimirMatriz(float ** Matriz, int n){
    for(int i = 0; i < n; i++){
        for(int j = 0; j < n; j++){
            printf("%.3f\t", Matriz[i][j]);
        }
        printf("\n");
    }
}

void NJ::ImprimirArreglo(int * Arreglo, int n){
    for(int i = 0; i < n; i++){
        printf("%d\t", ArregloId[i]);
    }
    printf("\n");
}

void NJ::ImprimirDivergencias(float * Divergencia, int n){
    printf("Divergencias:\n");
    for(int i = 0; i < n; i++){
        printf("%.3f\t", Divergencia[i]);
    }
    printf("\n");
}

void NJ::CrearNodoVirtual(int i, int j){//es que deberia recibir algun dato, nombre del documento quiza, para los reales
    //que pasa si quiero crear mas nodos reales? mm en teoria no puedo, ya que habira que recalcular todo
    //creo que esto sera para crear nodos virtuales
    ////recibe los i y j del arreglo de nodos
    //aqui recibo los i y j siendo i > j, por lo tanto lo que hacia con j debo hacerlo con y y lo que hacia con i debo hacerlo conj
    //float Lj = MatrizDistancias[j][i]/2 + (Divergencias[j] - Divergencias[i])/(2*(DimensionMatrizI-2));
    //float Li = MatrizDistancias[j][i] - Lj;
    float Lj = MatrizDistancias[i][j]/2 + (Divergencias[j] - Divergencias[i])/(2*(DimensionMatrizI-2));
    float Li = MatrizDistancias[i][j] - Lj;
    i = ArregloId[i];
    j = ArregloId[j];
    Nodo ** temp = Nodos;
    Nodos = new Nodo * [NumeroNodos + 1];
    for(int p = 0; p < NumeroNodos; p++){
        Nodos[p] = temp[p];
    }
    Nodos[NumeroNodos] = new Nodo;//lo nodos los debo borrar con new de tener que hacerlo?
    Nodos[NumeroNodos]->Id = NumeroNodos;
    Nodos[NumeroNodos]->Valido = 0;//si sera virtual o real
    delete [] temp;
    //cout << "copiado nodos" << endl;
    //crear las asociaciones// aqui se supone que se crea lo de las banch lenght
    //las distancias al padre
    Nodos[NumeroNodos]->HijosId[0] = j;
    Nodos[NumeroNodos]->HijosId[1] = i;
    Nodos[NumeroNodos]->Hijos[0] = Nodos[j];
    Nodos[NumeroNodos]->Hijos[1] = Nodos[i];
    Nodos[NumeroNodos]->DistanciasHijos[0] = Lj;
    Nodos[NumeroNodos]->DistanciasHijos[1] = Li;
    Nodos[NumeroNodos]->Orden = 0;//iteracion
    Nodos[j]->Padre = Nodos[NumeroNodos];
    Nodos[j]->PadreId = Nodos[NumeroNodos]->Id;//o odria ser solo NumeroNodos
    Nodos[j]->DistanciaPadre = Lj;
    Nodos[i]->Padre = Nodos[NumeroNodos];
    Nodos[i]->PadreId = Nodos[NumeroNodos]->Id;//o odria ser solo NumeroNodos
    Nodos[i]->DistanciaPadre = Li;
    //falta hacer los calculos de las distancias
    //cout << "realcionando" << endl;
    /*if(i > j){
        int temp = i;
        i = j;
        j = temp;
        //en lugar de esto, solo cambiar las variables o el uso de las variables, para que trabajn distinto
    }
    float Li = MatrizDistancias[i][j]/2 + (Divergencias[i] - Divergencias[j])/(2*(DimensionMatrizI-2));
    float Lj = MatrizDistancias[i][j] - Li;
    i = ArregloId[i];
    j = ArregloId[j];
    Nodo ** temp = Nodos;
    Nodos = new Nodo * [NumeroNodos + 1];
    for(int p = 0; p < NumeroNodos; p++){
        Nodos[p] = temp[p];
    }
    Nodos[NumeroNodos] = new Nodo;//lo nodos los debo borrar con new de tener que hacerlo?
    Nodos[NumeroNodos]->Id = NumeroNodos;
    Nodos[NumeroNodos]->Valido = 0;//si sera virtual o real
    delete [] temp;
    //cout << "copiado nodos" << endl;
    //crear las asociaciones// aqui se supone que se crea lo de las banch lenght
    //las distancias al padre
    Nodos[NumeroNodos]->HijosId[0] = i;
    Nodos[NumeroNodos]->HijosId[1] = j;
    Nodos[NumeroNodos]->Hijos[0] = Nodos[i];
    Nodos[NumeroNodos]->Hijos[1] = Nodos[j];
    Nodos[NumeroNodos]->DistanciasHijos[0] = Li;
    Nodos[NumeroNodos]->DistanciasHijos[1] = Lj;
    Nodos[NumeroNodos]->Orden = 0;//iteracion
    Nodos[i]->Padre = Nodos[NumeroNodos];
    Nodos[i]->PadreId = Nodos[NumeroNodos]->Id;//o odria ser solo NumeroNodos
    Nodos[i]->DistanciaPadre = Li;
    Nodos[j]->Padre = Nodos[NumeroNodos];
    Nodos[j]->PadreId = Nodos[NumeroNodos]->Id;//o odria ser solo NumeroNodos
    Nodos[j]->DistanciaPadre = Lj;
    //falta hacer los calculos de las distancias
    //cout << "realcionando" << endl;*/
    NumeroNodos++;

    //falta agregar lo de las ditancias de los branchs
//afuera de esta funcon se maneja lo de las hojas para los nodos virtaules, 
}

//mejorar las contruscciones de los nodos por defecto y demas, para hacer mas eficiente esta arte
//y si uso la clase vector?? contruir una version que lo use
void NJ::DatosIniciales(string * Datos, int n){
    //en teoria siempre se necesitan la misma cantidad de nodos virtuales, ya que el algoritmos siempre da las iteraciones fijas, solo depende de si n es par o impar;
    //por lo lanto se pueden crear de una vez los nodos virtuales, y solo llenarlos con la impofrmacion pertinenete y determnando a lcula llear, en cada iteracion, en lugar de estar creandolos en cada iteracion
    //crear los nodos reales
    NumeroNodos = n;
    NumeroNodosReales = n;
    Nodos = new Nodo * [n];
    for(int i = 0; i < n; i++){
        Nodos[i] = new Nodo;
        Nodos[i]->Id = i;
        Nodos[i]->Valido = 1;//si sera virtual o real
        Nodos[i]->Nombre = Datos[i];
        Nodos[i]->AgregarHoja(i);//esta linea podria ser lenta
    }
}

NJ::NJ(){

}

NJ::~NJ(){
    for(int i = 0; i < NumeroNodosReales; i++){//quiza sea util si la matriz no esta de la forma que deseo
        delete [] MatrizDistancias[i];
    }
    delete [] MatrizDistancias;
    delete [] ArregloId;
    delete [] Divergencias;
    //el arreglo nodos debo eliminarlo??
}

//buscar donde hay redundancia en el acceso a datos, quiza todos los sij, se puedan calcular a la vez, o cosas asi
//busar como manejar en la misma matriz los nuevos nodos virtuales, los reemplazos etc, para no crear matrices mas grandes, y seguir utilizando la que esta, quiza ids temporanoles, o que se yo
