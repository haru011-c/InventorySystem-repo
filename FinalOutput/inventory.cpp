/*====================================================
 *  SHEIN BRAND-AFFILIATED INVENTORY & SEARCH SYSTEM
 *====================================================*/
#include <iostream>
#include <iomanip>
#include <sstream>
#include <string>
#include <algorithm>
#include <cstdlib>
#include <cerrno> 
#include <cctype>
using namespace std;

struct InputClosed {};
struct NoCopy {
    NoCopy() {}
    NoCopy(const NoCopy&) = delete;
    NoCopy& operator=(const NoCopy&) = delete;
};

string str(long v) { ostringstream o; o << v; return o.str(); }
string money(double v) { ostringstream o; o << fixed << setprecision(2) << v; return o.str(); }
string lower(string s) { for (size_t i = 0; i < s.size(); i++) s[i] = (char)tolower((unsigned char)s[i]); return s; }
string fit(const string& s, size_t w) { return s.size() <= w ? s : s.substr(0, w - 2) + ".."; }
string trim(const string& s) {
    size_t a = s.find_first_not_of(" \t\r\n");
    return a == string::npos ? "" : s.substr(a, s.find_last_not_of(" \t\r\n") - a + 1);
}

string readLine(const string& prompt) {
    cout << prompt;
    string s;
    if (!getline(cin, s)) throw InputClosed();
    return trim(s);
}
int readInt(const string& prompt, int lo, int hi) {
    while (true) {
        string s = readLine(prompt);
        char* end; errno = 0;
        long v = strtol(s.c_str(), &end, 10);
        if (!s.empty() && *end == '\0' && errno == 0 && v >= lo && v <= hi) return (int)v;
        cout << "  [!] Enter a whole number from " << lo << " to " << hi << ".\n";
    }
}
double readDouble(const string& prompt, double lo, double hi) {
    while (true) {
        string s = readLine(prompt);
        char* end; errno = 0;
        double v = strtod(s.c_str(), &end);
        if (!s.empty() && *end == '\0' && errno == 0 && v >= lo && v <= hi) return v;
        cout << "  [!] Enter an amount from " << money(lo) << " to " << money(hi) << ".\n";
    }
}
string readText(const string& prompt, size_t maxLen = 40) {
    while (true) {
        string s = readLine(prompt);
        if (!s.empty() && s.size() <= maxLen) return s;
        cout << "  [!] Enter 1 to " << maxLen << " characters.\n";
    }
}

struct Product {
    int    id;
    string name;
    string category;
    string brand;
    double price;
    int    stockQuantity;
};
struct LineItem { int id; string name, brand; double price; int qty; };

static const char* BRANDS[5] = { "SHEIN EZwear", "SHEIN Curve", "ROMWE", "DAZY", "SHEGLAM" };
static const char* TAGLINES[5] = { "Easy everyday basics for effortless style.", "Fashion with room to be you.",
                                  "Dark, bold and proudly different.", "Trend-forward looks for every day.",
                                  "Glow-up essentials for every shade." };
struct Seed { int id; const char* name; const char* cat; int brand; double price; int stock; };
static const Seed SEED[] = {                      // brand = index into BRANDS
    { 1001, "Ribbed Knit Crop Tank",      "Tops",        0,  199,  45 },{ 1002, "High-Waist Wide-Leg Jeans",  "Bottoms",     0,  749,  30 },
    { 1003, "Oversized Graphic Tee",      "Tops",        0,  299,  60 },{ 1004, "Drawstring Jogger Pants",    "Bottoms",     0,  449,   8 },
    { 1005, "Casual Zip-Up Hoodie",       "Outerwear",   0,  899,  22 },{ 1006, "Plus Floral Wrap Dress",     "Dresses",     1,  849,  18 },
    { 1007, "Plus Stretch Leggings",      "Bottoms",     1,  349,  40 },{ 1008, "Plus Button-Front Blouse",   "Tops",        1,  499,  25 },
    { 1009, "Plus Denim Jacket",          "Outerwear",   1, 1199,  12 },{ 1010, "Plus Ruffle Hem Maxi Skirt", "Bottoms",     1,  649,   0 },
    { 1011, "Gothic Lace-Up Corset Top",  "Tops",        2,  599,  14 },{ 1012, "Cargo Pants with Chain",     "Bottoms",     2,  999,  20 },
    { 1013, "Plaid Pleated Mini Skirt",   "Bottoms",     2,  449,  33 },{ 1014, "Y2K Graphic Baby Tee",       "Tops",        2,  249,  50 },
    { 1015, "Studded Platform Boots",     "Footwear",    2, 1499,   6 },{ 1016, "Satin Slip Midi Dress",      "Dresses",     3,  799,  16 },
    { 1017, "Cutout Halter Top",          "Tops",        3,  349,  28 },{ 1018, "Lace Trim Cami Set",         "Tops",        3,  399,  35 },
    { 1019, "Pleated Tennis Skirt",       "Bottoms",     3,  449,   3 },{ 1020, "Chunky Chain Shoulder Bag",  "Accessories", 3,  549,  24 },
    { 1021, "Matte Liquid Lipstick",      "Beauty",      4,  249, 120 },{ 1022, "Glow Highlighter Palette",   "Beauty",      4,  399,  55 },
    { 1023, "Volume Mascara",             "Beauty",      4,  199,  80 },{ 1024, "Loose Setting Powder",       "Beauty",      4,  299,  42 },
    { 1025, "Brow Pencil Duo",            "Beauty",      4,  149,   9 } };
static const int SEEDS = (int)(sizeof(SEED) / sizeof(SEED[0]));

//=============================
//  LAB 1 - DYNAMIC ALLOCATION
//=============================
class ProductArray : NoCopy {
    Product** a;
    int n, cap;
public:
    bool verbose;
    explicit ProductArray(int capacity = 8) : a(new Product* [capacity]), n(0), cap(capacity), verbose(false) {}
    ~ProductArray() { delete[] a; }
    void push(Product* p) {
        if (n == cap) {
            Product** bigger = new Product * [cap * 2];
            for (int i = 0; i < n; i++) bigger[i] = a[i];
            delete[] a;
            a = bigger;
            if (verbose) cout << "  Array full: resized " << cap << " -> " << cap * 2 << " slots\n";
            cap *= 2;
        }
        a[n++] = p;
    }
    void removeAt(int i) { for (; i < n - 1; i++) a[i] = a[i + 1]; n--; }
    int size() const { return n; }
    int capacity() const { return cap; }
    Product* operator[](int i) const { return a[i]; }
    Product** data() const { return a; }
};

//=======================
//  LAB 2 - LINKED LISTS
//=======================
struct CartNode { LineItem it; CartNode* prev; CartNode* next; };
class Cart : NoCopy {
public:
    CartNode* head, * tail, * cur;
    int count;
    Cart() : head(nullptr), tail(nullptr), cur(nullptr), count(0) {}
    ~Cart() { clear(); }
    CartNode* find(int id) const {
        for (CartNode* n = head; n; n = n->next) if (n->it.id == id) return n;
        return nullptr;
    }
    void add(const LineItem& li) {
        if (CartNode* e = find(li.id)) { e->it.qty += li.qty; return; }
        CartNode* n = new CartNode{ li, tail, nullptr };
        if (tail) tail->next = n; else head = n;
        tail = n;
        count++;
    }
    void removeCur() {
        CartNode* v = cur;
        if (v->prev) v->prev->next = v->next; else head = v->next;
        if (v->next) v->next->prev = v->prev; else tail = v->prev;
        cur = v->next ? v->next : v->prev;
        delete v;
        count--;
    }
    void clear() {
        while (head) { CartNode* t = head; head = head->next; delete t; }
        tail = cur = nullptr;
        count = 0;
    }
    double total() const { double s = 0; for (CartNode* n = head; n; n = n->next) s += n->it.price * n->it.qty; return s; }
    void print() const {
        if (!head) { cout << "  (cart is empty)\n"; return; }
        int k = 0;
        for (CartNode* n = head; n; n = n->next)
            cout << "  " << ++k << ". [" << n->it.id << "] " << n->it.name << " (" << n->it.brand << ")  "
            << n->it.qty << " x " << money(n->it.price) << " = " << money(n->it.qty * n->it.price) << "\n";
        cout << "  TOTAL: PHP " << money(total()) << "\n  Reverse walk (prev links): ";
        for (CartNode* n = tail; n; n = n->prev) cout << "#" << n->it.id << (n->prev ? " -> " : "\n");
    }
};

struct WishNode { int id; string name; double price; WishNode* next; };
class Wishlist : NoCopy {
    WishNode* head;
public:
    Wishlist() : head(nullptr) {}
    ~Wishlist() { while (head) { WishNode* t = head; head = head->next; delete t; } }
    bool has(int id) const { for (WishNode* n = head; n; n = n->next) if (n->id == id) return true; return false; }
    bool add(int id, const string& name, double price) {
        if (has(id)) return false;
        WishNode** link = &head;
        while (*link) link = &(*link)->next;
        *link = new WishNode{ id, name, price, nullptr };
        return true;
    }
    bool remove(int id) {
        for (WishNode** link = &head; *link; link = &(*link)->next)
            if ((*link)->id == id) { WishNode* dead = *link; *link = dead->next; delete dead; return true; }
        return false;
    }
    void print() const {
        if (!head) { cout << "  (wishlist is empty)\n"; return; }
        for (WishNode* n = head; n; n = n->next) cout << "  [" << n->id << "] " << n->name << "  PHP " << money(n->price) << "\n";
    }
};

//===============================
//  LAB 3 - CIRCULAR LINKED LIST
//===============================
struct Banner { string brand, tagline; Banner* next; };
class Showcase : NoCopy {
    Banner* tail;
public:
    int count;
    Showcase() : tail(nullptr), count(0) {}
    ~Showcase() {
        if (!tail) return;
        Banner* n = tail->next;
        tail->next = nullptr;
        while (n) { Banner* t = n->next; delete n; n = t; }
    }
    void append(const string& brand, const string& tagline) {
        Banner* b = new Banner{ brand, tagline, nullptr };
        if (!tail) b->next = b;
        else { b->next = tail->next; tail->next = b; }
        tail = b;
        count++;
    }
    Banner* head() const { return tail ? tail->next : nullptr; }
    string brandAt(int i) const { Banner* b = head(); while (i-- > 0) b = b->next; return b->brand; }
};

//=======================
//  LAB 4 - STACK (LIFO)
//=======================
enum ActionType { A_ADD, A_REMOVE, A_STOCK };
struct Action {
    ActionType type;
    Product    snap;
    int        delta;
};
string describe(const Action& a) {
    string t = a.type == A_ADD ? "ADD    " : (a.type == A_REMOVE ? "REMOVE " : "STOCK  ");
    string extra = a.type == A_STOCK ? " (" + string(a.delta > 0 ? "+" : "") + str(a.delta) + " units)" : "";
    return t + "#" + str(a.snap.id) + " " + a.snap.name + extra;
}
struct StackNode { Action a; StackNode* below; };
class ActionStack : NoCopy {
    StackNode* top;
    int n;
public:
    ActionStack() : top(nullptr), n(0) {}
    ~ActionStack() { clear(); }
    void push(const Action& a) { top = new StackNode{ a, top }; n++; }
    bool peek(Action& out) const { if (!top) return false; out = top->a; return true; }
    bool pop(Action& out) {
        if (!top) return false;
        StackNode* t = top;
        out = t->a;
        top = t->below;
        delete t;
        n--;
        return true;
    }
    void clear() { Action x; while (pop(x)) {} }
    int size() const { return n; }
    void print(const string& title) const {
        cout << "  " << title << ": " << n << " action(s)\n";
        for (StackNode* s = top; s; s = s->below) cout << "    " << (s == top ? "TOP> " : "     ") << describe(s->a) << "\n";
    }
};

//======================
//  LAB 5 - QUEUE (FIFO)
//======================
struct Order : NoCopy {
    int id; string who; LineItem* lines; int n; double total;
    Order(int i, const string& w, int cnt) : id(i), who(w), lines(new LineItem[cnt]()), n(cnt), total(0) {}
    ~Order() { delete[] lines; }
};
struct QNode { Order* o; QNode* next; };
class OrderQueue : NoCopy {
    QNode* front, * rear;
    int n;
public:
    OrderQueue() : front(nullptr), rear(nullptr), n(0) {}
    ~OrderQueue() { while (Order* o = dequeue()) delete o; }
    void enqueue(Order* o) {
        QNode* q = new QNode{ o, nullptr };
        if (rear) rear->next = q; else front = q;
        rear = q;
        n++;
    }
    Order* dequeue() {
        if (!front) return nullptr;
        QNode* q = front;
        Order* o = q->o;
        front = q->next;
        if (!front) rear = nullptr;
        delete q;
        n--;
        return o;
    }
    int size() const { return n; }
    void print() const {
        if (!front) { cout << "  (no pending orders)\n"; return; }
        int pos = 0;
        for (QNode* q = front; q; q = q->next) {
            pos++;
            cout << "  " << pos << ". Order #" << q->o->id << " - " << q->o->who << " - " << q->o->n << " line(s) - PHP "
                << money(q->o->total) << (pos == 1 ? "   <- FRONT (served next)" : "") << "\n";
        }
    }
};

//==================================================
//  LAB 6 - BINARY SEARCH TREE (index by Product ID)
//==================================================
struct TreeNode { Product* p; TreeNode* l, * r; };
class ProductBST : NoCopy {
    TreeNode* root;
    static TreeNode* ins(TreeNode* t, Product* p) {
        if (!t) return new TreeNode{ p, nullptr, nullptr };
        if (p->id < t->p->id) t->l = ins(t->l, p); else t->r = ins(t->r, p);
        return t;
    }
    static TreeNode* del(TreeNode* t, int id) {
        if (!t) return nullptr;
        if (id < t->p->id) t->l = del(t->l, id);
        else if (id > t->p->id) t->r = del(t->r, id);
        else if (!t->l || !t->r) { TreeNode* child = t->l ? t->l : t->r; delete t; return child; }
        else {
            TreeNode* s = t->r;
            while (s->l) s = s->l;
            t->p = s->p;
            t->r = del(t->r, s->p->id);
        }
        return t;
    }
    static void kill(TreeNode* t) { if (t) { kill(t->l); kill(t->r); delete t; } }
    static void walk(TreeNode* t, ProductArray& out) { if (t) { walk(t->l, out); out.push(t->p); walk(t->r, out); } }
    static int h(TreeNode* t) { return t ? 1 + max(h(t->l), h(t->r)) : 0; }
public:
    ProductBST() : root(nullptr) {}
    ~ProductBST() { kill(root); }
    void insert(Product* p) { root = ins(root, p); }
    void remove(int id) { root = del(root, id); }
    void inorder(ProductArray& out) const { walk(root, out); }
    int height() const { return h(root); }
    Product* find(int id, int& steps) const {
        steps = 0;
        for (TreeNode* t = root; t; t = id < t->p->id ? t->l : t->r) {
            steps++;
            if (id == t->p->id) return t->p;
        }
        return nullptr;
    }
};

//==============================================================================
//  LAB 7 - SEARCHING
//  Linear search: O(n), works on unsorted data (keyword match).
//  Binary search: O(log n), needs data sorted by the key (price range).
//==============================================================================
int linearSearch(const ProductArray& src, const string& kw, ProductArray& out) {
    string k = lower(kw);
    for (int i = 0; i < src.size(); i++) {
        const Product* p = src[i];
        if (lower(p->name).find(k) != string::npos || lower(p->category).find(k) != string::npos ||
            lower(p->brand).find(k) != string::npos) out.push(src[i]);
    }
    return src.size();                              // records examined
}
// Index of the first product priced >= x (or > x when upper is true) in an array sorted by price.
int bound(Product** a, int n, double x, bool upper, int& cmp) {
    int lo = 0, hi = n;
    while (lo < hi) {
        int mid = lo + (hi - lo) / 2;
        cmp++;
        if (upper ? a[mid]->price <= x : a[mid]->price < x) lo = mid + 1; else hi = mid;
    }
    return lo;
}

//==============================================================================
//  LAB 8 - SORTING
//  Insertion sort: O(n^2) worst case, stable.  Quick sort: O(n log n) average.
//  They sort a VIEW (array of pointers); the catalog itself is never reordered.
//==============================================================================
struct Cmp {                                        // field: 1 price, 2 stock, 3 brand
    int field; bool desc;
    bool operator()(const Product* a, const Product* b) const {
        int c;
        if (field == 3) c = lower(a->brand).compare(lower(b->brand));
        else {
            double x = field == 1 ? a->price : a->stockQuantity, y = field == 1 ? b->price : b->stockQuantity;
            c = x < y ? -1 : (x > y ? 1 : 0);
        }
        if (c != 0) return desc ? c > 0 : c < 0;
        return a->id < b->id;                       // ties broken by ID so results are always predictable
    }
};
struct Stats { long cmp, moves; };
void insertionSort(Product** a, int n, const Cmp& less, Stats& s) {
    for (int i = 1; i < n; i++) {
        Product* key = a[i];
        int j = i - 1;
        while (j >= 0) {
            s.cmp++;
            if (!less(key, a[j])) break;
            a[j + 1] = a[j];                        // slide the bigger item right
            s.moves++;
            j--;
        }
        a[j + 1] = key;
    }
}
void quickSort(Product** a, int lo, int hi, const Cmp& less, Stats& s) {
    if (lo >= hi) return;
    swap(a[(lo + hi) / 2], a[hi]);                  // middle element is the pivot (safe on sorted data)
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

//------------------------------------------------------------------ table output
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

//==============================================================================
//  STORE APPLICATION (menus)
//==============================================================================
class StoreApp : NoCopy {
    ProductArray items;     // owns the Products (Lab 1)
    ProductBST   index;     // Product-ID index (Lab 6)
    Showcase     showcase;  // Lab 3
    Cart         cart;      // Lab 2 (doubly)
    Wishlist     wish;      // Lab 2 (singly)
    ActionStack  undo, redo;// Lab 4
    OrderQueue   orders;    // Lab 5
    int          nextOrder;

    //---- catalog helpers (every change to the catalog goes through these two)
    bool addProduct(const Product& p) {
        int s;
        if (index.find(p.id, s)) return false;      // IDs must be unique
        Product* np = new Product(p);
        items.push(np);
        index.insert(np);
        return true;
    }
    bool removeProduct(int id, Product& copy) {
        int s;
        Product* p = index.find(id, s);
        if (!p) return false;
        copy = *p;
        index.remove(id);
        for (int i = 0; i < items.size(); i++) if (items[i] == p) { items.removeAt(i); break; }
        delete p;
        return true;
    }
    void seed(int lo, int hi) {                     // median first, so the BST starts out balanced
        if (lo > hi) return;
        int m = (lo + hi) / 2;
        Product p = { SEED[m].id, SEED[m].name, SEED[m].cat, BRANDS[SEED[m].brand], SEED[m].price, SEED[m].stock };
        addProduct(p);
        seed(lo, m - 1);
        seed(m + 1, hi);
    }
    Product* ask(const string& prompt) {            // asks for an ID and finds it with the BST
        int id = readInt(prompt + " (0 = cancel): ", 0, 99999), steps;
        if (id == 0) return nullptr;
        Product* p = index.find(id, steps);
        if (!p) cout << "  [!] No product with ID " << id << ".\n";
        else cout << "  [Lab 6] BST found #" << id << " after visiting " << steps << " node(s).\n";
        return p;
    }

    //---- 1. catalog
    void viewCatalog() {
        ProductArray v(items.size() + 1);
        index.inorder(v);                           // Lab 6: in-order walk = ascending ID
        table("COMPLETE SHEIN CATALOG", v);
        long units = 0; double value = 0;
        for (int i = 0; i < v.size(); i++) { units += v[i]->stockQuantity; value += v[i]->price * v[i]->stockQuantity; }
        cout << "  Products: " << v.size() << " | Units in stock: " << units << " | Inventory value: PHP " << money(value)
            << "\n  Array capacity: " << items.capacity() << " | BST height: " << index.height() << "\n";
    }

    //---- 2. filter by brand
    void byBrand() {
        cout << "\nOfficial SHEIN partner brands:\n";
        for (int i = 0; i < showcase.count; i++) cout << "  " << i + 1 << ". " << showcase.brandAt(i) << "\n";
        int pick = readInt("Choose a brand (0 = cancel): ", 0, showcase.count);
        if (pick == 0) return;
        string brand = showcase.brandAt(pick - 1);
        ProductArray all(items.size() + 1), hits(items.size() + 1);
        index.inorder(all);
        for (int i = 0; i < all.size(); i++) if (all[i]->brand == brand) hits.push(all[i]);
        table("BRAND: " + brand, hits);
    }

    //---- 3. search (keyword = linear, Product ID = BST)
    void search() {
        cout << "\n  1. By keyword (Linear Search)\n  2. By Product ID (BST lookup)\n";
        int c = readInt("Search choice [1-2] (0 = back): ", 0, 2);
        if (c == 1) {
            string kw = readText("Keyword (name, category or brand): ");
            ProductArray all(items.size() + 1), hits(items.size() + 1);
            index.inorder(all);
            int examined = linearSearch(all, kw, hits);
            table("KEYWORD SEARCH: \"" + kw + "\"", hits);
            cout << "  Linear search examined " << examined << " products  [O(n)]\n";
        }
        else if (c == 2) {
            int id = readInt("Product ID (0 = cancel): ", 0, 99999), steps;
            if (id == 0) return;
            Product* p = index.find(id, steps);
            ProductArray one(2);
            if (p) one.push(p);
            table("PRODUCT ID " + str(id), one);
            cout << "  BST visited " << steps << " node(s); tree height " << index.height() << "  [O(log n)]\n";
        }
    }

    //---- 4. price range (quick sort, then binary search)
    void byPrice() {
        double lo = readDouble("Minimum price (PHP): ", 0, 99999.99);
        double hi = readDouble("Maximum price (PHP): ", 0, 99999.99);
        if (lo > hi) { swap(lo, hi); cout << "  (minimum was higher than maximum: swapped)\n"; }
        ProductArray v(items.size() + 1);
        for (int i = 0; i < items.size(); i++) v.push(items[i]);
        Stats st = { 0, 0 };
        quickSort(v.data(), 0, v.size() - 1, Cmp{ 1, false }, st);    // binary search needs sorted data
        int cmp = 0;
        int first = bound(v.data(), v.size(), lo, false, cmp), last = bound(v.data(), v.size(), hi, true, cmp);
        ProductArray hits(items.size() + 1);
        for (int i = first; i < last; i++) hits.push(v[i]);
        table("PRICE RANGE PHP " + money(lo) + " - " + money(hi), hits);
        cout << "  Quick Sort: " << st.cmp << " comparisons | Binary search: " << cmp << " comparisons (a linear scan needs "
            << v.size() << ")  [O(log n)]\n";
    }

    //---- 5. cart and wishlist
    void addToCart(Product* p) {
        CartNode* in = cart.find(p->id);
        int avail = p->stockQuantity - (in ? in->it.qty : 0);       // cannot exceed the remaining stock
        cout << "  " << p->name << " - PHP " << money(p->price) << " - " << p->stockQuantity << " in stock\n";
        if (avail <= 0) { cout << "  [!] No units available to add.\n"; return; }
        int q = readInt("  Quantity (1-" + str(avail) + "): ", 1, avail);
        cart.add(LineItem{ p->id, p->name, p->brand, p->price, q });
        cout << "  Added. Cart total: PHP " << money(cart.total()) << "\n";
    }
    void browse() {                                 // doubly linked list navigation
        if (!cart.head) { cout << "  Cart is empty.\n"; return; }
        cart.cur = cart.head;
        while (cart.head) {
            const LineItem& it = cart.cur->it;
            cout << "\n  Item: #" << it.id << " " << it.name << " (" << it.brand << ")  " << it.qty << " x PHP "
                << money(it.price) << "\n";
            string k = lower(readLine("  [N]ext  [P]revious  [R]emove  [B]ack > "));
            char c = k.empty() ? ' ' : k[0];
            if (c == 'n') { if (cart.cur->next) cart.cur = cart.cur->next; else cout << "  [!] This is the LAST item.\n"; }
            else if (c == 'p') { if (cart.cur->prev) cart.cur = cart.cur->prev; else cout << "  [!] This is the FIRST item.\n"; }
            else if (c == 'r') cart.removeCur();
            else if (c == 'b') return;
            else cout << "  [!] Type N, P, R or B.\n";
        }
        cout << "  Cart is now empty.\n";
    }
    void cartMenu() {
        while (true) {
            cout << "\n--- CART & WISHLIST | cart: " << cart.count << " item(s) ---\n"
                << "  1. Add product to cart\n  2. View cart\n  3. Browse cart (Next / Previous / Remove)\n"
                << "  4. Save product to wishlist\n  5. View wishlist\n  6. Remove from wishlist\n  7. Back\n";
            switch (readInt("Choice [1-7]: ", 1, 7)) {
            case 1: if (Product* p = ask("Product ID to add")) addToCart(p); break;
            case 2: cart.print(); break;
            case 3: browse(); break;
            case 4: if (Product* p = ask("Product ID to save"))
                cout << (wish.add(p->id, p->name, p->price) ? "  Saved.\n" : "  [!] Already on the wishlist.\n");
                break;
            case 5: wish.print(); break;
            case 6: {
                int id = readInt("Product ID to remove (0 = cancel): ", 0, 99999);
                if (id) cout << (wish.remove(id) ? "  Removed.\n" : "  [!] Not on the wishlist.\n");
                break;
            }
            case 7: return;
            }
        }
    }

    //---- 6. checkout queue
    void placeOrder() {
        if (cart.count == 0) { cout << "  [!] Your cart is empty - add items first (option 5).\n"; return; }
        cart.print();
        string who = readText("  Customer name: ", 30);                // ask first, allocate afterwards
        Order* o = new Order(nextOrder++, who, cart.count);
        int i = 0;
        for (CartNode* c = cart.head; c; c = c->next) { o->lines[i++] = c->it; o->total += c->it.price * c->it.qty; }
        int id = o->id; double total = o->total;
        orders.enqueue(o);                                              // the queue now owns the order
        cart.clear();
        cout << "  Order #" << id << " queued (PHP " << money(total) << "). Position: " << orders.size() << "\n";
    }
    void processNext() {                            // all-or-nothing: every line must be available
        Order* o = orders.dequeue();                // FIFO: the oldest order leaves first
        if (!o) { cout << "  The queue is empty - nothing to process.\n"; return; }
        cout << "\n>>> Order #" << o->id << " | " << o->who << " | PHP " << money(o->total) << "\n";
        string problem;
        int steps;
        for (int i = 0; i < o->n && problem.empty(); i++) {
            Product* p = index.find(o->lines[i].id, steps);
            if (!p) problem = o->lines[i].name + " is no longer in the catalog.";
            else if (p->stockQuantity < o->lines[i].qty)
                problem = "not enough stock for " + o->lines[i].name + " (ordered " + str(o->lines[i].qty) +
                ", left " + str(p->stockQuantity) + ").";
        }
        if (!problem.empty()) cout << "    REJECTED - " << problem << "\n";
        else {
            for (int i = 0; i < o->n; i++) {
                Product* p = index.find(o->lines[i].id, steps);
                cout << "    - " << o->lines[i].qty << " x " << o->lines[i].name << ": stock " << p->stockQuantity;
                p->stockQuantity -= o->lines[i].qty;
                cout << " -> " << p->stockQuantity << "\n";
            }
            cout << "    FULFILLED - PHP " << money(o->total) << "\n";
        }
        delete o;                                   // ownership came to us, so we free it
    }
    void checkoutMenu() {
        while (true) {
            cout << "\n--- CHECKOUT (Lab 5: FIFO queue) | pending orders: " << orders.size() << " ---\n"
                << "  1. Check out my cart (join the queue)\n  2. Process the NEXT order (dequeue)\n"
                << "  3. View the queue\n  4. Back\n";
            int c = readInt("Choice [1-4]: ", 1, 4);
            if (c == 1) placeOrder(); else if (c == 2) processNext(); else if (c == 3) orders.print(); else return;
        }
    }

    //---- 7. admin panel (stack-based undo / redo)
    // Applies an action forwards (add / redo) or backwards (undo). Returns false with a reason if impossible.
    bool apply(Action& a, bool forward, string& why) {
        int s;
        Product* p = index.find(a.snap.id, s);
        if (a.type == A_STOCK) {
            int d = forward ? a.delta : -a.delta;
            if (!p) { why = "the product no longer exists"; return false; }
            if (p->stockQuantity + d < 0) { why = "those units were already sold"; return false; }
            p->stockQuantity += d;
            return true;
        }
        if ((a.type == A_ADD) == forward) {         // ADD redone, or REMOVE undone: put the product (back) in
            if (p) { why = "that ID is already in use"; return false; }
            return addProduct(a.snap);
        }
        if (!p) { why = "the product no longer exists"; return false; }
        return removeProduct(a.snap.id, a.snap);    // snapshot is refreshed with the latest stock
    }
    void step(bool isUndo) {
        ActionStack& from = isUndo ? undo : redo;
        ActionStack& to = isUndo ? redo : undo;
        Action a, popped;
        if (!from.peek(a)) { cout << "  Nothing to " << (isUndo ? "undo" : "redo") << ".\n"; return; }
        string why;
        if (!apply(a, !isUndo, why)) { cout << "  [!] Blocked: " << why << ". The action stays on the stack.\n"; return; }
        from.pop(popped);                           // POP from one stack ...
        to.push(a);                                 // ... PUSH onto the other
        cout << (isUndo ? "  UNDONE: " : "  REDONE: ") << describe(a) << "\n";
    }
    void adminMenu() {
        while (true) {
            cout << "\n--- ADMIN PANEL (Lab 4: stack) | undo depth " << undo.size() << " | redo depth " << redo.size() << " ---\n"
                << "  1. Add a product\n  2. Adjust stock (+ / -)\n  3. Remove a product\n"
                << "  4. UNDO last action (pop)\n  5. REDO\n  6. View undo / redo stacks\n  7. Back\n";
            int c = readInt("Admin choice [1-7]: ", 1, 7);
            if (c == 7) return;
            if (c == 4 || c == 5) { step(c == 4); continue; }
            if (c == 6) { undo.print("UNDO STACK (top first)"); redo.print("REDO STACK (top first)"); continue; }
            Action a = {};
            if (c == 1) {
                a.type = A_ADD;
                a.snap.id = 1000;
                for (int i = 0; i < items.size(); i++) a.snap.id = max(a.snap.id, items[i]->id);
                a.snap.id++;                                            // next free ID
                a.snap.name = readText("  Product name: ", 60);
                a.snap.category = readText("  Category (e.g. Tops, Beauty): ", 20);
                for (int i = 0; i < showcase.count; i++) cout << "    " << i + 1 << ". " << showcase.brandAt(i) << "\n";
                a.snap.brand = showcase.brandAt(readInt("  Brand [1-" + str(showcase.count) + "]: ", 1, showcase.count) - 1);
                a.snap.price = readDouble("  Unit price (PHP): ", 0.01, 99999.99);
                a.snap.stockQuantity = readInt("  Starting stock: ", 0, 99999);
                addProduct(a.snap);
            }
            else if (c == 2) {
                Product* p = ask("Product ID");
                if (!p) continue;
                a.type = A_STOCK;
                a.snap = *p;
                a.delta = readInt("  Units to add (+) or remove (-): ", -p->stockQuantity, 99999);
                if (a.delta == 0) continue;
                p->stockQuantity += a.delta;
            }
            else {
                Product* p = ask("Product ID to remove");
                if (!p) continue;
                a.type = A_REMOVE;
                removeProduct(p->id, a.snap);
            }
            undo.push(a);                           // record the action; a new action clears the redo history
            redo.clear();
            cout << "  Pushed on undo stack: " << describe(a) << "\n";
        }
    }

    //---- 8. banner showcase (circular list)
    void showBanners() {
        cout << "\n=== PARTNER BRAND BANNERS ===\n";
        Banner* b = showcase.head();
        while (true) {
            int products = 0;
            for (int i = 0; i < items.size(); i++) if (items[i]->brand == b->brand) products++;
            cout << "\n  ********************************************\n  *  " << b->brand << "\n  *  \"" << b->tagline
                << "\"\n  *  " << products << " product(s) in the catalog\n  ********************************************\n";
            string in = lower(readLine("  ENTER = next banner (loops forever), Q = back: "));
            if (!in.empty() && in[0] == 'q') return;
            b = b->next;
            if (b == showcase.head()) cout << "  >> End of the ring: wrapping around to the FIRST banner!\n";
        }
    }

    //---- 9. sort
    void sortMenu() {
        cout << "\n  Sort by: 1. Price  2. Stock  3. Brand\n";
        int f = readInt("Field [1-3] (0 = cancel): ", 0, 3);
        if (f == 0) return;
        bool desc = readInt("Order: 1. Ascending  2. Descending: ", 1, 2) == 2;
        int algo = readInt("Algorithm: 1. Insertion Sort  2. Quick Sort: ", 1, 2);
        ProductArray v(items.size() + 1);
        for (int i = 0; i < items.size(); i++) v.push(items[i]);
        Stats st = { 0, 0 };
        Cmp cmp = { f, desc };
        if (algo == 1) insertionSort(v.data(), v.size(), cmp, st); else quickSort(v.data(), 0, v.size() - 1, cmp, st);
        const char* names[] = { "", "PRICE", "STOCK", "BRAND" };
        table(string("INVENTORY SORTED BY ") + names[f] + (desc ? " (desc)" : " (asc)") +
            (algo == 1 ? " - Insertion Sort" : " - Quick Sort"), v);
        cout << "  " << st.cmp << " comparisons, " << st.moves << (algo == 1 ? " shifts" : " swaps") << "\n";
    }

public:
    StoreApp() : items(4), nextOrder(1) {
        for (int i = 0; i < 5; i++) showcase.append(BRANDS[i], TAGLINES[i]);
        seed(0, SEEDS - 1);
    }
    ~StoreApp() { for (int i = 0; i < items.size(); i++) delete items[i]; }   // free every Product

    void run() {
        while (true) {
            const string bar(74, '=');
            cout << "\n" << bar << "\n  SHEIN INVENTORY & SEARCH SYSTEM\n" << bar << "\n"
                << " 1. View Complete Catalog (Tabular View with Brand Affiliations)\n"
                << " 2. Filter Products by Official Brand (e.g., SHEIN EZwear, ROMWE, DAZY)\n"
                << " 3. Search Products by Keyword (Linear Search)\n"
                << " 4. Filter Products by Price Range (Binary Search)\n"
                << " 5. Add Item to Shopping Cart (Doubly Linked List Navigation)\n"
                << " 6. Process Customer Checkout Queue (FIFO Order Queue)\n"
                << " 7. Perform Admin Undo Action (LIFO Stack Pop)\n"
                << " 8. Display Partner Brand Banner Showcase (Circular List Cycle)\n"
                << " 9. Sort Inventory Display (By Price, Stock, or Brand)\n"
                << "10. Exit\n" << bar << "\n  Cart: " << cart.count << " item(s) | Pending orders: " << orders.size()
                << " | Undo stack: " << undo.size() << "\n";
            switch (readInt("Enter your choice [1-10]: ", 1, 10)) {
            case 1: viewCatalog(); break;
            case 2: byBrand(); break;
            case 3: search(); break;
            case 4: byPrice(); break;
            case 5: cartMenu(); break;
            case 6: checkoutMenu(); break;
            case 7: adminMenu(); break;
            case 8: showBanners(); break;
            case 9: sortMenu(); break;
            case 10: cout << "\nThank you for using the SHEIN. Goodbye!\n"; return;
            }

            readLine("\n  Press ENTER to continue...");   // the menu only comes back after the user presses ENTER
        }
    }
};

int main() {
    StoreApp app;                       // owns everything; its destructors free all memory on exit
    try { app.run(); }
    catch (const InputClosed&) { cout << "\n[Input closed] Shutting down safely...\n"; }
    return 0;
}