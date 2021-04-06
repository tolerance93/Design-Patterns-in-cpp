#include <iostream>
#include <vector>
using namespace std;

template <typename T>
struct Node
{
    T value;
    Node *left{nullptr}, *right{nullptr}, *parent{nullptr};

    Node(T value) : value(value) {}

    Node(T value, Node<T> *left, Node<T> *right) : value(value), left(left), right(right) {
      left->parent = right->parent = this;
    }

    // traverse the node and its children preorder
    // and put all the results into `result`
    void preorder_traversal(vector<Node<T>*>& result)
    {
        preorder(this, result);
    }
    
    void preorder(Node* node, vector<Node<T>*>& result)
    {
        if (!node) return;
        result.push_back(node);
        if (node->left) preorder(node->left, result);
        if (node->right) preorder(node->right, result);
    }
};

int main()
{
    return 0;
}
