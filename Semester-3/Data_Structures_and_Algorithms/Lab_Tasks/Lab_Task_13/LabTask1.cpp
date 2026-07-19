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
    void shortestPath()
    {
        int path0134 = adjMatrix[0][1] + adjMatrix[1][3] + adjMatrix[3][4];
        int path0234 = adjMatrix[0][2] + adjMatrix[2][3] + adjMatrix[3][4];

        int shortest = (path0234 < path0134) ? path0234 : path0134;

        cout << "Shortest Path 0->4: " << shortest << " weight" << endl;
    }
};

int main()
{
    Graph g;
    g.printMatrix();
    cout << endl;

    g.addEdge(0, 1, 5);
    g.addEdge(0, 2, 1);
    g.addEdge(1, 3, 1);
    g.addEdge(2, 3, 3);
    g.addEdge(3, 4, 6);

    g.printMatrix();
    cout << endl;
    g.shortestPath();
    return 0;
}