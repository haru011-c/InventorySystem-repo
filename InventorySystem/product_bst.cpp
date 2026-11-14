#include "product_bst.h"
#include <algorithm>
using namespace std;

TreeNode* ProductBST::ins(TreeNode* t, Product* p) {
    if (!t) return new TreeNode{ p, nullptr, nullptr };
    if (p->id < t->p->id) t->l = ins(t->l, p); else t->r = ins(t->r, p);
    return t;
}
TreeNode* ProductBST::del(TreeNode* t, int id) {
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
void ProductBST::kill(TreeNode* t) { if (t) { kill(t->l); kill(t->r); delete t; } }
void ProductBST::walk(TreeNode* t, ProductArray& out) { if (t) { walk(t->l, out); out.push(t->p); walk(t->r, out); } }
int  ProductBST::h(TreeNode* t) { return t ? 1 + max(h(t->l), h(t->r)) : 0; }

ProductBST::ProductBST() : root(nullptr) {}
ProductBST::~ProductBST() { kill(root); }
void ProductBST::insert(Product* p) { root = ins(root, p); }
void ProductBST::remove(int id) { root = del(root, id); }
void ProductBST::inorder(ProductArray& out) const { walk(root, out); }
int  ProductBST::height() const { return h(root); }
Product* ProductBST::find(int id, int& steps) const {
    steps = 0;
    for (TreeNode* t = root; t; t = id < t->p->id ? t->l : t->r) {
        steps++;
        if (id == t->p->id) return t->p;
    }
    return nullptr;
}
