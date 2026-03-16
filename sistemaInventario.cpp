#include <iostream>
#include <string>
#include <iomanip>
#include <fstream>
#include <limits>
#include <ctime>
#include <sstream>

using namespace std;

struct NodoProductos {
    int codigo;
    string nombre;
    double precio;
    int stock;
    NodoProductos *next;
};


void reescribirArchivo(NodoProductos* cab);
void registrarMovimiento(int codigo, string nombre, string tipo, int cantidad);

string obtenerFechaHora() {
    time_t now = time(0);
    struct tm tstruct;
    char buf[80];
    tstruct = *localtime(&now);
    strftime(buf, sizeof(buf), "%Y-%m-%d %X", &tstruct);
    return string(buf);
}

void registrarMovimiento(int codigo, string nombre, string tipo, int cantidad) {
    ofstream archivo("historial.txt", ios::app);
    if (archivo.is_open()) {
        archivo << obtenerFechaHora() << " | " 
                << left << setw(10) << tipo << " | "
                << setw(8) << codigo << " | "
                << setw(20) << nombre << " | "
                << "Cant: " << cantidad << endl;
        archivo.close();
    }
}


void titulo() {
    setlocale(LC_ALL, "es_ES.UTF-8");
    system("cls");
    cout << "\033[4m" << "Sistema de Inventario Empresarial" << "\033[0m" << endl<<endl;
}

void pausar() {
    cout << "\nPresiona ENTER para continuar...";
    cin.ignore(numeric_limits<streamsize>::max(), '\n');
    cin.get();
}

string aMinusculas(string nombre) {
    for (int i = 0; i < nombre.size(); i++)
        nombre[i] = tolower(nombre[i]);
    return nombre;
}

int convertirListaArreglo(NodoProductos* cab, NodoProductos* arrLista[]) {

    int tam = 0;
    NodoProductos* actual = cab;

    do {
        arrLista[tam] = actual;
        tam++;
        actual = actual->next;
    } while (actual != cab);

    return tam;
}

void insertionSort(NodoProductos* arrLista[], int tam) {

    for (int i = 1; i < tam; i++) {
        NodoProductos* actual = arrLista[i];
        int j = i - 1;

        // desplazar elementos mayores hacia la derecha
        while (j >= 0 && aMinusculas(arrLista[j]->nombre) > aMinusculas(actual->nombre)) {
            arrLista[j + 1] = arrLista[j];
            j--;
        }

        // insertar en su posición correcta
        arrLista[j + 1] = actual;
    }
}

NodoProductos* codigoRepetido(NodoProductos* cab, int vCodigo) {
    if (cab == NULL) return NULL;
    NodoProductos* actual = cab;

    do {
        if (actual -> codigo == vCodigo)
            return actual; // encontrado

        actual = actual->next;

    } while (actual != cab);

    return NULL; // no encontrado
}

void buscarPorStock(NodoProductos* cab) { //Busqueda Lineal

    if (cab == NULL) {
        cout << "Lista vacía." << endl;
        return;
    }
    int buscar;
    bool encontrado = false;
    NodoProductos* actual = cab;

    cout << "\n==== Búsqueda Lineal por Stock ====" << endl;
    cout << "¿Cuál es el stock que desea buscar?: "; cin>>buscar;
    cout << endl << left
         << setw(10) << "Código"
         << setw(20) << "Nombre"
         << setw(10) << "Precio"
         << setw(10) << "Stock" << endl;
    cout << string(50, '-') << endl;

    do {
        if (actual -> stock == buscar) {
            cout << left
                 << setw(10) << actual->codigo
                 << setw(20) << actual->nombre
                 << setw(10) << actual->precio
                 << setw(10) << actual->stock << endl;
            encontrado = true;
        }
        actual = actual->next;

    } while (actual != cab);

    if (!encontrado)
        cout << "No se encontró ningún producto con stock = " << buscar << endl;
}

void buscarPorNombre(NodoProductos* cab) { //Busqueda Binaria

    if (cab == NULL) {
        cout << "Lista vacía." << endl;
        return;
    }
    string buscar;
    cout << "Ingrese el nombre a buscar: ";
    cin.ignore(numeric_limits<streamsize>::max(), '\n');
    getline(cin, buscar); 

    buscar = aMinusculas(buscar);
    NodoProductos* arrLista[100];
    int cantidad = convertirListaArreglo(cab, arrLista);

    insertionSort(arrLista, cantidad);

    int inicio = 0, fin = cantidad - 1;
    bool encontrado = false;

    while (inicio <= fin) {
        int medio = inicio + (fin - inicio) / 2;

        // convertir nombre del medio a minúsculas para comparar
        string nombreMedio = arrLista[medio]->nombre;
        nombreMedio = aMinusculas(nombreMedio);

        if (nombreMedio == buscar) {
            cout << "\n** Producto encontrado **" << endl;
            cout << left
                 << setw(10) << "Código"
                 << setw(20) << "Nombre"
                 << setw(10) << "Precio"
                 << setw(10) << "Stock" << endl;
            cout << string(50, '-') << endl;
            cout << left
                 << setw(10) << arrLista[medio]->codigo
                 << setw(20) << arrLista[medio]->nombre
                 << setw(10) << arrLista[medio]->precio
                 << setw(10) << arrLista[medio]->stock << endl;
            encontrado = true;
            break;

        } else if (nombreMedio < buscar) {
            inicio = medio + 1; // buscar en derecha

        } else {
            fin = medio - 1;    // buscar en izquierda
        }
    }

    if (!encontrado)
        cout << "No se encontró el producto: " << buscar << endl;
}


void buscarPorCodigo(NodoProductos* cab) {

    //Verificar si la lista está vacía
    if (cab == NULL) {
        cout << "Lista vacía." << endl;
        return;
    }

    int codigoBuscar;
    cout << "Ingrese el código a buscar: ";
    cin >> codigoBuscar;

    //puntero para recorrer la lista
    NodoProductos* actual = cab;
    bool encontrado = false; 

    do {
        if (actual->codigo == codigoBuscar) {

            cout << "\n** Producto encontrado **" << endl;
            cout << left
                 << setw(10) << "Código"
                 << setw(20) << "Nombre"
                 << setw(10) << "Precio"
                 << setw(10) << "Stock" << endl;

            cout << string(50, '-') << endl;

            cout << left
                 << setw(10) << actual->codigo
                 << setw(20) << actual->nombre
                 << setw(10) << actual->precio
                 << setw(10) << actual->stock << endl;

            encontrado = true;
            break;
        }

        //Avanzar al siguiente nodo
        actual = actual->next;

    } while (actual != cab); 

    //Si no se encontró mostramos..
    if (!encontrado) {
        cout << "No se encontró el producto con código: "
             << codigoBuscar << endl;
    }
}

void eliminarProducto(NodoProductos*& cab, NodoProductos*& cola) {

    if (cab == NULL) {
        cout << "Lista vacía." << endl;
        return;
    }

    int codigoEliminar;
    cout << "Ingrese el código del producto a eliminar: ";
    cin >> codigoEliminar;

    NodoProductos* actual = cab;
    NodoProductos* anterior = cola;

    do {

        if (actual->codigo == codigoEliminar) {

            if (cab == cola) { // solo un nodo
                delete actual;
                cab = NULL;
                cola = NULL;
            }

            else if (actual == cab) { // eliminar primero
                cab = cab->next;
                cola->next = cab;
                delete actual;
            }

            else if (actual == cola) { // eliminar último
                anterior->next = cab;
                cola = anterior;
                delete actual;
            }

            else { // eliminar intermedio
                anterior->next = actual->next;
                delete actual;
            }

            reescribirArchivo(cab);

            cout << "Producto eliminado correctamente." << endl;
            return;
        }

        anterior = actual;
        actual = actual->next;

    } while (actual != cab);

    cout << "Producto no encontrado." << endl;
}

void insertarProducto (NodoProductos *&cab, int vCodigo, string vNombre, double vPrecio, int vStock, NodoProductos *&cola) {
    NodoProductos *nuevo = new NodoProductos();
    nuevo -> codigo = vCodigo;
    nuevo -> nombre = vNombre;
    nuevo -> precio = vPrecio;
    nuevo -> stock = vStock;
    nuevo -> next = NULL;

    if (cab == NULL) {
        nuevo -> next = nuevo;
        cab = nuevo;
        cola = nuevo;
    } else {
        nuevo -> next = cab;
        cola -> next = nuevo;
        cola = nuevo;
    }  
}

void guardarArchivo (int vCodigo, string vNombre, double vPrecio, int vStock) {
    ofstream archivo("inventario.txt", ios::app);
    if (archivo.is_open()) {
        archivo<<vCodigo<<","<<vNombre<<","<<vPrecio<<","<<vStock<<endl;
        archivo.close();
    }
}

void reescribirArchivo(NodoProductos* cab) {
    ofstream archivo("inventario.txt"); // sin ios::app para sobreescribir
    if (!archivo.is_open()) return;

    NodoProductos* actual = cab;

    do {
        archivo << actual -> codigo << "," << actual -> nombre << ","<< actual -> precio << "," << actual -> stock  << endl;
        actual = actual->next;
    } while (actual != cab);
    archivo.close();
}

void cargarInventario(NodoProductos*& cab, NodoProductos*& cola) {

    ifstream archivo("inventario.txt");

    if (!archivo.is_open()) {
        cout << "No se encontró inventario.txt, se iniciará vacío." << endl;
        return;
    }

    string linea;

    while (getline(archivo, linea)) {  // lee línea completa

        stringstream ss(linea);        // envuelve la línea para leerla
        /*
        int    vCodigo, vStock;
        string vNombre;
        double vPrecio;

        ss >> vCodigo >> vNombre >> vPrecio >> vStock;
        */
       string vCodigoStr, vNombre, vPrecioStr, vStockStr;

        getline(ss, vCodigoStr, ',');  // lee hasta la primera coma
        getline(ss, vNombre,    ',');  // lee hasta la segunda coma
        getline(ss, vPrecioStr, ',');  // lee hasta la tercera coma
        getline(ss, vStockStr);        // lee el resto

        // convertir strings a sus tipos
        int    vCodigo = stoi(vCodigoStr);
        double vPrecio = stod(vPrecioStr);
        int    vStock  = stoi(vStockStr);
        insertarProducto(cab, vCodigo, vNombre, vPrecio, vStock, cola);

    }

    archivo.close();
    cout << "Inventario cargado correctamente." << endl;
}

void mostrarInventario(NodoProductos* cab) {

    if (cab == NULL) {
        cout << "La lista está vacía." << endl;
        return;
    }

    NodoProductos* actual = cab;

    //cout << "\n===== INVENTARIO =====" << endl;
    cout << left
         << setw(11) << "Código"
         << setw(20) << "Nombre"
         << setw(12) << "Precio"
         << setw(10) << "Stock" << endl;
    cout << string(50, '-') << endl;

    do {
        cout << left
             << setw(10) << actual->codigo
             << setw(20) << actual->nombre
             <<"S/."<< setw(10) << actual->precio
             << setw(10) << actual->stock << endl;

        actual = actual->next;

    } while (actual != cab); // para cuando dé la vuelta completa
}

void actualizarProducto (NodoProductos *&cab, NodoProductos *&codigoRep, int vCodigo, int vStock) {
    //NodoProductos *codigoRep = buscarPorCodigo(cab, vCodigo);
    char optSiNo;
        cout<<" ** El código "<<codigoRep -> codigo<<" ya existe."<<endl<<endl;
        cout << left << setw(11) << "Código" << setw(20) << "Nombre" << setw(12) << "Precio" << setw(10) << "Stock" << endl;
        cout << left << setw(10) << codigoRep -> codigo << setw(20) << codigoRep -> nombre <<"S/."<< setw(10) << codigoRep -> precio << setw(10) << codigoRep -> stock << endl;
        //cout<<" Nombre: "<<codigoRep -> nombre<<"; Precio: S/."<<codigoRep -> precio<<"; Cantidad: "<<codigoRep -> stock<<endl;
        cout<<endl<<"¿Desea actualizar el stock? (s/n): "; cin>>optSiNo;
        if (optSiNo == 's' || optSiNo == 'S') {
            int cantidad;
            cout<<"¿Cuántas unidades de "<<codigoRep -> nombre<<" desea agregar/quitar? (use negativo para quitar): "; cin>>cantidad;
            codigoRep -> stock += cantidad;
            
            string tipo = (cantidad >= 0) ? "ENTRADA" : "SALIDA";
            registrarMovimiento(codigoRep->codigo, codigoRep->nombre, tipo, (cantidad >= 0 ? cantidad : -cantidad));

            cout<<endl<<"** Producto Actualizado **"<<endl;
            cout << left << setw(11) << "Código" << setw(20) << "Nombre" << setw(12) << "Precio" << setw(10) << "Stock" << endl;
            cout << left << setw(10) << codigoRep -> codigo << setw(20) << codigoRep -> nombre <<"S/."<< setw(10) << codigoRep -> precio << setw(10) << codigoRep -> stock << endl;
            
            reescribirArchivo(cab);
        } else {
            cout<<"No se actualizó producto"<<endl;
        }
}


void quickSort(NodoProductos* arrLista[], int inicio, int fin) {
    if (inicio < fin) {

        double pivote = arrLista[fin]->precio;
        int i = inicio - 1;

        for (int j = inicio; j < fin; j++) {
            if (arrLista[j]->precio <= pivote) {
                i++;
                NodoProductos* temp = arrLista[i];
                arrLista[i] = arrLista[j];
                arrLista[j] = temp;
            }
        }

        NodoProductos* temp = arrLista[i + 1];
        arrLista[i + 1] = arrLista[fin];
        arrLista[fin]   = temp;

        int posPivote = i + 1;

        quickSort(arrLista, inicio, posPivote - 1);
        quickSort(arrLista, posPivote + 1, fin);
    }
}

void quickSortCodigo(NodoProductos* arr[], int inicio, int fin) {
    if (inicio < fin) {

        int pivote = arr[fin]->codigo;  // ← comparar por codigo
        int i = inicio - 1;

        for (int j = inicio; j < fin; j++) {
            if (arr[j]->codigo <= pivote) {
                i++;
                NodoProductos* temp = arr[i];
                arr[i] = arr[j];
                arr[j] = temp;
            }
        }

        NodoProductos* temp = arr[i + 1];
        arr[i + 1] = arr[fin];
        arr[fin]   = temp;

        int posPivote = i + 1;

        quickSortCodigo(arr, inicio, posPivote - 1);
        quickSortCodigo(arr, posPivote + 1, fin);
    }
}

void merge(NodoProductos* arrLista[], int inicio, int medio, int fin) {

    int tamIz = medio - inicio + 1;
    int tamDe = fin - medio;

    NodoProductos* arrIzquierda[tamIz];
    for (int i = 0; i < tamIz; i++)
        arrIzquierda[i] = arrLista[inicio + i];

    NodoProductos* arrDerecha[tamDe];
    for (int j = 0; j < tamDe; j++)
        arrDerecha[j] = arrLista[medio + 1 + j];

    int i = 0, j = 0, k = inicio;
    while (i < tamIz && j < tamDe) {
        if (arrIzquierda[i]->stock <= arrDerecha[j]->stock) {
            arrLista[k] = arrIzquierda[i];
            i++;
        } else {
            arrLista[k] = arrDerecha[j];
            j++;
        }
        k++;
    }

    // Copiar elementos restantes de izquierda si los hay
    while (i < tamIz) { arrLista[k] = arrIzquierda[i]; i++; k++; }

    // Copiar elementos restantes de derecha si los hay
    while (j < tamDe) { arrLista[k] = arrDerecha[j];   j++; k++; }
}

void mergeSort(NodoProductos* arrLista[], int inicio, int fin) {
    if (inicio < fin) {
        int medio = inicio + (fin - inicio) / 2;
        mergeSort(arrLista, inicio, medio);
        mergeSort(arrLista, medio + 1, fin);
        merge(arrLista, inicio, medio, fin);
    }
}

void mostrarArreglo(NodoProductos* arrLista[], int cantidad) {

    //cout << "==== Ordenado por precio (Quick Sort) ====" << endl;
    cout << left
         << setw(11) << "Código"
         << setw(20) << "Nombre"
         << setw(12) << "Precio"
         << setw(10) << "Stock" << endl;
    cout << string(50, '-') << endl;

    for (int i = 0; i < cantidad; i++) {
        cout << left
             << setw(10) << arrLista[i]->codigo
             << setw(20) << arrLista[i]->nombre
             <<"S/." << setw(10) << arrLista[i]->precio
             << setw(10) << arrLista[i]->stock << endl;
    }

}

void ordernarCodigo(NodoProductos* cab) { //->FALTA CREAR FUNCION
    if (cab == nullptr) {
        cout << "Lista vacía." << endl;
        return;
    }

    NodoProductos* arr[100];
    int cantidad = convertirListaArreglo(cab, arr);
    quickSortCodigo(arr, 0, cantidad - 1);
    mostrarArreglo(arr, cantidad);
}

void ordenarPrecio(NodoProductos *&cab) { //Quick Sort
    if (cab == NULL) {
        cout << "Lista vacía." << endl;
        return;
    }
    cout << "==== Ordenado por precio (Quick Sort) ====" << endl;
    NodoProductos* arrLista[100];
    int tam = convertirListaArreglo(cab, arrLista);
    quickSort(arrLista, 0, tam - 1);
    mostrarArreglo(arrLista, tam);
}

void ordenarNombre(NodoProductos *&cab) { //Insertion Sort
    if (cab == NULL) {
        cout << "Lista vacía." << endl;
        return;
    }
    cout << "==== Ordenado por nombre (Insertion Sort) ====" << endl;
    NodoProductos* arrLista[100];
    int tam = convertirListaArreglo(cab, arrLista);
    insertionSort(arrLista, tam);
    mostrarArreglo(arrLista, tam);
}

void ordenarStock(NodoProductos *&cab) { //Merge Sort
    if (cab == NULL) {
        cout << "Lista vacía." << endl;
        return;
    }
    cout << "==== Ordenado por stock (Merge Sort) ====" << endl;
    NodoProductos* arrLista[100];
    int tam = convertirListaArreglo(cab, arrLista);
    mergeSort(arrLista, 0, tam - 1);
    mostrarArreglo(arrLista, tam);
}

void historialMovimientos() { 
    ifstream archivo("historial.txt");
    if (!archivo.is_open()) {
        cout << "No hay historial de movimientos registrado." << endl;
        return;
    }

    cout << "=== HISTORIAL DE MOVIMIENTOS ===" << endl;
    cout << left << setw(20) << "Fecha y Hora" << " | "
         << setw(10) << "Tipo" << " | "
         << setw(8) << "Código" << " | "
         << setw(20) << "Nombre" << " | "
         << "Cantidad" << endl;
    cout << string(80, '-') << endl;

    string linea;
    while (getline(archivo, linea)) {
        cout << linea << endl;
    }
    archivo.close();
}


int main() {
    titulo();
    int optMenu, optInventario;
    char optSiNo;
    int vCodigo, vStock;
    double vPrecio;
    string vNombre;

    NodoProductos *cab = NULL;
    NodoProductos *cola = NULL;

    cargarInventario(cab, cola);

    do {
        titulo();
        cout<<" 1. Ver Inventario"<<endl;
        cout<<" 2. Ordenar Inventario"<<endl;
        cout<<" 3. Ingresar Producto"<<endl;
        cout<<" 4. Buscar Producto"<<endl;
        cout<<" 5. Eliminar Producto"<<endl;
        cout<<" 6. Historial de Movimientos"<<endl;
        cout<<" 7. Salir"<<endl;
        cout<<endl<<"-> Elija una opcion: "; cin>>optMenu;

        switch (optMenu)
        {
        case 1: //Ver Inventario
            titulo();
            cout<<"=== Ver Inventario ==="<<endl;
            ordernarCodigo(cab);
            //mostrarInventario(cab);
            pausar();
            break;
        case 2: //Ordenar Inventario
            do {
                titulo();
                cout<<"=== Ordenamiento ==="<<endl;
                cout<<" 1. Por precios"<<endl;
                cout<<" 2. Por stock"<<endl;
                cout<<" 3. Por nombres"<<endl;
                cout<<" 4. Menu principal"<<endl;
                cout<<endl<<"-> Elija una opción: "; cin>>optInventario;
                switch (optInventario)
                {
                case 1: //Ver por precios
                    titulo();
                    ordenarPrecio(cab);
                    pausar();
                    break;
                case 2: //Ver por stock
                    titulo();
                    ordenarStock(cab);
                    pausar();
                    break;
                case 3: //Ver por stock
                    titulo();
                    ordenarNombre(cab);
                    pausar();
                    break;
                default:
                    break;
                }
                //pausar();
            } while (optInventario != 4);
            
            break;

        case 3: //Insertar producto
            do {
                titulo();
                cout<<"=== Ingresar Producto ==="<<endl;
                cout<<" Código de producto: "; cin>>vCodigo;
                
                NodoProductos *codigoRep = codigoRepetido(cab, vCodigo);
                if (codigoRep != NULL) {
                    actualizarProducto(cab, codigoRep, vCodigo, vStock);
                    //continue;
                } else {
                    cin.ignore(numeric_limits<streamsize>::max(), '\n');
                    cout<<" Nombre de producto: ";
                    getline(cin, vNombre); 
                    cout<<" Precio de producto: "; cin>>vPrecio;
                    cout<<" Cantidad de producto: "; cin>>vStock;
                    insertarProducto(cab, vCodigo, vNombre, vPrecio, vStock, cola);
                    guardarArchivo(vCodigo, vNombre, vPrecio, vStock);
                    registrarMovimiento(vCodigo, vNombre, "ENTRADA", vStock);
                    cout<<"** Producto agregado correctamente **"<<endl;
                   // cout<<"Código: "<<vCodigo<<"; Nombre: "<<vNombre<<"; Precio: S/."<<vPrecio<<"; Cantidad: "<<vStock<<endl;
                    cout << left << setw(11) << "Código" << setw(20) << "Nombre" << setw(12) << "Precio" << setw(10) << "Stock" << endl;
                    cout << left << setw(10) << vCodigo << setw(20) << vNombre <<"S/."<< setw(10) << vPrecio << setw(10) << vStock << endl;
                        
                }              
                cout<<endl<<"¿Desea agregar otro producto? (s/n): "; cin>>optSiNo;            
            } while (optSiNo == 's' || optSiNo == 'S');

            break;

        case 4: //Buscar producto
            do {
                titulo();
                cout<<"=== Busqueda de productos ==="<<endl;
                cout<<" 1. Por código"<<endl;
                cout<<" 2. Por stock"<<endl;
                cout<<" 3. Por nombres"<<endl;
                cout<<" 4. Menu principal"<<endl;
                cout<<endl<<"-> Elija una opción: "; cin>>optInventario;
                switch (optInventario)
                {
                case 1: //Buscar por código
                    titulo();
                    buscarPorCodigo(cab);
                    pausar();
                    break;
                case 2: //Buscar por stock
                    titulo();
                    buscarPorStock(cab);
                    pausar();
                    break;
                case 3: //Buscar por Nombre
                    titulo();
                    buscarPorNombre(cab);
                    pausar();
                    break;
                default:
                    break;
                }
            } while (optInventario != 4);
            
            break;

        case 5: //Eliminar Producto
            titulo();
            eliminarProducto(cab, cola);
            pausar();
            break;

        case 6: //Historial de movimiento
            titulo();
            historialMovimientos();
            pausar();
            break;
        
        case 7: //salir
            cout<<"Saliendo del programa..."<<endl;
            pausar();

            break;
        
        default:
            cout<<"Opción incorrecta..."<<endl;
            pausar();
            break;
        }
    } while (optMenu != 7);
    

    return 0;
}