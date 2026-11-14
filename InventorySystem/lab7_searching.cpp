#include "lab7_searching.h"
#include "utils.h"
using namespace std;

int linearSearch(const ProductArray& src, const string& kw, ProductArray& out) {
    string k = lower(kw);
    for (int i = 0; i < src.size(); i++) {
        const Product* p = src[i];
        if (lower(p->name).find(k) != string::npos || lower(p->category).find(k) != string::npos ||
            lower(p->brand).find(k) != string::npos) out.push(src[i]);
    }
    return src.size();
}
int bound(Product** a, int n, double x, bool upper, int& cmp) {
    int lo = 0, hi = n;
    while (lo < hi) {
        int mid = lo + (hi - lo) / 2;
        cmp++;
        if (upper ? a[mid]->price <= x : a[mid]->price < x) lo = mid + 1; else hi = mid;
    }
    return lo;
}
