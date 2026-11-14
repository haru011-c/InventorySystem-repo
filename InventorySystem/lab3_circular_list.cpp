#include "lab3_circular_list.h"
using namespace std;

Showcase::Showcase() : tail(nullptr), count(0) {}
Showcase::~Showcase() {
    if (!tail) return;
    Banner* n = tail->next;
    tail->next = nullptr;                       // break the ring so the loop can end
    while (n) { Banner* t = n->next; delete n; n = t; }
}
void Showcase::append(const string& brand, const string& tagline) {
    Banner* b = new Banner{ brand, tagline, nullptr };
    if (!tail) b->next = b;
    else { b->next = tail->next; tail->next = b; }
    tail = b;
    count++;
}
string Showcase::brandAt(int i) const {
    Banner* b = head();
    while (i-- > 0) b = b->next;
    return b->brand;
}
