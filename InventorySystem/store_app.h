// store_app.h - the menu-driven application that ties all the labs together
#pragma once
#include <string>
#include "utils.h"
#include "models.h"
#include "product_array.h"
#include "product_bst.h"
#include "showcase.h"
#include "cart.h"
#include "wishlist.h"
#include "action_stack.h"
#include "order_queue.h"

class StoreApp : NoCopy {
    ProductArray items;      // owns the Products (Lab 1)
    ProductBST   index;      // Product-ID index (Lab 6)
    Showcase     showcase;   // Lab 3
    Cart         cart;       // Lab 2 (doubly)
    Wishlist     wish;       // Lab 2 (singly)
    ActionStack  undo, redo; // Lab 4
    OrderQueue   orders;     // Lab 5
    int          nextOrder;

    // catalog helpers (every change to the catalog goes through these two)
    bool addProduct(const Product& p);
    bool removeProduct(int id, Product& copy);
    void seed(int lo, int hi);
    Product* ask(const std::string& prompt);

    // menu features
    void viewCatalog();
    void byBrand();
    void search();
    void byPrice();
    void addToCart(Product* p);
    void browse();
    void cartMenu();
    void placeOrder();
    void processNext();
    void checkoutMenu();
    bool apply(Action& a, bool forward, std::string& why);
    void step(bool isUndo);
    void adminMenu();
    void showBanners();
    void sortMenu();

public:
    StoreApp();
    ~StoreApp();
    void run();
};
