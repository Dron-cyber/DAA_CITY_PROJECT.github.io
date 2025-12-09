#include <bits/stdc++.h>
using namespace std;

// Problem: Optimizing Utility Network Layouts using Minimum Spanning Tree
// Technique: Kruskal's Algorithm + Union-Find (Disjoint Set Union)
// Input: n nodes, m edges, followed by m lines (u v w)
// Output: Minimum total cost + edges selected

struct DSU {
    vector<int> parent, rank;
    DSU(int n) {
        parent.resize(n+1);
        rank.resize(n+1, 0);
        for(int i = 1; i <= n; i++) parent[i] = i;
    }
    int find(int x) {
        if(parent[x] != x) parent[x] = find(parent[x]);
        return parent[x];
    }
    void unite(int a, int b) {
        a = find(a);
        b = find(b);
        if(a != b) {
            if(rank[a] < rank[b]) swap(a, b);
            parent[b] = a;
            if(rank[a] == rank[b]) rank[a]++;
        }
    }
};

int main() {
    ios::sync_with_stdio(false);
    cin.tie(nullptr);

    int n, m;
    cin >> n >> m;

    vector<array<int,3>> edges(m);
    for(int i = 0; i < m; i++) {
        cin >> edges[i][0] >> edges[i][1] >> edges[i][2];
    }

    // Sort edges by weight
    sort(edges.begin(), edges.end(), [](auto &a, auto &b) {
        return a[2] < b[2];
    });

    DSU dsu(n);
    int mst_cost = 0;
    vector<array<int,3>> mst_edges;

    for(auto &e : edges) {
        int u = e[0], v = e[1], w = e[2];
        if(dsu.find(u) != dsu.find(v)) {
            dsu.unite(u, v);
            mst_cost += w;
            mst_edges.push_back({u, v, w});
        }
    }

    cout << "Minimum Total Cost: " << mst_cost << "\n";
    cout << "Edges in MST:\n";
    for(auto &e : mst_edges) {
        cout << e[0] << " " << e[1] << " " << e[2] << "\n";
    }

    return 0;
}
