#include <iostream>
#include <vector>
#include <queue>
#include <limits>
using namespace std;

// Structure to store edges
struct Edge {
    int to;
    int weight;
};

// Dijkstra function
vector<int> dijkstra(int start, int V, vector<vector<Edge>>& graph) {
    vector<int> dist(V + 1, numeric_limits<int>::max());
    dist[start] = 0;

    priority_queue<pair<int, int>, vector<pair<int, int>>, greater<pair<int, int>>> pq;
    pq.push({0, start}); // {distance, node}

    while (!pq.empty()) {
        auto [currentDist, node] = pq.top();
        pq.pop();

        if (currentDist > dist[node]) continue;

        for (auto& edge : graph[node]) {
            int next = edge.to;
            int cost = edge.weight;

            if (dist[node] + cost < dist[next]) {
                dist[next] = dist[node] + cost;
                pq.push({dist[next], next});
            }
        }
    }
    return dist;
}

int main() {
    int V, E;
    cout << "\n=== Emergency Vehicle Rapid Response Routing ===\n";
    cout << "Enter number of intersections (nodes): ";
    cin >> V;

    cout << "Enter number of roads (edges): ";
    cin >> E;

    vector<vector<Edge>> graph(V + 1);

    cout << "\nEnter road connections in format:\n";
    cout << "From  To  TravelTime\n";

    for (int i = 0; i < E; i++) {
        int u, v, w;
        cin >> u >> v >> w;

        graph[u].push_back({v, w});
        graph[v].push_back({u, w}); // Bidirectional roads
    }

    int start;
    cout << "\nEnter ambulance/fire-truck starting node: ";
    cin >> start;

    vector<int> result = dijkstra(start, V, graph);

    cout << "\n--- Fastest Travel Time to All Locations ---\n";
    for (int i = 1; i <= V; i++) {
        cout << "Node " << i << " : ";
        if (result[i] == numeric_limits<int>::max())
            cout << "UNREACHABLE\n";
        else
            cout << result[i] << " units\n";
    }

    return 0;
}

