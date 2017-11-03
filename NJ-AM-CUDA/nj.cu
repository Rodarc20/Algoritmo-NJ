#include"nj.h"

__global__
void CalcularDivergenciaDevice(float ** MatrizDistancia, float * Divergencias, int n){//funcion ejecutada en la gpu, osea la funcion device
    //n es la Dimension de la matriz y naturalmente tambien de las divergencias
    int i = blockIdx.x * blockDim.x + threadIdx.x;
    if(i < n){
        //printf("%f\n", Divergencias[i]);
        Divergencias[i] = 0;
        for(int j = 0; j < n; j++){
            if(i != j){
                Divergencias[i] += MatrizDistancia[i][j];
            }
        }
    }
}
__global__
void CalcularMijDevice(float ** MatrizDistancias, float * Divergencias, float * MatrizModificada, int DimensionMatrizI, int inicialMD, int MC, int TamValores){//funcion ejecutada en la gpu, osea la funcion device
    //n es la Dimension de la matriz y naturalmente tambien de las divergencias
    int i = blockIdx.x * blockDim.x + threadIdx.x;
    if(i < TamValores){
        int iM = i / MC;//revisar que estossean los correctos
        int jM = i % MC;
        int imd;
        if(jM/(inicialMD + iM)){
            imd = MC - (inicialMD + iM);
        }
        else{
            imd = inicialMD + iM;
        }
        int jmd = jM % (inicialMD + iM);
        //int prioridad = (int) Nodos[ArregloId[imd]]->Valido + (int) Nodos[ArregloId[jmd]]->Valido;
        //MatrizDistanciasModificadas[i][j] = Mij(i,j);//no es necasrio almacenar
        //if(MatrizDistanciasModificadas[i][j] <= MMin && prioridadMin < prioridad){//en lugar de matriz deberia se solo una varible temporal
        MatrizModificada[i] = MatrizDistancias[imd][jmd] - (Divergencias[imd] + Divergencias[jmd])/(DimensionMatrizI-2);
    }
}
__global__
void MoviendoIDevice(float ** MatrizDistancias, int DimensionMatrizI, int i){
    int k = blockIdx.x * blockDim.x + threadIdx.x;
    if(k < DimensionMatrizI){
        for(int h = i+1; h < DimensionMatrizI; h++){
            float tempD = MatrizDistancias[h-1][k];
            MatrizDistancias[h-1][k] = MatrizDistancias[h][k];
            MatrizDistancias[h][k] = tempD;
        }
    }
}
__global__
void MoviendoJDevice(float ** MatrizDistancias, int DimensionMatrizI, int i){
    int k = blockIdx.x * blockDim.x + threadIdx.x;
    if(k < DimensionMatrizI){
        for(int h = i+1; h < DimensionMatrizI; h++){
            float tempD = MatrizDistancias[k][h-1];
            MatrizDistancias[k][h-1] = MatrizDistancias[k][h];
            MatrizDistancias[k][h] = tempD;
        }
    }
}
__global__
void ActualizarDistanciasDevice(float ** MatrizDistancias, int DimensionMatrizI, int j, float Distanciaij){//funcion ejecutada en la gpu, osea la funcion device
    //cout << ini << " - " << fin << endl;
    int k = blockIdx.x * blockDim.x + threadIdx.x;
    if(k < DimensionMatrizI){
        //en este punto ya debe estar el nodo virtual nuevo en la posicion i, ya que no se movera alli, quiza yua este antes de entrar a esta funcion
        if(k != j){
            MatrizDistancias[j][k] = (MatrizDistancias[j][k] + MatrizDistancias[DimensionMatrizI][k] - Distanciaij)/2;
            MatrizDistancias[k][j] = MatrizDistancias[j][k];//quiza no sea necesario
        }
        else{
            MatrizDistancias[j][k] = 0;
            MatrizDistancias[k][j] = 0;
        }
    }
}
    //n es la Dimension de la matriz y naturalmente tambien de las divergencias
__global__
void ImprimirFilaDevice(float * FilaDistancia, int n){//funcion ejecutada en la gpu, osea la funcion device
    //n es la Dimension de la matriz y naturalmente tambien de las divergencias
    int i = blockIdx.x * blockDim.x + threadIdx.x;
    if(i < n){
        printf("%f\n", FilaDistancia[i]);
    }
}
__global__
void ImprimirDevice(float ** MatrizDistancia, int n){//funcion ejecutada en la gpu, osea la funcion device
    //n es la Dimension de la matriz y naturalmente tambien de las divergencias
    int k = blockIdx.x * blockDim.x + threadIdx.x;
    int i = k / n;
    int j = k % n;
    if(k < n*n){
        printf("(%d, %d): %f\n", i, j, MatrizDistancia[i][j]);
    }
}

int NJ::GenerarArbol(float ** MatrizDistancia, int NumeroElementos, Nodo ** & Arbol){
    //copiar la matriz de distancias//quiza no neceiso modificarla
    //esta amtriz recibida peuede ser la matriz inicial, quiza no sea necesario copiar o almacenarla en esta clase
    DimensionMatrizI= NumeroNodosReales;
    ArregloId = new int [NumeroNodosReales];
    Divergencias = new float [NumeroNodosReales];
    MatrizDistancias = new float * [NumeroNodosReales];
    Orden = 0;

    //la primera matriz de distanciasI debe ser igual
    for(int i = 0; i < NumeroNodosReales; i++){//quiza sea util si la matriz no esta de la forma que deseo
        MatrizDistancias[i] = new float [NumeroNodosReales];
        //MatrizDistanciasModificadas[i] = new float [NumeroElementos];
        ArregloId[i] = i;
        //Divergencias[i] = i;
        for(int j = 0; j < NumeroNodosReales; j++){
            MatrizDistancias[i][j] = MatrizDistancia[i][j];
        }
    }
    //Generando datos en Device
    int size = NumeroNodosReales * sizeof(float);
    int sizem = NumeroNodosReales * sizeof(float*);
    float **  MatrizDistanciasHost = new float * [NumeroNodosReales];//para controlar la creacion de memoria en device
    float **  MatrizDistanciasDevice;
    cudaMalloc((void***) & MatrizDistanciasDevice, sizem);
    for(int i = 0; i < NumeroNodosReales; i++){
        cudaMalloc((void **) &(MatrizDistanciasHost[i]), size);
        cudaMemcpy(MatrizDistanciasHost[i], MatrizDistancias[i], size, cudaMemcpyHostToDevice);
        //ImprimirFilaDevice<<<ceil(DimensionMatrizI/256.0), 256>>> (MatrizDistanciasHost[i], DimensionMatrizI);
    }
    cudaMemcpy(MatrizDistanciasDevice, MatrizDistanciasHost, sizem, cudaMemcpyHostToDevice);
    //ImprimirDevice<<<ceil(DimensionMatrizI/256.0), 256>>> (MatrizDistanciasDevice, DimensionMatrizI);

    float * DivergenciasDevice;
    cudaMalloc((void **) & DivergenciasDevice , size);
    //cudaMemcpy(DivergenciasDevice, Divergencias, size, cudaMemcpyHostToDevice);

    int inicialMD = (DimensionMatrizI-1)/2 + 1;
    int MC = DimensionMatrizI-1 + !((DimensionMatrizI-1) & 1);
    int Mf = DimensionMatrizI - inicialMD;
    int TamValores = (DimensionMatrizI*DimensionMatrizI - DimensionMatrizI)/2;//(n^2 - n)/2 = 15 ejemplo
    
    size = TamValores * sizeof(float);
    MatrizModificada = new float [TamValores];
    float * MatrizModificadaDevice;
    cudaMalloc((void **) & MatrizModificadaDevice , size);

    //ImprimirArreglo(ArregloId, NumeroNodosReales);
    //ImprimirMatriz(MatrizDistancias, NumeroNodosReales);
    //ImprimirArreglo(ArregloId, NumeroNodosReales);
    //ImprimirMatriz(MatrizDistanciasModificadas, NumeroNodosReales);
    //cout << "copiado de datos correcto" << endl;
    //zona de blce, calcular la matriz de dstancias y luego calcular la matriz de sumas, escoger el menro y calcular la nueva matriz de distancias
    //creacion de matrices
    for(int it = 0; it < NumeroNodosReales-2; it++){//con -2 llego los hago el algoritmo hasta el final
        //bulces
        //cout << "iteracion: " << it << endl;
        inicialMD = (DimensionMatrizI-1)/2 + 1;
        MC = DimensionMatrizI-1 + !((DimensionMatrizI-1) & 1);
        Mf = DimensionMatrizI - inicialMD;
        TamValores = (DimensionMatrizI*DimensionMatrizI - DimensionMatrizI)/2;//(n^2 - n)/2 = 15 ejemplo
        //cout << "inicialMD: " << inicialMD << endl;
        //cout << "MC: " << MC << endl;
        //cout << "Mf: " << Mf << endl;
        //cout << "TamValores: " << TamValores << endl;

        int sizeDivergencias = DimensionMatrizI * sizeof(float);
        CalcularDivergenciaDevice<<<ceil(DimensionMatrizI/256.0), 256>>> (MatrizDistanciasDevice, DivergenciasDevice, DimensionMatrizI);
        cudaMemcpy(Divergencias, DivergenciasDevice, sizeDivergencias, cudaMemcpyDeviceToHost);//se necesita para la creacion del nuevo nodo virtual
        CalcularMijDevice<<<ceil(DimensionMatrizI/256.0), 256>>>(MatrizDistanciasDevice, DivergenciasDevice, MatrizModificadaDevice, DimensionMatrizI, inicialMD, MC, TamValores);
        cudaMemcpy(MatrizModificada, MatrizModificadaDevice, TamValores * sizeof(float), cudaMemcpyDeviceToHost);


        //en realidad esta busuqeda del minimo deberia estar en cuda
        int iMin = DimensionMatrizI, jMin = DimensionMatrizI;
        int imdMin = DimensionMatrizI, jmdMin = DimensionMatrizI;
        int prioridadMin = -1;
        float MMin = numeric_limits<float>::max();//valor maximo
        //copiada quiza no sea util
        for(int i = 0; i < Mf; i++){//la forma de recorrer hace que i > j siempre primera posicion (1,0)
            for(int j = 0; j < MC; j++){//para recorrer la matriz tringula inferior
                //if(i != j){//ya no es necesario este if
                //arreglar lo de la prioirdad no son u y j, son ostros valores
                //comparar con los md o con los i j dde la matriz modificada?
                    int imd;
                    if(j/(inicialMD + i)){
                        imd = MC - (inicialMD + i);
                    }
                    else{
                        imd = inicialMD + i;
                    }
                    int jmd = j % (inicialMD + i);
                    int prioridad = (int) Nodos[ArregloId[imd]]->Valido + (int) Nodos[ArregloId[jmd]]->Valido;
                    //MatrizDistanciasModificadas[i][j] = Mij(i,j);//no es necasrio almacenar
                    //if(MatrizDistanciasModificadas[i][j] <= MMin && prioridadMin < prioridad){//en lugar de matriz deberia se solo una varible temporal
                    //if(ActualMij <= MMin && prioridadMin < prioridad){//en lugar de matriz deberia se solo una varible temporal
                    //cout << i << " " << j << ": " << MatrizModificada[i*MC + j] << endl;
                    //if(MatrizModificada[i*MC + j] < MMin || (MatrizModificada[i*MC + j] == MMin && prioridadMin < prioridad) || (MatrizModificada[i*MC + j] == MMin && prioridadMin == prioridad && imd*MC + jmd < iMin*MC + jMin)){
                    //if(MatrizModificada[i*MC + j] < MMin || (MatrizModificada[i*MC + j] == MMin && prioridadMin < prioridad)){
                    if(MatrizModificada[i*MC + j] < MMin || (MatrizModificada[i*MC + j] == MMin && prioridadMin < prioridad) || (MatrizModificada[i*MC + j] == MMin && prioridadMin == prioridad && imd * DimensionMatrizI + jmd < imdMin*DimensionMatrizI + jmdMin)){
                    //if(MatrizDistanciasModificadas[i][j] < MMin){//en lugar de matriz deberia se solo una varible temporal
                        iMin = i;
                        jMin = j;
                        imdMin = imd;
                        jmdMin = jmd;
                        MMin = MatrizModificada[i*MC + j];
                        //MMin = MatrizDistanciasModificadas[i][j];
                        prioridadMin = prioridad;
                    }
                //}
            }
        }
        //cout << "eleccion del modificado sij " << iMin << " " << jMin << endl;//no esta mostrando el nombre, esta mostrando los i j de la matriz
        int imd;
        if(jMin/(inicialMD + iMin)){
            imd = MC - (inicialMD + iMin);
        }
        else{
            imd = inicialMD + iMin;
        }
        int jmd = jMin % (inicialMD + iMin);
        iMin = imd;
        jMin = jmd;
        if(it == NumeroNodosReales-3){//este caso deberia estar mejo estructurado
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
        //ImprimirDevice<<<ceil(DimensionMatrizI/256.0), 256>>> (MatrizDistanciasDevice, DimensionMatrizI);
        //cout << "distancias modificadas" << endl;
        //ImprimirArreglo(ArregloId, DimensionMatrizI);
        //ImprimirMatriz(MatrizDistanciasModificadas, DimensionMatrizI);
        //cout << "eleccion del sij " << ArregloId[iMin] << " " << ArregloId[jMin] << endl;//no esta mostrando el nombre, esta mostrando los i j de la matriz
        //ya tengo los nodos mas similares
        //crear un nuevo nodo virtual, reemplazar
        //cout << "eleccion del sij " << iMin << "(" << ArregloId[iMin] << ") - " << jMin << "(" << ArregloId[jMin] << ")" << endl;//no esta mostrando el nombre, esta mostrando los i j de la matriz
        CrearNodoVirtual(iMin, jMin);
        //CrearNodoVirtual(ArregloId[iMin], ArregloId[jMin]);
        //cout << "creado nodo virtual" << endl;
        //substituimos el ide del nodo virrual en el i
        
        //ArregloId[iMin] = NumeroNodos-1;//como  j tiene que ser mayo que i e cambiado
        //ArregloId[jMin] = NumeroNodos-1;//como  j tiene que ser mayo que i e cambiado
        //esto deberia ser el numero nodo actual
        ArregloId[jMin] = NumeroNodosReales + NumeroNodosVirtuales - 1;
        //ImprimirArreglo(ArregloId, DimensionMatrizI);

        NuevaMatrizDistancias(iMin, jMin, MatrizDistanciasDevice, MatrizDistanciasHost);//ya se redujo el tamaño de la matriz//aqui dentro esta el n--
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
    for(int i = 0; i < NumeroNodosReales; i++){
        cudaFree(MatrizDistanciasHost[i]);
    }
    cudaFree(MatrizDistanciasDevice);
    cudaFree(DivergenciasDevice);
    return NumeroNodos;
}

float NJ::Mij(int i, int j){// en espacio de arreglo id 
    //como trabajo sobre la matriz inferior, esta bien lo i j que recibo, que me marque el (1,0)
    return MatrizDistancias[i][j] - (Divergencias[i] + Divergencias[j])/(DimensionMatrizI-2);
}

void NJ::NuevaMatrizDistancias(int i, int j, float ** MatrizDistanciasDevice, float ** MatrizDistanciasHost){//i y j son los nodos que fueron desginados como similares en esa iteracion
    //antes de ahcer el corrimiento seria buno calcular las deistancias para el nuevo, y solo hacer el cambio con i
    //como el j es el mas a la izquierda, ese lo dejo, y muevo el i hacia el final de la matriz
    //Host solo almacena los punteros de las filas en device
    //cout << "Nueva matriz de distacnias " << i << ", " << j << endl;//se escojo e primero
    float Distanciaij = MatrizDistancias[i][j];//este se puede dejar asi, ya que uso la matriz inferior

    for(int k = i+1; k < DimensionMatrizI; k++){
        int tempId = ArregloId[k-1];//solo debe modificar lo concerniete al arreglo j
        ArregloId[k-1] = ArregloId[k];//a la vez que cambio debo actulizar sus poseisiones en el arregloID
        ArregloId[k] = tempId;
        
    }
    //estos i y j, estan sobre el ArrgloId
    MoviendoIDevice<<<ceil(DimensionMatrizI/256.0), 256>>>(MatrizDistanciasDevice, DimensionMatrizI, i);
    MoviendoJDevice<<<ceil(DimensionMatrizI/256.0), 256>>>(MatrizDistanciasDevice, DimensionMatrizI, i);
    //cout << "i al final" << endl;
    //ImprimirArreglo(ArregloId, DimensionMatrizI);
    //ImprimirMatriz(MatrizDistancias, DimensionMatrizI);
    DimensionMatrizI--;
    //j ahora esta en la posicion DimensionMatriz
    ActualizarDistanciasDevice<<<ceil(DimensionMatrizI/256.0), 256>>>(MatrizDistanciasDevice, DimensionMatrizI, j, Distanciaij);
    int size = DimensionMatrizI * sizeof(float);
    for(int i = 0; i < DimensionMatrizI; i++){
        cudaMemcpy(MatrizDistancias[i], MatrizDistanciasHost[i], size, cudaMemcpyDeviceToHost);
    }
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
    //cout << "entrada: " << i << "-" << j << endl;
    //cout << MatrizDistancias[i][j] << endl;
    //cout << Divergencias[j] << endl;
    //cout << Divergencias[i] << endl;
    float Lj = MatrizDistancias[i][j]/2 + (Divergencias[j] - Divergencias[i])/(2*(DimensionMatrizI-2));
    float Li = MatrizDistancias[i][j] - Lj;
    i = ArregloId[i];
    j = ArregloId[j];

    int nv = NumeroNodosReales + NumeroNodosVirtuales;
    //cout <<"arreglo: " << i << "-" << j << " " << "id nuevo nodo virtual " << nv << endl;
    //cout << "Lj: " << Lj << " Li: " << Li << endl;

    //Nodos[nv]->Id = nv;
    Nodos[nv]->HijosId[0] = j;
    Nodos[nv]->HijosId[1] = i;
    Nodos[nv]->Hijos[0] = Nodos[j];
    Nodos[nv]->Hijos[1] = Nodos[i];
    Nodos[nv]->DistanciasHijos[0] = Lj;
    Nodos[nv]->DistanciasHijos[1] = Li;
    Nodos[nv]->Orden = 0;//iteracion
    Nodos[j]->Padre = Nodos[nv];
    Nodos[j]->PadreId = Nodos[nv]->Id;//o odria ser solo NumeroNodos
    Nodos[j]->DistanciaPadre = Lj;
    Nodos[j]->Orden = Orden;//iteracion
    Orden++;
    Nodos[i]->Padre = Nodos[nv];
    Nodos[i]->PadreId = Nodos[nv]->Id;//o odria ser solo NumeroNodos
    Nodos[i]->DistanciaPadre = Li;
    Nodos[i]->Orden = Orden;//iteracion
    Orden++;
    NumeroNodosVirtuales++;

    //falta agregar lo de las ditancias de los branchs
//afuera de esta funcon se maneja lo de las hojas para los nodos virtaules, 
}

//mejorar las contruscciones de los nodos por defecto y demas, para hacer mas eficiente esta arte
//y si uso la clase vector?? contruir una version que lo use
void NJ::DatosIniciales(string * Datos, int n){
    //en teoria siempre se necesitan la misma cantidad de nodos virtuales, ya que el algoritmos siempre da las iteraciones fijas, solo depende de si n es par o impar;
    //por lo lanto se pueden crear de una vez los nodos virtuales, y solo llenarlos con la impofrmacion pertinenete y determnando a lcula llear, en cada iteracion, en lugar de estar creandolos en cada iteracion
    //crear los nodos reales
    NumeroNodos = n + n - 2;
    NumeroNodosReales = n;
    Nodos = new Nodo * [NumeroNodos];
    for(int i = 0; i < NumeroNodosReales; i++){
        Nodos[i] = new Nodo;
        Nodos[i]->Id = i;
        Nodos[i]->Valido = 1;//si sera virtual o real
        Nodos[i]->Nombre = Datos[i];
    }
    for(int i = NumeroNodosReales; i < NumeroNodos; i++){//para crear los nodos virtuales
        Nodos[i] = new Nodo;
        Nodos[i]->Id = i;
        Nodos[i]->Valido = 0;//si sera virtual o real
    }
    NumeroNodosVirtuales = 0;//aun no los he creado coomo parte del algoritmo,
    //esta variable me ayudara a iterar para trabajr sobre le nodo virtual aporpiado,
    //lo que nates hacian cuando incrementaba la variable NUmeroNodos en cada iteracion
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
////la cantidad de iteraciones es fija, por lo tanto crear los nodos virtuales necesarios desde el inicio, y en la funcion CrearNodoVIrual solamente hacer las uniones y calculos respectivos
