#include <bits/stdc++.h>
using namespace std;

// Problem: Garbage Truck Route Optimization
// Algorithm: TSP Heuristics (Nearest Neighbor)
// Input: n (number of bins), distance matrix (n x n)
// Output: Optimized route (approximate) and total distance

int main() {
    ios::sync_with_stdio(false);
    cin.tie(nullptr);

    int n;
    cin >> n;
    vector<vector<int>> dist(n, vector<int>(n));
    for(int i=0;i<n;i++){
        for(int j=0;j<n;j++){
            cin >> dist[i][j];
        }
    }

    vector<bool> visited(n, false);
    vector<int> route;
    int total_distance = 0;
    int current = 0; // start from depot (bin 0)
    route.push_back(current);
    visited[current] = true;

    for(int step=1; step<n; step++){
        int next_node = -1;
        int min_dist = INT_MAX;
        for(int j=0;j<n;j++){
            if(!visited[j] && dist[current][j] < min_dist){
                min_dist = dist[current][j];
                next_node = j;
            }
        }
        visited[next_node] = true;
        route.push_back(next_node);
        total_distance += min_dist;
        current = next_node;
    }

    total_distance += dist[current][0]; // return to depot
    route.push_back(0);

    cout << "Optimized route (approximate): ";
    for(int i=0;i<route.size();i++){
        if(i) cout << " -> ";
        cout << route[i];
    }
    cout << "\nTotal distance: " << total_distance << "\n";

    return 0;
}

