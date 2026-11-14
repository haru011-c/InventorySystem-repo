#include "order_queue.h"
#include <iostream>
using namespace std;

Order::Order(int i, const string& w, int cnt) : id(i), who(w), lines(new LineItem[cnt]()), n(cnt), total(0) {}
Order::~Order() { delete[] lines; }

OrderQueue::OrderQueue() : front(nullptr), rear(nullptr), n(0) {}
OrderQueue::~OrderQueue() { while (Order* o = dequeue()) delete o; }

void OrderQueue::enqueue(Order* o) {
    QNode* q = new QNode{ o, nullptr };
    if (rear) rear->next = q; else front = q;
    rear = q;
    n++;
}
Order* OrderQueue::dequeue() {
    if (!front) return nullptr;
    QNode* q = front;
    Order* o = q->o;
    front = q->next;
    if (!front) rear = nullptr;
    delete q;
    n--;
    return o;
}
void OrderQueue::print() const {
    if (!front) { cout << "  (no pending orders)\n"; return; }
    int pos = 0;
    for (QNode* q = front; q; q = q->next) {
        pos++;
        cout << "  " << pos << ". Order #" << q->o->id << " - " << q->o->who << " - " << q->o->n << " line(s) - PHP "
            << money(q->o->total) << (pos == 1 ? "   <- FRONT (served next)" : "") << "\n";
    }
}
