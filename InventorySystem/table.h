// table.h - tabular console output for a list of products
#pragma once
#include <string>
#include "product_array.h"

void table(const std::string& title, const ProductArray& v);
