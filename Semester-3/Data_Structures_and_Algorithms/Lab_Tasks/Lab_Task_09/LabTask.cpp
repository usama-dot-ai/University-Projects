#include <iostream>
using namespace std;

// int fun(int n)
// {
//     if (n == 1)
//     {
//         return 1;
//     }
//     return n * fun(n - 1);
// }
// int pow(int n, int p)
// {
//     if (p == 0)
//     {
//         return 1;
//     }
//     return n * pow(n, p - 1);
// }
class TNode
{
public:
    int data;
    TNode *left = nullptr;
    TNode *Right = nullptr;
};
class BST
{
private:
    TNode *root;

public:
    BST() : root(nullptr) {}
    TNode *createNode(int val)
    {
        TNode *newNode = new TNode();
        newNode->data = val;
        newNode->left = nullptr;
        newNode->Right = nullptr;
        return newNode;
    }
    TNode *insert(TNode *node, int val)
    {
        if (node == nullptr)
        {
            node = createNode(val);
        }
        else if (val < node->data)
        {
            node->left = insert(node->left, val);
        }
        else if (val > node->data)
        {
            node->Right = insert(node->Right, val);
        }
        return node;
    }
    void insertvalue(int data)
    {
        root = insert(root, data);
        cout << root->left;
    }
};

int main()
{
    BST b;
    b.insertvalue(10);
    b.insertvalue(8);
    b.insertvalue(12);

    // int n = 10;
    // int result = fun(n);
    // cout << result << endl;

    // int n1 = 2, p = 10;
    // int result1 = pow(n1, p);
    // cout << result1 << endl;

    return 0;
}