// models.h - plain data records shared by every module
#pragma once
#include <string>

struct Product {
    int         id;
    std::string name;
    std::string category;
    std::string brand;
    double      price;
    int         stockQuantity;
};

struct LineItem { int id; std::string name, brand; double price; int qty; };
