// product_array.h - LAB 1: dynamic allocation (growable array of Product*)
#pragma once
#include "models.h"
#include "utils.h"

class ProductArray : NoCopy {
    Product** a;
    int n, cap;
public:
    bool verbose;
    explicit ProductArray(int capacity = 8);
    ~ProductArray();
    void push(Product* p);
    void removeAt(int i);
    int size() const { return n; }
    int capacity() const { return cap; }
    Product* operator[](int i) const { return a[i]; }
    Product** data() const { return a; }
};
