#include "lab2_doubly_linked_list.h"
#include <iostream>
using namespace std;

Cart::Cart() : head(nullptr), tail(nullptr), cur(nullptr), count(0) {}
Cart::~Cart() { clear(); }

CartNode* Cart::find(int id) const {
    for (CartNode* n = head; n; n = n->next) if (n->it.id == id) return n;
    return nullptr;
}
void Cart::add(const LineItem& li) {
    if (CartNode* e = find(li.id)) { e->it.qty += li.qty; return; }
    CartNode* n = new CartNode{ li, tail, nullptr };
    if (tail) tail->next = n; else head = n;
    tail = n;
    count++;
}
void Cart::removeCur() {
    CartNode* v = cur;
    if (v->prev) v->prev->next = v->next; else head = v->next;
    if (v->next) v->next->prev = v->prev; else tail = v->prev;
    cur = v->next ? v->next : v->prev;
    delete v;
    count--;
}
void Cart::clear() {
    while (head) { CartNode* t = head; head = head->next; delete t; }
    tail = cur = nullptr;
    count = 0;
}
double Cart::total() const {
    double s = 0;
    for (CartNode* n = head; n; n = n->next) s += n->it.price * n->it.qty;
    return s;
}
void Cart::print() const {
    if (!head) { cout << "  (cart is empty)\n"; return; }
    int k = 0;
    for (CartNode* n = head; n; n = n->next)
        cout << "  " << ++k << ". [" << n->it.id << "] " << n->it.name << " (" << n->it.brand << ")  "
        << n->it.qty << " x " << money(n->it.price) << " = " << money(n->it.qty * n->it.price) << "\n";
    cout << "  TOTAL: PHP " << money(total()) << "\n  Reverse walk (prev links): ";
    for (CartNode* n = tail; n; n = n->prev) cout << "#" << n->it.id << (n->prev ? " -> " : "\n");
}
