// Spare Graphs
#include <iostream>
using namespace std;

#define n 5

class Node
{
public:
    int data;
    Node *next;
};

class Graph
{
private:
    Node *head[n];

public:
    Graph()
    {

        for (int i = 0; i < n; i++)
        {
            head[i] = nullptr;
        }
    }
    Node *createNode(int v)
    {
        Node *newNode = new Node();
        newNode->next = nullptr;
        newNode->data = v;

        return newNode;
    }
    void addEdge(int u, int v)
    {
        Node *newNode = createNode(v);
        newNode->next = head[u];
        head[u] = newNode;

        newNode = createNode(u);
        newNode->next = head[v];
        head[v] = newNode;
    }
};

int main()
{
    Graph g;

    return 0;
}