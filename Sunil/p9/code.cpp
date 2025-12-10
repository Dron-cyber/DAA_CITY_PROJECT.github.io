
#include <iostream>
#include <vector>
#include <queue>
#include <climits>
using namespace std;

// BFS for Edmonds-Karp to find augmenting path
bool bfs(vector<vector<int>>& rGraph, int s, int t, vector<int>& parent) {
    int n = rGraph.size();
    vector<bool> visited(n, false);
    queue<int> q;

    q.push(s);
    visited[s] = true;
    parent[s] = -1;

    while (!q.empty()) {
        int u = q.front();
        q.pop();

        for (int v = 0; v < n; v++) {
            if (!visited[v] && rGraph[u][v] > 0) {
                q.push(v);
                parent[v] = u;
                visited[v] = true;

                if (v == t) return true;
            }
        }
    }
    return false;
}

// Edmonds–Karp Max-Flow
int maxFlow(vector<vector<int>>& graph, int s, int t) {
    int n = graph.size();
    vector<vector<int>> rGraph = graph;  
    vector<int> parent(n);
    int max_flow = 0;

    while (bfs(rGraph, s, t, parent)) {
        int path_flow = INT_MAX;

        // Find minimum residual capacity along BFS path
        for (int v = t; v != s; v = parent[v]) {
            int u = parent[v];
            path_flow = min(path_flow, rGraph[u][v]);
        }

        // Update residual graph
        for (int v = t; v != s; v = parent[v]) {
            int u = parent[v];
            rGraph[u][v] -= path_flow;
            rGraph[v][u] += path_flow;
        }

        max_flow += path_flow;
    }

    return max_flow;
}

// BFS for shortest path to guide evacuation
vector<int> shortestPath(vector<vector<int>>& graph, int src, int dst) {
    int n = graph.size();
    vector<int> parent(n, -1);
    vector<bool> visited(n, false);
    queue<int> q;

    q.push(src);
    visited[src] = true;

    while (!q.empty()) {
        int u = q.front(); q.pop();
        if (u == dst) break;

        for (int v = 0; v < n; v++) {
            if (graph[u][v] > 0 && !visited[v]) {
                visited[v] = true;
                parent[v] = u;
                q.push(v);
            }
        }
    }

    vector<int> path;
    if (!visited[dst]) return path; // no path

    for (int v = dst; v != -1; v = parent[v])
        path.push_back(v);

    reverse(path.begin(), path.end());
    return path;
}

int main() {
    int V, E, s, t;
    cin >> V >> E;   // number of nodes and edges

    vector<vector<int>> graph(V, vector<int>(V, 0));

    // Input: u v capacity
    for (int i = 0; i < E; i++) {
        int u, v, c;
        cin >> u >> v >> c;
        graph[u][v] = c;
    }

    cin >> s >> t;   // source (danger zone) and sink (safe zone)

    // Shortest path for guidance
    vector<int> path = shortestPath(graph, s, t);

    // Max-flow for total evacuation capacity
    int maxflow = maxFlow(graph, s, t);

    // Output
    cout << "Shortest Evacuation Path: ";
    if (path.empty()) cout << "No Path Exists\n";
    else {
        for (int p : path) cout << p << " ";
        cout << "\n";
    }

    cout << "Maximum Evacuation Flow: " << maxflow << "\n";
}
