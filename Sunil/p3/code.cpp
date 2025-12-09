
#include <bits/stdc++.h>
using namespace std;

// Problem: Analyzing Connected Green Zones
// Algorithm: BFS / DFS (Connected Components)
// Input: n m (number of parks, number of paths), then m lines of edges u v
// Output: Number of distinct green zones (connected components)

void dfs(int u, vector<vector<int>> &adj, vector<bool> &visited) {
    visited[u] = true;
    for(int v : adj[u]){
        if(!visited[v]) dfs(v, adj, visited);
    }
}

int main() {
    ios::sync_with_stdio(false);
    cin.tie(nullptr);

    int n, m; // n = number of parks, m = number of paths
    cin >> n >> m;

    vector<vector<int>> adj(n+1); // adjacency list
    for(int i=0; i<m; i++){
        int u, v;
        cin >> u >> v;
        adj[u].push_back(v);
        adj[v].push_back(u); // undirected graph
    }

    vector<bool> visited(n+1, false);
    int components = 0;

    for(int i=1; i<=n; i++){
        if(!visited[i]){
            components++;
            dfs(i, adj, visited);
        }
    }

    cout << "Number of distinct green zones: " << components << "\n";

    return 0;
}
