#include "lab8_sorting.h"
#include "utils.h"
#include <algorithm>
using namespace std;

bool Cmp::operator()(const Product* a, const Product* b) const {
    int c;
    if (field == 3) c = lower(a->brand).compare(lower(b->brand));
    else {
        double x = field == 1 ? a->price : a->stockQuantity, y = field == 1 ? b->price : b->stockQuantity;
        c = x < y ? -1 : (x > y ? 1 : 0);
    }
    if (c != 0) return desc ? c > 0 : c < 0;
    return a->id < b->id;                   // ties broken by ID so results are always predictable
}

void insertionSort(Product** a, int n, const Cmp& less, Stats& s) {
    for (int i = 1; i < n; i++) {
        Product* key = a[i];
        int j = i - 1;
        while (j >= 0) {
            s.cmp++;
            if (!less(key, a[j])) break;
            a[j + 1] = a[j];                // slide the bigger item right
            s.moves++;
            j--;
        }
        a[j + 1] = key;
    }
}
void quickSort(Product** a, int lo, int hi, const Cmp& less, Stats& s) {
    if (lo >= hi) return;
    swap(a[(lo + hi) / 2], a[hi]);          // middle element is the pivot (safe on sorted data)
    Product* pivot = a[hi];
    int store = lo;
    for (int i = lo; i < hi; i++) {
        s.cmp++;
        if (less(a[i], pivot)) { swap(a[i], a[store]); s.moves++; store++; }
    }
    swap(a[store], a[hi]);
    quickSort(a, lo, store - 1, less, s);
    quickSort(a, store + 1, hi, less, s);
}
