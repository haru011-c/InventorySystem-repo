#include "action_stack.h"
#include <iostream>
using namespace std;

string describe(const Action& a) {
    string t = a.type == A_ADD ? "ADD    " : (a.type == A_REMOVE ? "REMOVE " : "STOCK  ");
    string extra = a.type == A_STOCK ? " (" + string(a.delta > 0 ? "+" : "") + str(a.delta) + " units)" : "";
    return t + "#" + str(a.snap.id) + " " + a.snap.name + extra;
}

ActionStack::ActionStack() : top(nullptr), n(0) {}
ActionStack::~ActionStack() { clear(); }

void ActionStack::push(const Action& a) { top = new StackNode{ a, top }; n++; }
bool ActionStack::peek(Action& out) const { if (!top) return false; out = top->a; return true; }
bool ActionStack::pop(Action& out) {
    if (!top) return false;
    StackNode* t = top;
    out = t->a;
    top = t->below;
    delete t;
    n--;
    return true;
}
void ActionStack::clear() { Action x; while (pop(x)) {} }
void ActionStack::print(const string& title) const {
    cout << "  " << title << ": " << n << " action(s)\n";
    for (StackNode* s = top; s; s = s->below) cout << "    " << (s == top ? "TOP> " : "     ") << describe(s->a) << "\n";
}
