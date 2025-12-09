//this is based on sample data
#include <iostream>
#include <vector>
#include <string>
#include <sstream>
#include <algorithm>
#include <map>
#include <numeric>

using namespace std;

// --- 1. Union-Find (Disjoint Set Union) Structure ---
// Core data structure for connectivity check and cycle prevention (Cut and Cycle Properties)

class UnionFind {
private:
    vector<int> parent;
    int num_sets;

public:
    UnionFind(int n) : num_sets(n) {
        // Initialize parent array: each element is its own parent
        parent.resize(n);
        iota(parent.begin(), parent.end(), 0); 
    }

    // Find the representative (root) of the set containing element i (with Path Compression)
    int find(int i) {
        if (parent[i] == i)
            return i;
        // Path compression: set parent[i] directly to the root
        return parent[i] = find(parent[i]);
    }

    // Unite the sets containing elements i and j (Union by Rank/Size can be added for optimization)
    bool unite(int i, int j) {
        int root_i = find(i);
        int root_j = find(j);

        if (root_i != root_j) {
            // Merge the two sets
            parent[root_i] = root_j; 
            num_sets--;
            return true; // Union successful (no cycle formed)
        }
        return false; // Already in the same set (cycle formed)
    }

    int get_num_sets() const {
        return num_sets;
    }
};

// --- 2. Edge Structure ---
struct Edge {
    string from;
    string to;
    int cost;

    // Custom comparison function for sorting (Greedy choice)
    bool operator<(const Edge& other) const {
        return cost < other.cost;
    }
};

/**
 * @brief Parses the raw dataset string into a list of edges and extracts unique area names.
 * @param raw_data The string containing the space-separated dataset.
 * @param edges Output vector to store all potential road edges.
 * @param areas Output map to store unique area names and their assigned IDs.
 */
void parse_data(const string& raw_data, vector<Edge>& edges, map<string, int>& areas) {
    stringstream ss(raw_data);
    string line, area_from, area_to, cost_str;
    int id_counter = 0;

    // Skip the header line
    getline(ss, line); 

    // Read the data line by line
    while (ss >> area_from && ss >> area_to && ss >> cost_str) {
        
        // Remove underscores if they were used to replace spaces during data entry
        replace(area_from.begin(), area_from.end(), '_', ' ');
        replace(area_to.begin(), area_to.end(), '_', ' ');

        int cost = stoi(cost_str);
        
        edges.push_back({area_from, area_to, cost});

        // Assign unique ID to each area
        if (areas.find(area_from) == areas.end()) {
            areas[area_from] = id_counter++;
        }
        if (areas.find(area_to) == areas.end()) {
            areas[area_to] = id_counter++;
        }
    }
}

// --- 3. Kruskal's Algorithm Implementation ---

/**
 * @brief Finds the Minimum Spanning Tree (MST) using Kruskal's algorithm.
 * @param edges All potential edges (roads) in the graph.
 * @param areas Map of area names to their Union-Find IDs.
 * @return A vector of edges that form the MST.
 */
vector<Edge> kruskal_mst(vector<Edge>& edges, const map<string, int>& areas) {
    // 1. Sort edges by cost (ascending)
    sort(edges.begin(), edges.end()); 

    int V = areas.size();
    UnionFind uf(V);
    vector<Edge> mst_edges;
    long long total_cost = 0;

    // 2. Iterate through sorted edges (Greedy Choice)
    for (const auto& edge : edges) {
        // Map area names to their Union-Find IDs
        int u = areas.at(edge.from);
        int v = areas.at(edge.to);

        // 3. Check for cycle using Union-Find (Connectivity Check)
        if (uf.unite(u, v)) {
            // No cycle formed: add the edge to the MST
            mst_edges.push_back(edge);
            total_cost += edge.cost;
            
            // Stop when V-1 edges are found
            if (mst_edges.size() == V - 1) {
                break;
            }
        }
    }

    if (mst_edges.size() != V - 1 && V > 0) {
        cout << "\nWarning: The graph is disconnected. MST could not connect all " << V << " areas.\n";
    }

    cout << "\n--- Kruskal's MST Result ---\n";
    cout << "Total Minimum Construction Cost: " << total_cost << " Crores\n";
    cout << "--------------------------------\n";

    return mst_edges;
}


int main() {
    // Dataset provided by the user (using underscores for areas with spaces for simple parsing)
    const string dataset = 
        "area_from area_to cost_in_crores\n"
        "Central_Market Railway_Station 4\n"
        "Central_Market Residential_Block_A 6\n"
        "Central_Market Old_City 5\n"
        "Central_Market IT_Park 12\n"
        "Central_Market Airport 20\n"
        "Railway_Station Residential_Block_A 3\n"
        "Railway_Station Old_City 4\n"
        "Railway_Station Industrial_Area 15\n"
        "Residential_Block_A Residential_Block_B 2\n"
        "Residential_Block_A Old_City 5\n"
        "Residential_Block_A IT_Park 10\n"
        "Residential_Block_B Residential_Block_C 3\n"
        "Residential_Block_B IT_Park 8\n"
        "Residential_Block_B Commercial_Hub 7\n"
        "Residential_Block_C Old_City 6\n"
        "Residential_Block_C Slum_Area 5\n"
        "Old_City Slum_Area 2\n"
        "Old_City Industrial_Area 9\n"
        "Old_City Commercial_Hub 11\n"
        "Slum_Area Industrial_Area 6\n"
        "Slum_Area Airport 18\n"
        "IT_Park Commercial_Hub 4\n"
        "IT_Park Airport 14\n"
        "IT_Park Industrial_Area 12\n"
        "Commercial_Hub Industrial_Area 7\n"
        "Commercial_Hub Airport 16\n";

    vector<Edge> all_potential_roads;
    map<string, int> area_to_id;

    // 1. Parse Data and identify all unique areas (nodes)
    parse_data(dataset, all_potential_roads, area_to_id);

    // 2. Run Kruskal's Algorithm
    vector<Edge> mst_roads = kruskal_mst(all_potential_roads, area_to_id);

    // 3. Report the selected roads (The new network design)
    cout << "\n--- Recommended Road Network Edges ---\n";
    cout << "To Achieve Minimum Cost:\n";
    for (const auto& edge : mst_roads) {
        cout << "- Connect **" << edge.from << "** to **" << edge.to 
             << "** (Cost: " << edge.cost << " Cr)\n";
    }
    cout << "------------------------------------\n";

    return 0;
}
