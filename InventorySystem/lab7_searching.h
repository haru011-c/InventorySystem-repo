// searching.h - LAB 7: linear search (keyword) and binary search (price range)
#pragma once
#include <string>
#include "models.h"
#include "lab1_dynamic_allocation.h"

// O(n). Returns how many records were examined.
int linearSearch(const ProductArray& src, const std::string& kw, ProductArray& out);

// O(log n). Index of the first product priced >= x (or > x when upper is true) in an array sorted by price.
int bound(Product** a, int n, double x, bool upper, int& cmp);
