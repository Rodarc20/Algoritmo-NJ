#include"nj.h"

int NJ::GenerarArbol(float ** MatrizDistancia, int NumeroElementos, Nodo ** & Arbol){
    //copiar la matriz de distancias//quiza no neceiso modificarla
    //esta amtriz recibida peuede ser la matriz inicial, quiza no sea necesario copiar o almacenarla en esta clase
    NumeroNodosReales = NumeroElementos;
    DimensionMatrizI= NumeroElementos;
    ArregloId = new int [NumeroElementos];
    MatrizDistancias = new float * [NumeroElementos];
    MatrizDistanciasI = new float * [NumeroElementos];
    MatrizSumaBranchs = new float * [NumeroElementos];
    //la primera matriz de distanciasI debe ser igual
    for(int i = 0; i < NumeroElementos; i++){//quiza sea util si la matriz no esta de la forma que deseo
        MatrizDistancias[i] = new float [NumeroElementos];
        MatrizDistanciasI[i] = new float [NumeroElementos];
        MatrizSumaBranchs[i] = new float [NumeroElementos];
        ArregloId[i] = i;
        for(int j = 0; j < NumeroElementos; j++){
            MatrizDistancias[i][j] = MatrizDistancia[i][j];
            MatrizDistanciasI[i][j] = MatrizDistancia[i][j];
        }
    }
    //ImprimirMatriz(MatrizDistancias, NumeroNodosReales);
    ImprimirArreglo(ArregloId, NumeroNodosReales);
    ImprimirMatriz(MatrizDistanciasI, NumeroNodosReales);
    ImprimirArreglo(ArregloId, NumeroNodosReales);
    ImprimirMatriz(MatrizSumaBranchs, NumeroNodosReales);
    cout << "copiado de datos correcto" << endl;
    //zona de blce, calcular la matriz de dstancias y luego calcular la matriz de sumas, escoger el menro y calcular la nueva matriz de distancias
    //creacion de matrices
    for(int it = 0; it < NumeroElementos-2; it++){//con -2 llego los hago el algoritmo hasta el final
        //bulces
        cout << "iteracion: " << it << endl;
        int iMin = -1, jMin = -1;
        float SMin = numeric_limits<float>::max();//valor maximo
        //copiada quiza no sea util
        for(int i = 0; i < DimensionMatrizI; i++){
            for(int j = 0; j < DimensionMatrizI; j++){
                //no calcular si i y j son iguales; y no hacer caluclo doble, es decir solo recorrel el triangulo superior o inferior
                if(i != j){
                    MatrizSumaBranchs[i][j] = Sij(i,j);
                    if(MatrizSumaBranchs[i][j] < SMin){//en lugar de matriz deberia se solo una varible temporal
                        iMin = i;
                        jMin = j;
                        SMin = MatrizSumaBranchs[i][j];
                    }
                }
            }
        }
        if(it == NumeroElementos-3){//este caso deberia estar mejo estructurado
            cout << "caso especial: " << it << endl;
            float DMin = numeric_limits<float>::max();//valor maximo
            //copiada quiza no sea util
            for(int i = 0; i < DimensionMatrizI; i++){
                for(int j = 0; j < DimensionMatrizI; j++){
                    //no calcular si i y j son iguales; y no hacer caluclo doble, es decir solo recorrel el triangulo superior o inferior
                    if(i != j){
                        if(MatrizDistanciasI[i][j] < DMin){//en lugar de matriz deberia se solo una varible temporal
                            iMin = i;
                            jMin = j;
                            DMin = MatrizDistanciasI[i][j];
                        }
                    }
                }
            }
        }

        ImprimirArreglo(ArregloId, DimensionMatrizI);
        ImprimirMatriz(MatrizDistanciasI, DimensionMatrizI);
        ImprimirArreglo(ArregloId, DimensionMatrizI);
        ImprimirMatriz(MatrizSumaBranchs, DimensionMatrizI);
        cout << "eleccion del sij " << ArregloId[iMin] << " " << ArregloId[jMin] << endl;//no esta mostrando el nombre, esta mostrando los i j de la matriz
        
        //ya tengo los nodos mas similares
        //crear un nuevo nodo virtual, reemplazar
        CrearNodoVirtual(iMin, jMin);
        //CrearNodoVirtual(ArregloId[iMin], ArregloId[jMin]);
        cout << "creado nodo virtual" << endl;
        //darle los hijos de i y j, Mexlcanod sus arreglos hojas.
        //dar padres he hijos respectivos, quiza la propia funcion crear nodo virtaul sirva,

        //substituimos el ide del nodo virrual en el i
        ArregloId[iMin] = NumeroNodos-1;

        NuevaMatrizDistancias(iMin, jMin);//ya se redujo el tamaño de la matriz
        cout << "creada nueva matriz de distancias" << endl;

        //es necesario almacenarlo? es decir solo basta con calcular y quedarme con el minimo y sus respectivos i y j
        //ni si quiera se reutilizan esos valores
        
        //generar nueva matriz de distancias, crear un nuevo nodo dependiendo del menor
        //usar la misma matriz distancias I, solo reducir el numero de fila y columna en 1 y reemplazar los aropiados
    }
    //se supone que cuando quedan los 3 ultimos otus, elos Sij son iguales en todos
    //para seguir con el momdelo de arbol para el NJTree, unirre los dos otus mas similares en un nuevo nodo virtaul, y luego estos dos ultimos otus, los pondre padres uno del otro;
    //solo falta unir los ultimos nodos, sin emabrgo nada me asugra que sean los ultimos nodos virtuales
    //al final se calculo una nuev matriz de distancias, por lo tanto arreglo ID tiene solo 2 elementos
    //UNION: a ellos los unimos
    Nodos[ArregloId[0]]->Padre = Nodos[ArregloId[1]];
    Nodos[ArregloId[0]]->PadreId = ArregloId[1];//o usando -D sobre la linea anterior
    Nodos[ArregloId[1]]->Padre = Nodos[ArregloId[0]];
    Nodos[ArregloId[1]]->PadreId = ArregloId[0];//o usando -D sobre la linea anterior

    Arbol = Nodos;
    return NumeroNodos;

}

float NJ::So (){
    float acum = 0;
    for(int i = 0; i < DimensionMatrizI-1; i++){//el s0 lo saco de la matriz de sitancias de cada itercaion, por lo tanto el limite deberia ser DimensionMatrizI
        for(int j = i+1; j < DimensionMatrizI; j++){
            acum += MatrizDistanciasI[i][j];//quiza deba ser MatrizDistanciasI
        }
    }
    return acum/(DimensionMatrizI- 1);
}

float NJ::Sij(int i, int j){
    float result = 0;
    for(int k = 0; k < DimensionMatrizI; k++){
        if(k != i && k != j){
            result += MatrizDistanciasI[i][k] + MatrizDistanciasI[j][k];
        }
    }
    result /= (2*(DimensionMatrizI-2));
    result += MatrizDistanciasI[i][j]/2;

    float resultadoParcial = 0;
    for(int a = 0; a < DimensionMatrizI-1; a++){//el s0 lo saco de la matriz de sitancias de cada itercaion, por lo tanto el limite deberia ser DimensionMatrizI
        for(int b = a + 1; b < DimensionMatrizI; b++){
            if(a != i && a != j && b != i && b != j){
                resultadoParcial += MatrizDistanciasI[a][b];//quiza deba ser MatrizDistanciasI
            }
        }
    }
    resultadoParcial /= (DimensionMatrizI - 2);
    return result + resultadoParcial;
}

float NJ::CalcularDistancia(int i, int j){
    if(i == j){
        return 0;
    }
    //por si no trabajo lo del arregloid afuera de esta funcion
    //i = ArregloId[i];
    //j = ArregloId[j];
    //una vez que tengo los i y j reales sobre el vecot r de todos los nodos;
    int * hojasi = Nodos[i]->HojasId;
    int * hojasj = Nodos[j]->HojasId;
    int nhojasi = Nodos[i]->NumeroHojas;
    int nhojasj = Nodos[j]->NumeroHojas;
    float acum = 0;
    for(int k = 0; k < nhojasi; k++){
        for(int h = 0; h < nhojasj; h++){
            acum += MatrizDistancias[hojasi[k]][hojasj[h]];
        }
    }
    return acum / (nhojasi * nhojasj);
}

void NJ::CalcularLongitudRamas(int i, int j, float & li, float & lj){
    //estos i y j, deben ser los del espacio local por que el calculo de distancias ya esta sobre la matriz de las iteraciones
    //por si no trabajo lo del arregloid afuera de esta funcion
    //i = ArregloId[i];
    //j = ArregloId[j];
    //una vez que tengo los i y j reales sobre el vecot r de todos los nodos;
    float result = 0;
    for(int k = 0; k < DimensionMatrizI; k++){
        if(k != i && k != j){
            result += MatrizDistanciasI[i][k] - MatrizDistanciasI[j][k];
        }
    }
    result /= (2*(DimensionMatrizI-2));
    result += MatrizDistanciasI[i][j]/2;
    li = result;
    lj = MatrizDistanciasI[i][j] - li;
}

void NJ::NuevaMatrizDistancias(int i, int j){//i y j son los nodos que fueron desginados como similares en esa iteracion
    //estos i y j, estan sobre el ArrgloId
    for(int k = j+1; k < DimensionMatrizI; k++){
        float * temp = MatrizDistanciasI[k-1];
        MatrizDistanciasI[k-1] = MatrizDistanciasI[k];//deberia borrar la deschada? creo que no
        MatrizDistanciasI[k] = temp;
        int tempId = ArregloId[k-1];//solo debe modificar lo concerniete al arreglo j
        ArregloId[k-1] = ArregloId[k];//a la vez que cambio debo actulizar sus poseisiones en el arregloID
        ArregloId[k] = tempId;
        
    }
    for(int k = 0; k < DimensionMatrizI; k++){
        for(int h = j+1; h < DimensionMatrizI; h++){
            float tempD = MatrizDistanciasI[k][h-1];
            MatrizDistanciasI[k][h-1] = MatrizDistanciasI[k][h];
            MatrizDistanciasI[k][h] = tempD;
        }
    }
    //falta el cambio tambien en ArregloID
    DimensionMatrizI--;
    for(int k = 0; k < DimensionMatrizI; k++){
        //en este punto ya debe estar el nodo virtual nuevo en la posicion i, ya que no se movera alli, quiza yua este antes de entrar a esta funcion
        MatrizDistanciasI[i][k] = CalcularDistancia(ArregloId[i], ArregloId[k]);
        MatrizDistanciasI[k][i] = MatrizDistanciasI[i][k];//quiza no sea necesario
    }
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

void NJ::CrearNodoVirtual(int iA, int jA){//es que deberia recibir algun dato, nombre del documento quiza, para los reales
    //que pasa si quiero crear mas nodos reales? mm en teoria no puedo, ya que habira que recalcular todo
    //creo que esto sera para crear nodos virtuales
    ////recibe los i y j del arreglo de nodos
    int i = ArregloId[iA];
    int j = ArregloId[jA];
    Nodo ** temp = Nodos;
    Nodos = new Nodo * [NumeroNodos + 1];
    for(int p = 0; p < NumeroNodos; p++){
        Nodos[p] = temp[p];
    }
    Nodos[NumeroNodos] = new Nodo;//lo nodos los debo borrar con new de tener que hacerlo?
    Nodos[NumeroNodos]->Id = NumeroNodos;
    Nodos[NumeroNodos]->Valido = 0;//si sera virtual o real
    delete [] temp;
    cout << "copiado nodos" << endl;
    //crear las asociaciones// aqui se supone que se crea lo de las banch lenght
    //las distancias al padre
    float Li, Lj;
    CalcularLongitudRamas(iA, jA, Li, Lj);
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
    cout << "realcionando" << endl;

    int e = 0;
    Nodos[NumeroNodos]->NumeroHojas = Nodos[i]->NumeroHojas + Nodos[j]->NumeroHojas;
    Nodos[NumeroNodos]->HojasId = new int [Nodos[NumeroNodos]->NumeroHojas];
    int k = 0, h = 0;
    while((k < Nodos[i]->NumeroHojas) && (h < Nodos[j]->NumeroHojas)/*k<(fin-ini+1)*/){//corregir el algoritmo de mezcla, quiza el tercer if, puede ir solo en otro while, o separar ese if en dos while, pero afuera de este
        if (Nodos[i]->HojasId[k] < Nodos[j]->HojasId[h]){
            Nodos[NumeroNodos]->HojasId[e] = Nodos[i]->HojasId[k];
            k++;
        }
        else{
            Nodos[NumeroNodos]->HojasId[e] = Nodos[j]->HojasId[h];
            h++;
        }
        e++;
    }
    // hasta este punto unoa de las dos mitades termino de llenarse
    while(h < Nodos[j]->NumeroHojas){
        Nodos[NumeroNodos]->HojasId[e] = Nodos[j]->HojasId[h];
        h++;
        e++;
    }
    while(k < Nodos[i]->NumeroHojas){
        Nodos[NumeroNodos]->HojasId[e] = Nodos[i]->HojasId[k];
        k++;
        e++;
    }

    cout << "mezlcado de hojas" << endl;
    //mezlado de hojas
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
        delete [] MatrizDistanciasI[i];
    }
    delete [] MatrizDistancias;
    delete [] MatrizDistanciasI;
    delete [] ArregloId;
    //el arreglo nodos debo eliminarlo??
}

//buscar donde hay redundancia en el acceso a datos, quiza todos los sij, se puedan calcular a la vez, o cosas asi
//busar como manejar en la misma matriz los nuevos nodos virtuales, los reemplazos etc, para no crear matrices mas grandes, y seguir utilizando la que esta, quiza ids temporanoles, o que se yo
