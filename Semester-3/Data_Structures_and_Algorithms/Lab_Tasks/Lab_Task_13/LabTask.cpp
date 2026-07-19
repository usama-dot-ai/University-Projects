// Dense Graphs
#include <iostream>
using namespace std;

#define n 5

class Graph
{
private:
    int adjMatrix[n][n];

public:
    Graph()
    {
        for (int i = 0; i < n; i++)
        {
            for (int j = 0; j < n; j++)
            {
                adjMatrix[i][j] = 0;
            }
        }
    }
    void printMatrix()
    {
        for (int i = 0; i < n; i++)
        {
            for (int j = 0; j < n; j++)
            {
                cout << adjMatrix[i][j] << " ";
            }
            cout << endl;
        }
    }
    void addEdge(int u, int v, int weight)
    {
        adjMatrix[u][v] = weight;
        adjMatrix[v][u] = weight;
    }
};

int main()
{
    Graph g;
    g.printMatrix();
    cout << endl;

    g.addEdge(0, 1, 1);
    g.addEdge(0, 2, 2);
    g.addEdge(1, 3, 3);
    g.addEdge(2, 4, 4);
    g.addEdge(3, 4, 5);

    g.printMatrix();
    return 0;
}