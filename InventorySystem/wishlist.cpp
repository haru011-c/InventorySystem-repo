#include "wishlist.h"
#include <iostream>
using namespace std;

Wishlist::Wishlist() : head(nullptr) {}
Wishlist::~Wishlist() { while (head) { WishNode* t = head; head = head->next; delete t; } }

bool Wishlist::has(int id) const {
    for (WishNode* n = head; n; n = n->next) if (n->id == id) return true;
    return false;
}
bool Wishlist::add(int id, const string& name, double price) {
    if (has(id)) return false;
    WishNode** link = &head;
    while (*link) link = &(*link)->next;
    *link = new WishNode{ id, name, price, nullptr };
    return true;
}
bool Wishlist::remove(int id) {
    for (WishNode** link = &head; *link; link = &(*link)->next)
        if ((*link)->id == id) { WishNode* dead = *link; *link = dead->next; delete dead; return true; }
    return false;
}
void Wishlist::print() const {
    if (!head) { cout << "  (wishlist is empty)\n"; return; }
    for (WishNode* n = head; n; n = n->next) cout << "  [" << n->id << "] " << n->name << "  PHP " << money(n->price) << "\n";
}
