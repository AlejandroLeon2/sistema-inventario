#pragma once
#include "../models/Product.hpp"

// Búsqueda lineal en arreglo
Product* linearSearch(Product arr[], int n, int codigo);

// Búsqueda binaria en arreglo ordenado
Product* binarySearch(Product arr[], int n, int codigo);
