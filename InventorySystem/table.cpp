#include "table.h"
#include "utils.h"
#include <iostream>
#include <iomanip>
using namespace std;

void table(const string& title, const ProductArray& v) {
    cout << "\n=== " << title << " ===\n";
    if (v.size() == 0) { cout << "  (no products)\n"; return; }
    const string rule = "+" + string(7, '-') + "+" + string(28, '-') + "+" + string(13, '-') + "+" +
        string(14, '-') + "+" + string(13, '-') + "+" + string(7, '-') + "+\n";
    cout << rule << "| ID    | Product Name               | Category    | Brand        | Price (PHP) | Stock |\n" << rule;
    for (int i = 0; i < v.size(); i++) {
        const Product* p = v[i];
        cout << "| " << left << setw(5) << p->id << " | " << setw(26) << fit(p->name, 26) << " | " << setw(11)
            << fit(p->category, 11) << " | " << setw(12) << p->brand << " | " << right << setw(11) << money(p->price)
            << " | " << setw(5) << p->stockQuantity << " |\n";
    }
    cout << rule;
}
