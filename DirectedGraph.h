#ifndef _DirectedGraph

#include <stdlib.h>
#include <iostream>
#include <list>
#include <vector>


class DirectedGraph {
private:

    enum Color { WHITE, GRAY, BLACK }; // Define the colors as enum types

    int numVertices = 0; // Number of vertices in the graph
    std::vector<std::list<int>> adjList; // Adjacency list

    void DFSUtil(int v, std::vector<Color>& color, std::vector<int>& result) {
        // Gray (being visited)
        color[v] = GRAY;

        // Recur for all the vertices adjacent to this vertex
        for (int u : adjList[v]) {
            if (color[u] == WHITE) { // White (not visited)
                DFSUtil(u, color, result);
            }
        }

        // Black (finished)
        color[v] = BLACK;
        result.push_back(v); // Push current vertex to result as it finishes
    }

    void DFSUtilSecond(int v, std::vector<Color>& color, std::vector<int>& component, std::vector<int>& componentLookup, int currentSCCId) 
    {
        color[v] = GRAY;
        component.push_back(v);
        componentLookup[v] = currentSCCId;  // Assign the vertex to the current SCC ID
        for (int u : adjList[v]) {
            if (color[u] == WHITE) {
                DFSUtilSecond(u, color, component, componentLookup, currentSCCId);
            }
        }
        color[v] = BLACK;
    }

    //void DFSUtilSecond(int v, std::vector<Color>& color, std::vector<int>& component, int& edgeCount)
    //{
    //    color[v] = GRAY;
    //    component.push_back(v);
    //    for (int u : adjList[v]) {
    //        if (color[u] == WHITE) {
    //            DFSUtilSecond(u, color, component, edgeCount);
    //        }
    //        if (color[u] != BLACK) { // Count edges within the SCC that are not to a fully processed node
    //            edgeCount++;
    //        }
    //    }
    //    color[v] = BLACK;
    //}

public:
    // Constructor to initialize an empty graph with n vertices
    DirectedGraph(int n) {
        MakeEmptyGraph(n);
    }

    // Initialize an empty graph with n vertices
    void MakeEmptyGraph(int n) {
        numVertices = n;
        adjList.clear();
        adjList.resize(n);
    }

    // Add an edge from u to v
    bool AddEdge(int u, int v) {
        if (u >= 1 && u <= numVertices && v >= 1 && v <= numVertices && u != v) {
            // Avoid adding the same edge twice
            if (!IsAdjacent(u-1, v-1)) {
                adjList[u-1].push_back(v-1);
                return true;
            }
        }
        return false;
    }

    // Remove an edge from u to v
    void RemoveEdge(int u, int v) {
        if (u >= 0 && u < numVertices) {
            adjList[u].remove(v);
        }
    }

    // Check if there is an edge from u to v
    bool IsAdjacent(int u, int v) {
        if (u >= 0 && u < numVertices) 
        {
            for (int adj : adjList[u]) 
            {
                if (adj == v) {
                    return true;
                }
            }
        }
        return false;
    }

    // Get the adjacency list of vertex u
    std::list<int> GetAdList(int u) {
        if (u >= 0 && u < numVertices) {
            return adjList[u];
        }
        else {
            return std::list<int>(); // return an empty list
        }
    }

    // Perform DFS starting from vertex v
    std::vector<int> DFS() {
        std::vector<Color> color(numVertices, WHITE); // Initialize color of all vertices as WHITE
        std::vector<int> result;

        // Call the recursive helper function to store DFS traversal
        for (int i = 0; i < numVertices; i++) {
            if (color[i] == WHITE) {
                DFSUtil(i, color, result);
            }
        }

        // Since result has vertices in finish time order, reverse it to get the correct order
        std::reverse(result.begin(), result.end());
        return result;
    }

    //std::vector<int> DFSsecond(const std::vector<int>& finishOrder) 
    //{
    //    std::vector<Color> color(numVertices, WHITE);
    //    std::vector<int> componentSizes;
    //    for (int v : finishOrder) 
    //    {
    //        if (color[v] == WHITE) 
    //        {
    //            std::vector<int> component;
    //            int edgeCount = 0;
    //            DFSUtilSecond(v, color, component, edgeCount);
    //            componentSizes.push_back(component.size());  // Here we use size, you can change as needed
    //        }
    //    }
    //    return componentSizes;
    //}

    void Kosaraju() 
    {
        std::vector<int> finishOrder = DFS();
        DirectedGraph transposed = TransposeGraph();
        std::vector<Color> color(numVertices, WHITE);
        std::vector<std::vector<int>> sccs;
        std::vector<int> componentLookup(numVertices, -1);
        int currentSCCId = 0;

        for (int v : finishOrder) {
            if (color[v] == WHITE) {
                std::vector<int> component;
                DFSUtilSecond(v, color, component, componentLookup, currentSCCId);
                sccs.push_back(component);
                currentSCCId++;  // Increment the SCC ID for the next component
            }
        }

        // Count edges between different SCCs
        int interComponentEdges = 0;
        for (int v = 0; v < numVertices; v++) {
            for (int u : adjList[v]) {
                if (componentLookup[v] != -1 && componentLookup[u] != -1 && componentLookup[v] != componentLookup[u]) {
                    interComponentEdges++;
                }
            }
        }

        std::cout << "Total strongly connected components: " << sccs.size() << std::endl;
        std::cout << "Total edges between different SCCs: " << interComponentEdges << std::endl;
    }

    // Transpose the graph
    DirectedGraph TransposeGraph() 
    {
        DirectedGraph transposed(numVertices);
        for (int u = 0; u < numVertices; ++u) {
            for (int v : adjList[u]) {
                transposed.AddEdge(v, u);
            }
        }
        return transposed;
    }
};


#endif // !