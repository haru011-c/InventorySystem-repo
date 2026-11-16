// table.h - tabular console output for a list of products
#pragma once
#include <string>
#include "lab1_dynamic_allocation.h"

void table(const std::string& title, const ProductArray& v);

void clearScreen();
