#include <iostream>
using namespace std;

class TNode
{
public:
    int data;
    TNode *left = nullptr;
    TNode *right = nullptr;
};
class Tree
{
private:
    TNode *root = nullptr;

    TNode *createNode(int val)
    {
        TNode *newNode = new TNode();
        newNode->data = val;
        newNode->left = nullptr;
        newNode->right = nullptr;

        return newNode;
    }
    TNode *insertNode(int val, TNode *rootNode)
    {
        if (rootNode == nullptr)
        {
            return createNode(val);
        }

        if (val < rootNode->data)
        {
            rootNode->left = insertNode(val, rootNode->left);
        }
        else
        {
            rootNode->right = insertNode(val, rootNode->right);
        }

        return rootNode;
    }
    void inOrder(TNode *rootNode)
    {
        if (rootNode == nullptr)
        {
            return;
        }
        inOrder(rootNode->left);
        cout << rootNode->data << " ";
        inOrder(rootNode->right);
    }
    TNode *inOrderSuccessor(TNode *rootNode)
    {
        while (rootNode != nullptr && rootNode->left != nullptr)
        {
            rootNode = rootNode->left;
        }
        return rootNode;
    }
    TNode *deletion(TNode *rootNode, int val)
    {
        if (rootNode == nullptr)
        {
            return nullptr;
        }
        if (val < rootNode->data)
        {
            rootNode->left = deletion(rootNode->left, val);
        }
        else if (val > rootNode->data)
        {
            rootNode->right = deletion(rootNode->right, val);
        }
        else
        {
            if (rootNode->left == nullptr)
            {
                TNode *temp = rootNode->right;
                delete rootNode;
                return temp;
            }
            else if (rootNode->right == nullptr)
            {
                TNode *temp = rootNode->left;
                delete rootNode;
                return temp;
            }
            else
            {
                TNode *ios = inOrderSuccessor(rootNode);
                rootNode->data = ios->data;
                rootNode->right = deletion(rootNode, val);
            }
        }
        return rootNode;
    }
    int findMin(TNode *rootNode)
    {
        if (rootNode == nullptr)
        {
            return INT_MAX;
        }
        while (rootNode->left != nullptr)
        {
            rootNode = rootNode->left;
        }
        return rootNode->data;
    }
    int findMax(TNode *rootNode)
    {
        if (rootNode == nullptr)
        {
            return INT_MIN;
        }
        while (rootNode->right != nullptr)
        {
            rootNode = rootNode->right;
        }
        return rootNode->data;
    }

public:
    void insertValue(int val)
    {
        root = insertNode(val, root);
    }
    void traverseInOrder()
    {
        cout << "\nIn Order: ";
        inOrder(root);
    }
    void deleteNode(int value)
    {
        root = deletion(root, value);
    }
    void calculateMin()
    {
        cout << "\nMinimum: " << findMin(root);
    }
    void calculateMax()
    {
        cout << "\nMaximum: " << findMax(root);
    }
};

int main()
{
    Tree t1;
    t1.insertValue(10);
    t1.insertValue(8);
    t1.insertValue(12);
    t1.insertValue(6);
    t1.insertValue(14);
    t1.insertValue(4);

    t1.traverseInOrder();
    t1.deleteNode(12);
    t1.traverseInOrder();

    t1.calculateMin();
    t1.calculateMax();

    return 0;
}