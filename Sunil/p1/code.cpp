#include <iostream>
#include <vector>
#include <algorithm>
#include <numeric>

// Structure to represent a weighted edge
struct Edge {
    int u, v, weight;
    // Overload the < operator for sorting
    bool operator<(const Edge& other) const {
        return weight < other.weight;
    }
};

// Union-Find Data Structure (Disjoint Set Union) to detect cycles
struct DSU {
    std::vector<int> parent;
    DSU(int n) {
        parent.resize(n + 1);
        // Initialize each node as its own parent
        std::iota(parent.begin(), parent.end(), 0);
    }
    // Find the representative of the set containing i with path compression
    int find(int i) {
        if (parent[i] == i)
            return i;
        return parent[i] = find(parent[i]);
    }
    // Unite the sets containing i and j
    void unite(int i, int j) {
        int root_i = find(i);
        int root_j = find(j);
        if (root_i != root_j) {
            parent[root_i] = root_j;
        }
    }
};

int main() {
    // --- Hardcoded Sample Input (representing data from CSV) ---
    int num_nodes = 5; // Number of city infrastructure nodes
    int num_edges = 7; // Number of potential connections
    std::vector<Edge> edges = {
        {1, 2, 10}, {1, 3, 5},
        {2, 3, 8}, {2, 4, 12},
        {3, 4, 7}, {3, 5, 15},
        {4, 5, 9}
    };

    std::cout << "--- Optimizing Utility Network Layout ---" << std::endl;
    std::cout << "Nodes: " << num_nodes << ", Potential Connections: " << num_edges << std::endl;
    std::cout << "-----------------------------------------" << std::endl;

    // Step 1: Sort all edges in non-decreasing order of their weight
    std::sort(edges.begin(), edges.end());

    DSU dsu(num_nodes);
    long long mst_weight = 0;
    std::vector<Edge> mst_edges;

    // Step 2: Iterate through sorted edges and pick those that don't form a cycle
    for (const auto& edge : edges) {
        int u = edge.u;
        int v = edge.v;
        int weight = edge.weight;

        // Check if including this edge forms a cycle
        if (dsu.find(u) != dsu.find(v)) {
            // If not, include it in the MST and unite the sets
            dsu.unite(u, v);
            mst_weight += weight;
            mst_edges.push_back(edge);
        }
    }

    // Output Results
    std::cout << "Minimum Total Cost/Length: " << mst_weight << std::endl;
    std::cout << "Selected Connections for Optimal Layout:" << std::endl;
    for (const auto& edge : mst_edges) {
        std::cout << "Node " << edge.u << " -- Node " << edge.v << " (Cost: " << edge.weight << ")" << std::endl;
    }

    return 0;
}
