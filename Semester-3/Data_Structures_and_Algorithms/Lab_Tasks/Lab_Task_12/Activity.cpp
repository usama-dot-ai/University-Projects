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
        return node;
    }

    // Helper function to find inorder successor (leftmost in right subtree)
    Tnode *inOrderSuccessor(Tnode *rootNode)
    {
        while (rootNode != nullptr && rootNode->left != nullptr) // Traverse to leftmost node
        {
            rootNode = rootNode->left;
        }
        return rootNode; // Return the successor node
    }

    Tnode *deleteNode(Tnode *rootNode, int val)
    {
        if (rootNode == nullptr) // value not found
        {
            return nullptr;
        }
        if (val < rootNode->data) // If value is smaller, go left
        {
            rootNode->left = deleteNode(rootNode->left, val);
        }
        else if (val > rootNode->data) // If value is larger, go right
        {
            rootNode->right = deleteNode(rootNode->right, val);
        }
        else // Value found, handle deletion
        {
            if (rootNode->left == nullptr) // No left child: return right subtree
            {
                Tnode *temp = rootNode->right;
                delete rootNode;
                return temp;
            }
            else if (rootNode->right == nullptr) // No right child: return left subtree
            {
                Tnode *temp = rootNode->left;
                delete rootNode;
                return temp;
            }
            else // Two children: replace with successor and delete successor
            {
                Tnode *ios = inOrderSuccessor(rootNode->right);           // Find successor in right subtree
                rootNode->data = ios->data;                               // Replace value
                rootNode->right = deleteNode(rootNode->right, ios->data); // Delete successor from right
            }
        }

        // Update height after deletion
        rootNode->height = 1 + max(getHeight(rootNode->left), getHeight(rootNode->right));

        // Check balance factor and perform rotations if needed
        int bal = getBalanceFactor(rootNode);

        if (bal > 1 && getBalanceFactor(rootNode->left) >= 0)
        {
            return LL_Rotation(rootNode);
        }
        if (bal > 1 && getBalanceFactor(rootNode->left) < 0)
        {
            return LR_Rotation(rootNode);
        }
        if (bal < -1 && getBalanceFactor(rootNode->right) <= 0)
        {
            return RR_Rotation(rootNode);
        }
        if (bal < -1 && getBalanceFactor(rootNode->right) > 0)
        {
            return RL_Rotation(rootNode);
        }

        return rootNode; // Return updated subtree root
    }

    void inorderTraversal(Tnode *node)
    {
        if (node != nullptr)
        {
            inorderTraversal(node->left);
            cout << node->data << " ";
            inorderTraversal(node->right);
        }
    }

public:
    AVLTree() : root(nullptr) {}

    void insert(int val)
    {
        root = insertNode(root, val);
    }

    void deleteNode(int val)
    {
        root = deleteNode(root, val);
    }

    void inorder()
    {
        inorderTraversal(root);
        cout << endl;
    }
};

int main()
{
    AVLTree tree;

    // Insert 5 values
    tree.insert(10);
    tree.insert(20);
    tree.insert(30);
    tree.insert(40);
    tree.insert(50);

    cout << "Inorder traversal before deletion: ";
    tree.inorder();

    // Delete a node (e.g., 30)
    tree.deleteNode(30);

    cout << "Inorder traversal after deletion: ";
    tree.inorder();

    return 0;
}
