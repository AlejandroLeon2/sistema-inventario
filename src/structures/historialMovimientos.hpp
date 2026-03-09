#pragma once
#include <iostream>    
#include "../models/Product.hpp"
using namespace std;

// Nodo del historial
struct Movimiento {
    int codigoProducto;
    string tipoMovimiento;
    int cantidad;

    Movimiento* next;
    Movimiento* prev;
};

void mostrarHistorial(Movimiento*& cabeza) {
    if (cabeza == nullptr) {
        cout << "Historial vacío" << endl;
        return;
    }

    Movimiento* actual = cabeza;
    do {
        cout << "Producto: " << actual->codigoProducto
             << " | Tipo: "  << actual->tipoMovimiento
             << " | Cant: "  << actual->cantidad << endl;

        actual = actual->next;

    } while (actual != cabeza); // parar cuando demos la vuelta completa
}

void agregarMovimiento(Movimiento*& cabeza, int codigo,
                       string tipo, int cantidad) {
    Movimiento* nuevo = new Movimiento();
    nuevo->codigoProducto = codigo;
    nuevo->tipoMovimiento = tipo;
    nuevo->cantidad       = cantidad;

    if (cabeza == nullptr) {
        nuevo->next = nuevo;
        nuevo->prev = nuevo;
        cabeza      = nuevo;
        return;
    }

    Movimiento* ultimo  = cabeza->prev;
    ultimo->next        = nuevo;
    nuevo->prev         = ultimo;
    nuevo->next         = cabeza;
    cabeza->prev        = nuevo;
}