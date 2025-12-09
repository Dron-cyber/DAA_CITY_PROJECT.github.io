// This program implements Dijkstra's algorithm to find the shortest route between two locations.
// The current graph is based on sample data generated for our city map project.
// It is fully functional for these nodes and edges, but the graph can be easily expanded
// by adding more junctions, roads, and corresponding distances or travel times.
#include <iostream>
#include <vector>
#include <queue>
#include <climits>
#include <unordered_map>
#include <string>

using namespace std;

typedef pair<int, int> pii;

class CityGraph {
    int V;
    vector<vector<pii>> adj;
public:
    CityGraph(int V) {
        this->V = V;
        adj.resize(V);
    }

    void addRoad(int u, int v, int w) {
        adj[u].push_back({v, w});
        adj[v].push_back({u, w}); // Bidirectional
    }

    void shortestPath(int src, int dest, unordered_map<int, string>& idToName) {
        vector<int> dist(V, INT_MAX);
        vector<int> parent(V, -1);
        priority_queue<pii, vector<pii>, greater<pii>> pq;

        dist[src] = 0;
        pq.push({0, src});

        while (!pq.empty()) {
            int u = pq.top().second;
            int d = pq.top().first;
            pq.pop();

            if (d > dist[u]) continue;

            for (auto edge : adj[u]) {
                int v = edge.first;
                int w = edge.second;
                if (dist[u] + w < dist[v]) {
                    dist[v] = dist[u] + w;
                    parent[v] = u;
                    pq.push({dist[v], v});
                }
            }
        }

        if (dist[dest] == INT_MAX) {
            cout << "No path found from " << idToName[src] << " to " << idToName[dest] << endl;
            return;
        }

        cout << "\nShortest distance from " << idToName[src] << " to " << idToName[dest] << " is: " << dist[dest] << " minutes\n";

        cout << "Path: ";
        vector<int> path;
        for (int v = dest; v != -1; v = parent[v])
            path.push_back(v);
        for (int i = path.size() - 1; i >= 0; i--)
            cout << idToName[path[i]] << (i == 0 ? "\n" : " -> ");
    }
};

int main() {
    // Node mapping
    unordered_map<string, int> nameToId = {
        {"N1",0}, {"N2",1}, {"N3",2}, {"N4",3}, {"N5",4}, {"N6",5},
        {"N7",6}, {"N8",7}, {"N9",8}, {"N10",9}, {"N11",10}, {"N12",11},
        {"N13",12}, {"N14",13}, {"N15",14}, {"N16",15}, {"N17",16}, {"N18",17}
    };

    unordered_map<int, string> idToName = {
        {0,"Central Circle Junction"}, {1,"City Civic Center (CCC)"}, {2,"School Main Gate"},
        {3,"University Junction"}, {4,"Residential Area North"}, {5,"Residential Area South"},
        {6,"Hospital"}, {7,"Fire Station"}, {8,"Shopping District"},
        {9,"Industrial Area Gate"}, {10,"IT & Tech Park"}, {11,"Airport Entrance"},
        {12,"Airport Terminal"}, {13,"Forest Edge"}, {14,"Park & Lake"},
        {15,"Bus Stand"}, {16,"Railway Station"}, {17,"Govt Service Center"}
    };

    CityGraph city(18);

    // Add edges (time in minutes)
    city.addRoad(0,1,4); city.addRoad(0,2,3); city.addRoad(0,8,3); city.addRoad(0,15,5);
    city.addRoad(1,3,5); city.addRoad(1,6,7);
    city.addRoad(2,4,4); city.addRoad(2,8,3);
    city.addRoad(3,5,5); city.addRoad(3,10,8);
    city.addRoad(4,6,6); city.addRoad(4,14,4);
    city.addRoad(5,17,5); city.addRoad(5,14,3);
    city.addRoad(6,7,2); city.addRoad(6,16,6);
    city.addRoad(7,9,9);
    city.addRoad(8,5,4); city.addRoad(8,15,3);
    city.addRoad(9,10,7); city.addRoad(9,13,8);
    city.addRoad(10,11,10);
    city.addRoad(11,12,2);
    city.addRoad(12,13,7);
    city.addRoad(13,14,5);
    city.addRoad(14,17,4);
    city.addRoad(15,16,4);
    city.addRoad(16,11,11);

    string sourceName, destName;
    cout << "Enter source place name: ";
    getline(cin, sourceName);
    cout << "Enter destination place name: ";
    getline(cin, destName);

    // Find node IDs
    int src = -1, dest = -1;
    for(auto &p: idToName) {
        if(p.second == sourceName) src = p.first;
        if(p.second == destName) dest = p.first;
    }

    if(src == -1 || dest == -1) {
        cout << "Invalid source or destination name entered!\n";
        return 0;
    }

    city.shortestPath(src, dest, idToName);

    return 0;
}

