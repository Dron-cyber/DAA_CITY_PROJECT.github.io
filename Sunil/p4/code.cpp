
#include <bits/stdc++.h>
using namespace std;

// Problem: Smart Power Grid Load Balancing
// Algorithm: Greedy & Max-Flow (Edmonds-Karp for demonstration)
// Input: n m (nodes, edges), then m lines u v capacity, then source and sink
// Output: Maximum electricity flow from source to sink

int bfs(int s, int t, vector<vector<int>> &adj, vector<vector<int>> &cap, vector<int> &parent) {
    fill(parent.begin(), parent.end(), -1);
    parent[s] = -2;
    queue<pair<int,int>> q;
    q.push({s, INT_MAX});

    while(!q.empty()) {
        int cur = q.front().first;
        int flow = q.front().second;
        q.pop();
        for(int next : adj[cur]){
            if(parent[next] == -1 && cap[cur][next] > 0){
                parent[next] = cur;
                int new_flow = min(flow, cap[cur][next]);
                if(next == t) return new_flow;
                q.push({next, new_flow});
            }
        }
    }
    return 0;
}

int main() {
    ios::sync_with_stdio(false);
    cin.tie(nullptr);

    int n, m; // substations and transmission lines
    cin >> n >> m;

    vector<vector<int>> cap(n+1, vector<int>(n+1, 0));
    vector<vector<int>> adj(n+1);

    for(int i=0;i<m;i++){
        int u,v,w;
        cin >> u >> v >> w;
        adj[u].push_back(v);
        adj[v].push_back(u);
        cap[u][v] = w;
    }

    int source, sink;
    string tmp;
    cin.ignore();
    getline(cin, tmp);
    source = stoi(tmp.substr(tmp.find(":")+1));
    getline(cin, tmp);
    sink = stoi(tmp.substr(tmp.find(":")+1));

    int flow = 0;
    vector<int> parent(n+1);
    int new_flow;

    while(new_flow = bfs(source, sink, adj, cap, parent)){
        flow += new_flow;
        int cur = sink;
        while(cur != source){
            int prev = parent[cur];
            cap[prev][cur] -= new_flow;
            cap[cur][prev] += new_flow;
            cur = prev;
        }
    }

    cout << "Maximum electricity flow from substation " << source << " to " << sink << ": " << flow << "\n";

    return 0;
}
