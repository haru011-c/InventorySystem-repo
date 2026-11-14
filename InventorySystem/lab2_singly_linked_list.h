// wishlist.h - LAB 2: singly linked list (wishlist)
#pragma once
#include <string>
#include "utils.h"

struct WishNode { int id; std::string name; double price; WishNode* next; };

class Wishlist : NoCopy {
    WishNode* head;
public:
    Wishlist();
    ~Wishlist();
    bool has(int id) const;
    bool add(int id, const std::string& name, double price);
    bool remove(int id);
    void print() const;
};
