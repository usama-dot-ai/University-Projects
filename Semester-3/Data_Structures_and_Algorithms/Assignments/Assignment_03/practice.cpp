#include <iostream>
#include <iomanip>
#include <string>
using namespace std;

struct Product
{
    int id;
    string name;
    int quantity;
    double price;
};

class Tnode
{
public:
    Product data;
    Tnode *left;
    Tnode *right;
};

class Tree
{
private:
    Tnode *root;
    Tnode *createNode(Product p)
    {
        Tnode *newNode = new Tnode();
        newNode->data = p;
        newNode->left = nullptr;
        newNode->right = nullptr;
        return newNode;
    }
    Tnode *insertNode(Tnode *node, Product p)
    {
        if (node == nullptr)
        {
            return createNode(p);
        }
        else if (p.id < node->data.id)
        {
            node->left = insertNode(node->left, p);
        }
        else if (p.id > node->data.id)
        {
            node->right = insertNode(node->right, p);
        }
        else
        {
            cout << "Product Already Exists." << endl;
        }
        return node;
    }
    Tnode *searchNode(Tnode *node, int id)
    {
        if (node == nullptr)
        {
            return nullptr;
        }
        else if (id == node->data.id)
        {
            return node;
        }
        else if (id < node->data.id)
        {
            return searchNode(node->left, id);
        }
        else
        {
            return searchNode(node->right, id);
        }
    }
    Tnode *findSuccessor(Tnode *node)
    {
        while (node != nullptr && node->left != nullptr)
        {
            node = node->left;
        }
        return node;
    }
    Tnode *deleteNode(Tnode *node, int id)
    {
        if (node == nullptr)
        {
            return nullptr;
        }
        else if (id < node->data.id)
        {
            node->left = deleteNode(node->left, id);
        }
        else if (id > node->data.id)
        {
            node->right = deleteNode(node->right, id);
        }
        else
        {
            if (node->left == nullptr)
            {
                Tnode *temp = node->right;
                delete node;
                return temp;
            }
            else if (node->right == nullptr)
            {
                Tnode *temp = node->left;
                delete node;
                return temp;
            }
            else
            {
                Tnode *successor = findSuccessor(node->right);
                node->data = successor->data;
                node->right = deleteNode(node->right, successor->data.id);
            }
        }
        return node;
    }
    void inOrder(Tnode *node)
    {
        if (node == nullptr)
        {
            return;
        }
        inOrder(node->left);
        cout << "ID: " << node->data.id << endl;
        cout << "Name: " << node->data.name << endl;
        cout << "Quantity: " << node->data.quantity << endl;
        cout << "Price: " << fixed << setprecision(2) << node->data.price << endl;
        cout << "-------------------" << endl;
        inOrder(node->right);
    }
    void countProducts(Tnode *node, int &total, int &outStock)
    {
        if (node == nullptr)
        {
            return;
        }
        total = total + 1;
        if (node->data.quantity == 0)
        {
            outStock = outStock + 1;
        }
        countProducts(node->left, total, outStock);
        countProducts(node->right, total, outStock);
    }

public:
    Tree() : root(nullptr) {}
    void addProduct()
    {
        Product details;

        cout << "Enter Product ID: ";
        cin >> details.id;
        cin.ignore();
        cout << "Enter Product Name: ";
        getline(cin, details.name);
        cout << "Enter Product Quantity: ";
        cin >> details.quantity;
        cout << "Enter Product Price: ";
        cin >> details.price;

        root = insertNode(root, details);
        cout << "Product Added Successfully." << endl;
    }
    void searchProduct()
    {
        int id;
        cout << "Enter Product ID: ";
        cin >> id;
        Tnode *foundProduct = searchNode(root, id);
        if (foundProduct == nullptr)
        {
            cout << "Product does not Exist." << endl;
        }
        else
        {
            cout << "Product ID: " << foundProduct->data.id << endl;
            cout << "Product Name: " << foundProduct->data.name << endl;
            cout << "Product Quantity: " << foundProduct->data.quantity << endl;
            cout << "Product Price: " << fixed << setprecision(2) << foundProduct->data.price << endl;
        }
    }
    void deleteProduct()
    {
        int id;
        cout << "Enter Product ID: ";
        cin >> id;

        Tnode *foundProduct = searchNode(root, id);

        if (foundProduct == nullptr)
        {
            cout << "Product does not exist." << endl;
        }
        else
        {
            root = deleteNode(root, id);
            cout << "Product Deleted Successfully." << endl;
        }
    }
    void traverseInorder()
    {
        cout << "===Inventory===" << endl;
        inOrder(root);
    }
    void updateQuantity()
    {
        int id, newQuantity;

        cout << "Enter Product ID: ";
        cin >> id;

        Tnode *node = searchNode(root, id);

        if (node == nullptr)
        {
            cout << "Product " << id << " Not Found." << endl;
        }
        else
        {
            cout << "Enter New Quantity: " << endl;
            cin >> newQuantity;

            node->data.quantity = newQuantity;
            cout << "Quantity Updated Successfully." << endl;
        }
    }
    void displayCounts()
    {
        int total = 0;
        int outStock = 0;
        countProducts(root, total, outStock);
        cout << "Total Products: " << total << endl;
        cout << "Out of Stock: " << outStock << endl;
    }
};

int main()
{
    Tree inventory;
    int choice;

    while (true)
    {
        cout << "\n=== Product Inventory Management ===" << endl;
        cout << "1. Add Product" << endl;
        cout << "2. Search Product" << endl;
        cout << "3. Delete Product" << endl;
        cout << "4. Update Quantity" << endl;
        cout << "5. Display Inventory" << endl;
        cout << "6. Display Counts (Total and Out of Stock)" << endl;
        cout << "7. Exit" << endl;
        cout << "Enter your choice: ";
        cin >> choice;

        switch (choice)
        {
        case 1:
            inventory.addProduct();
            break;
        case 2:
            inventory.searchProduct();
            break;
        case 3:
            inventory.deleteProduct();
            break;
        case 4:
            inventory.updateQuantity();
            break;
        case 5:
            inventory.traverseInorder();
            break;
        case 6:
            inventory.displayCounts();
            break;
        case 7:
            cout << "Exiting..." << endl;
            return 0;
        default:
            cout << "Invalid choice. Please try again." << endl;
        }
    }

    return 0;
}
