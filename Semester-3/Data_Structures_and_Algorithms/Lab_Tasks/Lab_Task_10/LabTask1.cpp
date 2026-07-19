#include <iostream>
#include <vector>
#include <climits>
#include <algorithm>

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
    TNode *buildTree(vector<int> v)
    {
        TNode *rootNode = nullptr;
        for (int val : v)
        {
            rootNode = insertNode(val, rootNode);
        }
        return rootNode;
    }
    void preOrder(TNode *rootNode)
    {

        if (rootNode == nullptr)
        {
            return;
        }
        cout << rootNode->data << " ";
        preOrder(rootNode->left);
        preOrder(rootNode->right);
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
    void postOrder(TNode *rootNode)
    {
        if (rootNode == nullptr)
        {
            return;
        }
        postOrder(rootNode->left);
        postOrder(rootNode->right);
        cout << rootNode->data << " ";
    }
    int Height(TNode *rootNode)
    {
        if (rootNode == nullptr)
        {
            return 0;
        }
        int leftHeight = Height(rootNode->left);
        int rightHeight = Height(rootNode->right);

        return (max(leftHeight, rightHeight) + 1);
    }
    int count(TNode *rootNode)
    {
        if (rootNode == nullptr)
        {
            return 0;
        }
        int leftCount = count(rootNode->left);
        int rightCount = count(rootNode->right);

        return (leftCount + rightCount + 1);
    }
    int sumOfNodes(TNode *rootNode)
    {
        if (rootNode == nullptr)
        {
            return 0;
        }
        int leftSum = sumOfNodes(rootNode->left);
        int rightSum = sumOfNodes(rootNode->right);

        return (leftSum + rightSum + rootNode->data);
    }
    bool searchByValue(TNode *rootNode, int key)
    {
        if (rootNode == nullptr)
        {
            return false;
        }
        if (rootNode->data == key)
        {
            return true;
        }
        else if (key < rootNode->data)
        {
            return searchByValue(rootNode->left, key);
        }
        else
        {
            return searchByValue(rootNode->right, key);
        }
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
    int countLeaves(TNode *rootNode)
    {
        if (rootNode == nullptr)
        {
            return 0;
        }
        if (rootNode->left == nullptr && rootNode->right == nullptr)
        {
            return 1;
        }
        return (countLeaves(rootNode->left) + countLeaves(rootNode->right));
    }

public:
    void insertValue(int val)
    {
        root = insertNode(val, root);
    }
    void build(vector<int> vec)
    {
        root = buildTree(vec);
    }
    void traversePreOrder()
    {
        cout << "\nPre Order: ";
        preOrder(root);
    }
    void traverseInOrder()
    {
        cout << "\nIn Order: ";
        inOrder(root);
    }
    void traversePostOrder()
    {
        cout << "\nPost Order: ";
        postOrder(root);
    }
    void calculateHeight()
    {
        cout << "\nTree Height: " << Height(root);
    }
    void calculateNodes()
    {
        cout << "\nTotal Nodes: " << count(root);
    }
    void calculateSum()
    {
        cout << "\nSum of data in Nodes: " << sumOfNodes(root);
    }
    bool searchNode(int value)
    {
        return searchByValue(root, value);
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
    void countLeaf()
    {
        cout << "\nEmpty Leaves: " << countLeaves(root);
    }
};

int main()
{
    vector<int> array = {5, 4, 6, 7, 3, 2, 8};
    Tree t1;
    // t1.insertValue(10);
    // t1.insertValue(8);
    // t1.insertValue(12);
    // t1.insertValue(6);
    // t1.insertValue(14);
    // t1.insertValue(9);
    // t1.insertValue(13);
    t1.build(array);
    t1.traversePreOrder();
    t1.traverseInOrder();
    t1.traversePostOrder();

    t1.calculateHeight();
    t1.calculateNodes();
    t1.calculateSum();

    cout << "\nElement " << (t1.searchNode(3) ? "Found" : "Not Found");
    t1.deleteNode(7);
    t1.traverseInOrder();

    t1.calculateMax();
    t1.calculateMin();
    t1.countLeaf();

    cout << endl;
    return 0;
}