#include "lab1_dynamic_allocation.h"
#include <iostream>
using namespace std;

ProductArray::ProductArray(int capacity) : a(new Product* [capacity]), n(0), cap(capacity), verbose(false) {}
ProductArray::~ProductArray() { delete[] a; }

void ProductArray::push(Product* p) {
    if (n == cap) {
        Product** bigger = new Product * [cap * 2];
        for (int i = 0; i < n; i++) bigger[i] = a[i];
        delete[] a;
        a = bigger;
        if (verbose) cout << "  Array full: resized " << cap << " -> " << cap * 2 << " slots\n";
        cap *= 2;
    }
    a[n++] = p;
}
void ProductArray::removeAt(int i) { for (; i < n - 1; i++) a[i] = a[i + 1]; n--; }
