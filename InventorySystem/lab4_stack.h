// action_stack.h - LAB 4: stack (LIFO) used for admin undo / redo
#pragma once
#include <string>
#include "models.h"
#include "utils.h"

enum ActionType { A_ADD, A_REMOVE, A_STOCK };
struct Action {
    ActionType type;
    Product    snap;
    int        delta;
};
std::string describe(const Action& a);

struct StackNode { Action a; StackNode* below; };

class ActionStack : NoCopy {
    StackNode* top;
    int n;
public:
    ActionStack();
    ~ActionStack();
    void push(const Action& a);
    bool peek(Action& out) const;
    bool pop(Action& out);
    void clear();
    int size() const { return n; }
    void print(const std::string& title) const;
};
