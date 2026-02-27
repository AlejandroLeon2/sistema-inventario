#include <iostream>
#include "models/Product.hpp"
#include "structures/CircularList.hpp"
#include "algorithms/Sorting.hpp"
#include "algorithms/Searching.hpp"
using namespace std;

int main()
{
    CircularList lista;
    lista.insert({101, "Laptop", 2500.0, 10});
    lista.insert({102, "Mouse", 25.0, 50});
    lista.insert({103, "Teclado", 45.0, 30});

    cout << "Inventario actual:\n";
    lista.display();
    return 0;
}
