#pragma once
#include "../models/Product.hpp"

// Búsqueda lineal en arreglo
Product* linearSearch(Product arr[], int n, int codigo){
  for (int i = 0; i < n; i++) {
        if (arr[i].codigo == codigo) {
            return &arr[i]; 
        }
    }
    return nullptr; 

};

// Búsqueda binaria en arreglo ordenado
Product* binarySearch(Product arr[], int n, int codigo);
