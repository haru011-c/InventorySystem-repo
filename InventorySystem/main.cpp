/*====================================================
 *  SHEIN BRAND-AFFILIATED INVENTORY & SEARCH SYSTEM
 *====================================================*/
#include <iostream>
#include "store_app.h"
#include "console.h"
using namespace std;

int main() {
    CenterOutput centered;              // everything printed below is centered in the terminal
    StoreApp app;                       // owns everything; its destructors free all memory on exit
    try { app.run(); }
    catch (const InputClosed&) { cout << "\n[Input closed] Shutting down safely...\n"; }
    return 0;
}
