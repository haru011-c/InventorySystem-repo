#include "store_app.h"
#include "catalog_data.h"
#include "table.h"
#include "lab7_searching.h"
#include "lab8_sorting.h"
#include <iostream>
#include <algorithm>
using namespace std;

//---- catalog helpers
bool StoreApp::addProduct(const Product& p) {
    int s;
    if (index.find(p.id, s)) return false;      // IDs must be unique
    Product* np = new Product(p);
    items.push(np);
    index.insert(np);
    return true;
}
bool StoreApp::removeProduct(int id, Product& copy) {
    int s;
    Product* p = index.find(id, s);
    if (!p) return false;
    copy = *p;
    index.remove(id);
    for (int i = 0; i < items.size(); i++) if (items[i] == p) { items.removeAt(i); break; }
    delete p;
    return true;
}
void StoreApp::seed(int lo, int hi) {           // median first, so the BST starts out balanced
    if (lo > hi) return;
    int m = (lo + hi) / 2;
    Product p = { SEED[m].id, SEED[m].name, SEED[m].cat, BRANDS[SEED[m].brand], SEED[m].price, SEED[m].stock };
    addProduct(p);
    seed(lo, m - 1);
    seed(m + 1, hi);
}
Product* StoreApp::ask(const string& prompt) {  // asks for an ID and finds it with the BST
    int id = readInt(prompt + " (0 = cancel): ", 0, 99999), steps;
    if (id == 0) return nullptr;
    Product* p = index.find(id, steps);
    if (!p) cout << "  [!] No product with ID " << id << ".\n";
    else cout << "  [Lab 6] BST found #" << id << " after visiting " << steps << " node(s).\n";
    return p;
}

//---- 1. catalog
void StoreApp::viewCatalog() {
    ProductArray v(items.size() + 1);
    index.inorder(v);                           // Lab 6: in-order walk = ascending ID
    table("COMPLETE SHEIN CATALOG", v);
    long units = 0; double value = 0;
    for (int i = 0; i < v.size(); i++) { units += v[i]->stockQuantity; value += v[i]->price * v[i]->stockQuantity; }
    cout << "  Products: " << v.size() << " | Units in stock: " << units << " | Inventory value: PHP " << money(value)
        << "\n  Array capacity: " << items.capacity() << " | BST height: " << index.height() << "\n";
}

//---- 2. filter by brand
void StoreApp::byBrand() {
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
void StoreApp::search() {
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
void StoreApp::byPrice() {
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
void StoreApp::addToCart(Product* p) {
    CartNode* in = cart.find(p->id);
    int avail = p->stockQuantity - (in ? in->it.qty : 0);       // cannot exceed the remaining stock
    cout << "  " << p->name << " - PHP " << money(p->price) << " - " << p->stockQuantity << " in stock\n";
    if (avail <= 0) { cout << "  [!] No units available to add.\n"; return; }
    int q = readInt("  Quantity (1-" + str(avail) + "): ", 1, avail);
    cart.add(LineItem{ p->id, p->name, p->brand, p->price, q });
    cout << "  Added. Cart total: PHP " << money(cart.total()) << "\n";
}
void StoreApp::browse() {                       // doubly linked list navigation
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
void StoreApp::cartMenu() {
    while (true) {
        clearScreen();
        cout << "\n--- CART & WISHLIST | cart: " << cart.count << " item(s) ---\n"
            << "  1. Add product to cart\n  2. View cart\n  3. Browse cart (Next / Previous / Remove)\n"
            << "  4. Save product to wishlist\n  5. View wishlist\n  6. Remove from wishlist\n  7. Back\n";
        int c = readInt("Choice [1-7]: ", 1, 7);
        if (c == 7) return;
        switch (c) {
        case 1: if (Product* p = ask("Product ID to add")) addToCart(p); break;
        case 2: cart.print(); break;
        case 3: browse(); continue;                 // browse has its own prompts, so no extra pause
        case 4:
            if (Product* p = ask("Product ID to save"))
                cout << (wish.add(p->id, p->name, p->price) ? "  Saved.\n" : "  [!] Already on the wishlist.\n");
            break;
        case 5: wish.print(); break;
        case 6: {
            int id = readInt("Product ID to remove (0 = cancel): ", 0, 99999);
            if (id) cout << (wish.remove(id) ? "  Removed.\n" : "  [!] Not on the wishlist.\n");
            break;
        }
        }
        readLine("\n  Press ENTER to continue...");
    }
}

//---- 6. checkout queue
void StoreApp::placeOrder() {
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
void StoreApp::processNext() {                  // all-or-nothing: every line must be available
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
void StoreApp::checkoutMenu() {
    while (true) {
        clearScreen();
        cout << "\n--- CHECKOUT (Lab 5: FIFO queue) | pending orders: " << orders.size() << " ---\n"
            << "  1. Check out my cart (join the queue)\n  2. Process the NEXT order (dequeue)\n"
            << "  3. View the queue\n  4. Back\n";
        int c = readInt("Choice [1-4]: ", 1, 4);
        if (c == 4) return;
        if (c == 1) placeOrder(); else if (c == 2) processNext(); else orders.print();
        readLine("\n  Press ENTER to continue...");
    }
}

//---- 7. admin panel (stack-based undo / redo)
// Applies an action forwards (add / redo) or backwards (undo). Returns false with a reason if impossible.
bool StoreApp::apply(Action& a, bool forward, string& why) {
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
void StoreApp::step(bool isUndo) {
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
void StoreApp::adminMenu() {
    auto pause = [] { readLine("\n  Press ENTER to continue..."); };
    while (true) {
        clearScreen();
        cout << "\n--- ADMIN PANEL (Lab 4: stack) | undo depth " << undo.size() << " | redo depth " << redo.size() << " ---\n"
            << "  1. Add a product\n  2. Adjust stock (+ / -)\n  3. Remove a product\n"
            << "  4. UNDO last action (pop)\n  5. REDO\n  6. View undo / redo stacks\n  7. Back\n";
        int c = readInt("Admin choice [1-7]: ", 1, 7);
        if (c == 7) return;
        if (c == 4 || c == 5) { step(c == 4); pause(); continue; }
        if (c == 6) { undo.print("UNDO STACK (top first)"); redo.print("REDO STACK (top first)"); pause(); continue; }
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
            if (!p) { pause(); continue; }
            a.type = A_STOCK;
            a.snap = *p;
            a.delta = readInt("  Units to add (+) or remove (-): ", -p->stockQuantity, 99999);
            if (a.delta == 0) { pause(); continue; }
            p->stockQuantity += a.delta;
        }
        else {
            Product* p = ask("Product ID to remove");
            if (!p) { pause(); continue; }
            a.type = A_REMOVE;
            removeProduct(p->id, a.snap);
        }
        undo.push(a);                           // record the action; a new action clears the redo history
        redo.clear();
        cout << "  Pushed on undo stack: " << describe(a) << "\n";
        pause();
    }
}

//---- 8. banner showcase (circular list)
void StoreApp::showBanners() {
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
void StoreApp::sortMenu() {
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

//---- construction / main loop
StoreApp::StoreApp() : items(4), nextOrder(1) {
    for (int i = 0; i < 5; i++) showcase.append(BRANDS[i], TAGLINES[i]);
    seed(0, SEEDS - 1);
}
StoreApp::~StoreApp() { for (int i = 0; i < items.size(); i++) delete items[i]; }   // free every Product

void StoreApp::run() {
    while (true) {
        clearScreen();                              // wipe the screen before showing the menu
        const string bar(74, '=');
        cout << "\n" << bar << "\n  SHEIN INVENTORY & SEARCH SYSTEM\n" << bar << "\n"
            << " 1. View Complete Catalog\n"
            << " 2. Filter Products by Official Brand\n"
            << " 3. Search Products by Keyword\n"
            << " 4. Filter Products by Price Range\n"
            << " 5. Add Item to Shopping Cart\n"
            << " 6. Process Customer Checkout Queue\n"
            << " 7. Perform Admin Undo Action\n"
            << " 8. Display Partner Brand Banner Showcase\n"
            << " 9. Sort Inventory Display\n"
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