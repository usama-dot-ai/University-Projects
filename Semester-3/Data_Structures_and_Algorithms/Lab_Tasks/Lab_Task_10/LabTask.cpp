#include <iostream>
using namespace std;

class Tnode
{
public:
    int data;
    Tnode *left;
    Tnode *right;
};
class Tree
{
private:
    Tnode *root;

public:
    Tree() : root(nullptr) {}
    Tnode *createNode(int val)
    {
        Tnode *newNode = new Tnode();
        newNode->data = val;
        newNode->left = nullptr;
        newNode->right = nullptr;
        return newNode;
    }
    Tnode *insertNode(Tnode *node, int val)
    {
        if (node == nullptr)
        {
            return createNode(val);
        }
        if (val < node->data)
        {
            node->left = insertNode(node->left, val);
        }
        else
        {
            node->right = insertNode(node->right, val);
        }
        return node;
    }
    void insertValue()
    {
        cout << "Enter value to insert: ";
        int val;
        cin >> val;
        root = insertNode(root, val);
    }
    void preOrder(Tnode *node)
    {
        if (node == nullptr)
        {
            return;
        }
        cout << node->data << " ";
        preOrder(node->left);
        preOrder(node->right);
    }
    void inOrder(Tnode *node)
    {
        if (node == nullptr)
        {
            return;
        }
        inOrder(node->left);
        cout << node->data << " ";
        inOrder(node->right);
    }
    void postOrder(Tnode *node)
    {
        if (node == nullptr)
        {
            return;
        }
        postOrder(node->left);
        postOrder(node->right);
        cout << node->data << " ";
    }
    Tnode *getroot()
    {
        return root;
    }
};
int main()
{
    Tree t1;
    t1.insertValue();
    t1.insertValue();
    t1.insertValue();

    t1.preOrder(t1.getroot());
    cout << endl;
    t1.inOrder(t1.getroot());
    cout << endl;
    t1.postOrder(t1.getroot());
    cout << endl;

    return 0;
}