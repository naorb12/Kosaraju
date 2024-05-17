#include "DirectedGraph.h"

int main() 
{
    int numVertices, numEdges, u, v;
    std::cout << "Enter the number of vertices: ";
    std::cin >> numVertices;
    std::cout << "Enter the number of edges: ";
    std::cin >> numEdges;

    if (numEdges > numVertices * (numVertices - 1))
    {
        std::cerr << "Error: Number of edges exceeds the maximum possible (" << numVertices * (numVertices - 1) << ")." << std::endl;
        exit(1);
    }

    DirectedGraph g(numVertices);

    std::cout << "Enter the edges in the format 'u v':" << std::endl;
    for (int i = 0; i < numEdges; i++) 
    {
        std::cin >> u >> v;
        while (!g.AddEdge(u, v))
        {
            std::cerr << "Invalid edge or edge already exists: " << u << " -> " << v << std::endl;
            std::cin >> u >> v;
        }
    }

    g.Kosaraju();

    std::vector<int> finalList = g.DFS();
    for (int i = 0; i < finalList.size(); i++)
    {
        std::cout << finalList[i] + 1;
    }


    // For demonstration, we can use functions here or just conclude the program.
    return 0;
}