
#include <iostream>
#include <vector>
#include <cmath>
#include <limits>
#include <algorithm>
using namespace std;

struct Delivery {
    int id;
    double x, y;     // Coordinates
    int demand;      // Package weight
};

// Euclidean distance
double dist(double x1, double y1, double x2, double y2) {
    return sqrt(pow(x1 - x2, 2) + pow(y1 - y2, 2));
}

// Greedy TSP (Nearest Neighbor)
vector<int> tspNearestNeighbor(const vector<Delivery>& cluster) {
    int n = cluster.size();
    vector<bool> visited(n, false);
    vector<int> route;
    
    int current = 0; 
    visited[0] = true;
    route.push_back(cluster[0].id);

    for(int i = 1; i < n; i++) {
        int next = -1;
        double best = 1e9;

        for(int j = 0; j < n; j++) {
            if(!visited[j]) {
                double d = dist(cluster[current].x, cluster[current].y,
                                cluster[j].x, cluster[j].y);
                if(d < best) {
                    best = d;
                    next = j;
                }
            }
        }

        visited[next] = true;
        route.push_back(cluster[next].id);
        current = next;
    }

    return route;
}

// Simple clustering by capacity (Greedy)
vector<vector<Delivery>> clusterByCapacity(vector<Delivery> &deliveries, int capacity) {
    vector<vector<Delivery>> clusters;
    vector<Delivery> currentCluster;
    int currentLoad = 0;

    for (auto &d : deliveries) {
        if (currentLoad + d.demand <= capacity) {
            currentCluster.push_back(d);
            currentLoad += d.demand;
        } else {
            clusters.push_back(currentCluster);
            currentCluster = { d };
            currentLoad = d.demand;
        }
    }
    if (!currentCluster.empty())
        clusters.push_back(currentCluster);

    return clusters;
}

int main() {
    int n, capacity;
    cout << "Enter number of deliveries: ";
    cin >> n;

    cout << "Enter vehicle capacity: ";
    cin >> capacity;

    vector<Delivery> deliveries(n);

    cout << "Enter each delivery (x y demand):\n";
    for(int i = 0; i < n; i++) {
        deliveries[i].id = i + 1;
        cin >> deliveries[i].x >> deliveries[i].y >> deliveries[i].demand;
    }

    auto clusters = clusterByCapacity(deliveries, capacity);

    cout << "\n--- Clustered Routes (each = truck) ---\n";

    for(int i = 0; i < clusters.size(); i++) {
        auto route = tspNearestNeighbor(clusters[i]);
        cout << "Truck " << i+1 << " route: ";

        for(int id : route)
            cout << id << " ";
        cout << "\n";
    }

    return 0;
}
