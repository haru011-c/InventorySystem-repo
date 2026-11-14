// cart.h - LAB 2: doubly linked list (shopping cart)
#pragma once
#include "models.h"
#include "utils.h"

struct CartNode { LineItem it; CartNode* prev; CartNode* next; };

class Cart : NoCopy {
public:
    CartNode* head, * tail, * cur;
    int count;
    Cart();
    ~Cart();
    CartNode* find(int id) const;
    void add(const LineItem& li);
    void removeCur();
    void clear();
    double total() const;
    void print() const;
};
