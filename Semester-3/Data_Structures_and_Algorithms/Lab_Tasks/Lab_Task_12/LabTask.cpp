#include <iostream>
using namespace std;

class Tnode
{
public:
    int data, height;
    Tnode *left = nullptr;
    Tnode *right = nullptr;
};
class AVLTree
{
private:
    Tnode *root;

public:
    AVLTree() : root(nullptr) {}
    Tnode *createNode(int val)
    {
        Tnode *newNode = new Tnode();
        newNode->data = val;
        newNode->right = nullptr;
        newNode->left = nullptr;
        newNode->height = 1;
        return newNode;
    }
    int getHeight(Tnode *node)
    {
        if (node == nullptr)
        {
            return 0;
        }
        return node->height;
    }
    int getBalanceFactor(Tnode *node)
    {
        if (node == nullptr)
        {
            return 0;
        }
        return (getHeight(node->left) - getHeight(node->right));
    }
    Tnode *LL_Rotation(Tnode *L1)
    {
        Tnode *L2 = L1->left;
        Tnode *L3 = L2->right;

        L2->right = L1;
        L1->left = L3;

        L1->height = max(getHeight(L1->left), getHeight(L1->right)) + 1;
        L2->height = max(getHeight(L2->left), getHeight(L2->right)) + 1;

        return L2;
    }
    Tnode *RR_Rotation(Tnode *R1)
    {
        Tnode *R2 = R1->right;
        Tnode *R3 = R2->left;

        R2->left = R1;
        R1->right = R3;

        R1->height = max(getHeight(R1->left), getHeight(R1->right)) + 1;
        R2->height = max(getHeight(R2->left), getHeight(R2->right)) + 1;

        return R2;
    }
    Tnode *LR_Rotation(Tnode *L1)
    {
        L1->left = RR_Rotation(L1->left);
        return LL_Rotation(L1);
    }
    Tnode *RL_Rotation(Tnode *R1)
    {
        R1->right = LL_Rotation(R1->right);
        return RR_Rotation(R1);
    }
    Tnode *insertNode(Tnode *node, int val)
    {
        if (node == nullptr)
        {
            return createNode(val);
        }
        else if (val < node->data)
        {
            node->left = insertNode(node->left, val);
        }
        else if (val > node->data)
        {
            node->right = insertNode(node->right, val);
        }
        else
        {
            return node;
        }

        node->height = 1 + max(getHeight(node->left), getHeight(node->right));
        int balFactor = getBalanceFactor(node);

        if (balFactor > 1 && val < node->left->data) // LL case
        {
            return LL_Rotation(node);
        }
        if (balFactor < -1 && val > node->right->data) // RR case
        {
            return RR_Rotation(node);
        }
        if (balFactor > 1 && val > node->left->data) // LR case
        {
            return LR_Rotation(node);
        }
        if (balFactor < -1 && val < node->right->data) // RL case
        {
            return RL_Rotation(node);
        }
    }
};