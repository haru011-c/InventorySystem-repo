// sorting.h - LAB 8: insertion sort and quick sort over a view of Product pointers
#pragma once
#include "models.h"

struct Cmp {                                // field: 1 price, 2 stock, 3 brand
    int field; bool desc;
    bool operator()(const Product* a, const Product* b) const;
};
struct Stats { long cmp, moves; };

void insertionSort(Product** a, int n, const Cmp& less, Stats& s);   // O(n^2), stable
void quickSort(Product** a, int lo, int hi, const Cmp& less, Stats& s); // O(n log n) average
