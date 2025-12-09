#include <iostream>
#include <vector>
#include <algorithm>
#include <limits>
#include <string>

const int INF = std::numeric_limits<int>::max() / 2;

void floyd_warshall(std::vector<std::vector<int>>& dist, int n) {
    for (int k = 1; k <= n; ++k) {
        for (int i = 1; i <= n; ++i) {
            for (int j = 1; j <= n; ++j) {
                if (dist[i][k] != INF && dist[k][j] != INF) {
                    dist[i][j] = std::min(dist[i][j], dist[i][k] + dist[k][j]);
                }
            }
        }
    }
}

int main() {
    int num_landmarks;
    int num_edges;

    std::cout << "Enter the number of landmarks (nodes): ";
    if (!(std::cin >> num_landmarks) || num_landmarks <= 1) {
        std::cerr << "Invalid number of landmarks. Exiting." << std::endl;
        return 1;
    }

    std::vector<std::vector<int>> dist(num_landmarks + 1, std::vector<int>(num_landmarks + 1, INF));

    for (int i = 1; i <= num_landmarks; ++i) {
        dist[i][i] = 0;
    }

    std::cout << "Enter the number of direct road connections (edges): ";
    if (!(std::cin >> num_edges) || num_edges < 0) {
        std::cerr << "Invalid number of edges. Exiting." << std::endl;
        return 1;
    }

    std::cout << "\nEnter the connections as: StartNode EndNode Time/Distance" << std::endl;
    std::cout << "Example: 1 2 5 (means 5 minutes from L1 to L2)" << std::endl;
    std::cout << "Note: Enter connections for both directions (i.e., 2 1 5 as well if undirected)." << std::endl;

    for (int e = 0; e < num_edges; ++e) {
        int u, v, weight;
        std::cout << "Edge " << e + 1 << ": ";
        if (!(std::cin >> u >> v >> weight) || u < 1 || v < 1 || u > num_landmarks || v > num_landmarks || weight < 0) {
            std::cerr << "Invalid input for edge. Skipping or exiting." << std::endl;
            e--;
            continue;
        }
        dist[u][v] = weight;
    }

    floyd_warshall(dist, num_landmarks);

    std::cout << "\n=========================================================" << std::endl;
    std::cout << "All-Pairs Shortest Travel Time Matrix (in minutes):" << std::endl;
    std::cout << "=========================================================" << std::endl;

    std::cout << "   \t";
    for (int j = 1; j <= num_landmarks; ++j) {
        std::cout << "L" << j << "\t";
    }
    std::cout << std::endl;

    for (int i = 1; i <= num_landmarks; ++i) {
        std::cout << "L" << i << "\t";
        for (int j = 1; j <= num_landmarks; ++j) {
            std::cout << (dist[i][j] == INF ? "INF" : std::to_string(dist[i][j])) << "\t";
        }
        std::cout << std::endl;
    }

    return 0;
}
