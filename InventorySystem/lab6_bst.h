// product_bst.h - LAB 6: binary search tree indexed by Product ID
#pragma once
#include "models.h"
#include "utils.h"
#include "lab1_dynamic_allocation.h"

struct TreeNode { Product* p; TreeNode* l, * r; };

class ProductBST : NoCopy {
    TreeNode* root;
    static TreeNode* ins(TreeNode* t, Product* p);
    static TreeNode* del(TreeNode* t, int id);
    static void kill(TreeNode* t);
    static void walk(TreeNode* t, ProductArray& out);
    static int h(TreeNode* t);
public:
    ProductBST();
    ~ProductBST();
    void insert(Product* p);
    void remove(int id);
    void inorder(ProductArray& out) const;
    int height() const;
    Product* find(int id, int& steps) const;
};
