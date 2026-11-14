// order_queue.h - LAB 5: queue (FIFO) of checkout orders
#pragma once
#include <string>
#include "models.h"
#include "utils.h"

struct Order : NoCopy {
    int id; std::string who; LineItem* lines; int n; double total;
    Order(int i, const std::string& w, int cnt);
    ~Order();
};

struct QNode { Order* o; QNode* next; };

class OrderQueue : NoCopy {
    QNode* front, * rear;
    int n;
public:
    OrderQueue();
    ~OrderQueue();
    void enqueue(Order* o);
    Order* dequeue();
    int size() const { return n; }
    void print() const;
};
