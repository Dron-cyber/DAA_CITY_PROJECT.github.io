/*
    NOTE:
    This program uses sample crime data (from crime.csv) to demonstrate the
    clustering algorithm for identifying high-crime hotspots. The dataset is 
    not real city data—it is provided only for testing, illustration, and 
    validating the DFS-based hotspot detection logic.

    The system is fully scalable. More rows, columns, or real-world crime 
    records can be added easily without modifying the algorithm.
*/

#include <iostream>
#include <vector>
#include <string>
#include <sstream>
#include <queue>
#include <algorithm>
#include <map>
#include <tuple>

using namespace std;

// --- GLOBAL CONFIGURATION ---
const int ROWS = 15;
const int COLS = 15;
const int DR[] = {-1, 1, 0, 0}; // Row changes (4-way connectivity)
const int DC[] = {0, 0, -1, 1}; // Column changes
const int NUM_DIRECTIONS = 4;

// Global data structures
int crime_grid[ROWS][COLS] = {0};
bool visited[ROWS][COLS] = {false};
// Map to store the place name for reporting purposes: Key = (row*COLS + col), Value = place_name
map<int, string> place_name_map; 


/**
 * @brief Utility to get a unique key for the map based on 0-based coordinates.
 */
int get_key(int r, int c) {
    return r * COLS + c;
}

/**
 * @brief Depth First Search to find a connected high-crime cluster.
 * * @param r Current row index (0-based).
 * @param c Current column index (0-based).
 * @param threshold The crime score threshold.
 * @return The total severity score (sum of crime scores) of the connected cluster.
 */
int dfs_cluster(int r, int c, int threshold) {
    // Base Case: Out of bounds, already visited, or below crime threshold
    if (r < 0 || r >= ROWS || c < 0 || c >= COLS || visited[r][c] || crime_grid[r][c] < threshold) {
        return 0;
    }

    // Mark current cell as visited
    visited[r][c] = true;
    
    int current_cluster_score = crime_grid[r][c];

    // Recurse on all adjacent neighbors
    for (int i = 0; i < NUM_DIRECTIONS; ++i) {
        current_cluster_score += dfs_cluster(r + DR[i], c + DC[i], threshold);
    }

    return current_cluster_score;
}


/**
 * @brief Parses the raw dataset string into the global data structures.
 */
void parse_data(const string& raw_data) {
    stringstream ss(raw_data);
    int row_1based, col_1based, score;
    string token;

    // Skip the header line
    string header_line;
    getline(ss, header_line);

    // Process data records
    while (ss >> row_1based && ss >> col_1based) {
        
        string current_place_name = "";
        
        // Read tokens until the next one is an integer (the score)
        while (ss >> token) {
            bool is_score = true;
            for (char ch : token) {
                if (!isdigit(ch)) {
                    is_score = false;
                    break;
                }
            }
            
            if (is_score) {
                score = stoi(token);
                break;
            } else {
                // Append token to place name, adding space if not the first token
                if (!current_place_name.empty()) current_place_name += " ";
                current_place_name += token;
            }
        }

        // Convert 1-based index to 0-based index
        int r_0based = row_1based - 1;
        int c_0based = col_1based - 1;

        if (r_0based >= 0 && r_0based < ROWS && c_0based >= 0 && c_0based < COLS) {
            crime_grid[r_0based][c_0based] = score;
            place_name_map[get_key(r_0based, c_0based)] = current_place_name;
        }
    }
}


int main() {
    // Dataset provided by the user (multiline string)
    const string dataset = 
        "row col place_name crime_score\n"
        "2 1 Central Market 3\n2 2 Central Market 6\n2 3 Central Market 7\n2 4 Railway Station Road 2\n"
        "2 5 Railway Station Road 1\n2 6 Old Bus Stand 0\n2 7 Shopping Street 5\n2 8 Shopping Street 6\n"
        "2 9 Shopping Street 8\n2 10 Shopping Street 7\n2 11 Railway Colony 3\n2 12 Railway Colony 1\n"
        "2 13 Old Bus Stand 0\n2 14 Shopping Street 4\n2 15 Shopping Street 6\n3 1 Railway Colony 1\n"
        "3 2 Railway Colony 2\n3 3 Central Market 6\n3 4 Central Market 7\n3 5 Central Market 8\n"
        "3 6 Railway Station Road 3\n3 7 Railway Station Road 2\n3 8 Old Bus Stand 1\n3 9 Shopping Street 6\n"
        "3 10 Shopping Street 7\n3 11 Old Bus Stand 0\n3 12 Railway Colony 2\n3 13 Railway Colony 3\n"
        "3 14 Shopping Street 6\n3 15 Shopping Street 7\n4 1 Residential Block A 0\n4 2 Residential Block A 1\n"
        "4 3 Residential Block A 2\n4 4 Residential Block A 6\n4 5 Residential Block A 7\n4 6 Residential Block A 8\n"
        "4 7 Residential Block A 5\n4 8 Residential Block B 1\n4 9 Residential Block B 0\n4 10 Residential Block B 3\n"
        "4 11 Residential Block B 6\n4 12 Residential Block B 7\n4 13 Residential Block B 8\n4 14 Residential Block C 6\n"
        "4 15 Residential Block C 1\n5 1 Residential Block A 2\n5 2 Residential Block A 3\n5 3 Residential Block A 0\n"
        "5 4 Residential Block A 1\n5 5 Residential Block B 6\n5 6 Residential Block B 7\n5 7 Residential Block B 8\n"
        "5 8 Residential Block B 6\n5 9 Residential Block B 3\n5 10 Residential Block C 1\n5 11 Residential Block C 0\n"
        "5 12 Residential Block C 4\n5 13 Residential Block C 6\n5 14 Residential Block C 7\n5 15 Residential Block C 8\n"
        "6 1 Residential Block A 3\n6 2 Residential Block A 6\n6 3 Residential Block B 7\n6 4 Residential Block B 8\n"
        "6 5 Residential Block B 2\n6 6 Residential Block B 1\n6 7 Residential Block C 0\n6 8 Residential Block C 6\n"
        "6 9 Residential Block C 7\n6 10 Residential Block C 8\n6 11 Residential Block C 3\n6 12 Residential Block C 2\n"
        "6 13 Residential Block C 1\n6 14 Residential Block C 6\n6 15 Residential Block C 7\n7 1 Old City 1\n"
        "7 2 Old City 2\n7 3 Old City 3\n7 4 Old City 6\n7 5 Slum Area 7\n7 6 Slum Area 8\n7 7 Slum Area 6\n"
        "7 8 Old City 3\n7 9 Old City 2\n7 10 Old City 1\n7 11 Slum Area 0\n7 12 Slum Area 6\n7 13 Slum Area 7\n"
        "7 14 Slum Area 8\n7 15 Slum Area 6\n8 1 Slum Area 0\n8 2 Slum Area 1\n8 3 Slum Area 6\n8 4 Slum Area 7\n"
        "8 5 Slum Area 8\n8 6 Slum Area 7\n8 7 Slum Area 6\n8 8 Old City 3\n8 9 Old City 1\n8 10 Old City 0\n"
        "8 11 Old City 2\n8 12 Slum Area 6\n8 13 Slum Area 7\n8 14 Slum Area 8\n8 15 Slum Area 6\n9 1 Old City 2\n"
        "9 2 Old City 3\n9 3 Slum Area 6\n9 4 Slum Area 1\n9 5 Slum Area 0\n9 6 Old City 2\n9 7 Slum Area 6\n"
        "9 8 Slum Area 7\n9 9 Slum Area 8\n9 10 Slum Area 6\n9 11 Old City 3\n9 12 Old City 1\n9 13 Old City 0\n"
        "9 14 Old City 4\n9 15 Old City 6\n10 1 IT Park 1\n10 2 IT Park 6\n10 3 IT Park 7\n10 4 IT Park 8\n"
        "10 5 Commercial Hub 2\n10 6 Commercial Hub 1\n10 7 Commercial Hub 0\n10 8 Commercial Hub 6\n"
        "10 9 Commercial Hub 7\n10 10 Commercial Hub 8\n10 11 IT Park Extension 3\n10 12 IT Park Extension 2\n"
        "10 13 IT Park Extension 6\n10 14 IT Park Extension 7\n10 15 IT Park Extension 8\n11 1 IT Park 0\n"
        "11 2 IT Park 1\n11 3 IT Park 6\n11 4 IT Park 7\n11 5 Commercial Hub 8\n11 6 Commercial Hub 3\n"
        "11 7 Commercial Hub 2\n11 8 Commercial Hub 6\n11 9 Commercial Hub 7\n11 10 Commercial Hub 8\n"
        "11 11 IT Park Extension 3\n11 12 IT Park Extension 1\n11 13 IT Park Extension 6\n11 14 IT Park Extension 7\n"
        "11 15 IT Park Extension 8\n12 1 IT Park 3\n12 2 IT Park 6\n12 3 IT Park 7\n12 4 IT Park 8\n"
        "12 5 Commercial Hub 2\n12 6 Commercial Hub 1\n12 7 Commercial Hub 0\n12 8 IT Park Extension 6\n"
        "12 9 IT Park Extension 7\n12 10 IT Park Extension 8\n12 11 IT Park Extension 3\n12 12 IT Park Extension 0\n"
        "12 13 IT Park Extension 2\n12 14 IT Park Extension 6\n12 15 IT Park Extension 7\n13 1 Airport Road 2\n"
        "13 2 Airport Road 3\n13 3 Airport Road 1\n13 4 Industrial Area 6\n13 5 Industrial Area 7\n"
        "13 6 Industrial Area 8\n13 7 Industrial Area 6\n13 8 Industrial Belt 3\n13 9 Industrial Belt 1\n"
        "13 10 Airport Zone 0\n13 11 Airport Zone 2\n13 12 Airport Zone 6\n13 13 Airport Zone 7\n"
        "13 14 Industrial Belt 8\n13 15 Industrial Belt 6\n14 1 Airport Road 1\n14 2 Airport Road 2\n"
        "14 3 Industrial Area 6\n14 4 Industrial Area 7\n14 5 Industrial Area 8\n14 6 Industrial Area 7\n"
        "14 7 Industrial Area 6\n14 8 Industrial Belt 3\n14 9 Industrial Belt 1\n14 10 Airport Zone 0\n"
        "14 11 Airport Zone 2\n14 12 Airport Zone 6\n14 13 Airport Zone 7\n14 14 Industrial Belt 8\n"
        "14 15 Industrial Belt 6\n15 1 Airport Road 0\n15 2 Airport Road 1\n15 3 Industrial Area 6\n"
        "15 4 Industrial Area 7\n15 5 Industrial Area 8\n15 6 Industrial Belt 3\n15 7 Industrial Belt 2\n"
        "15 8 Industrial Belt 6\n15 9 Industrial Belt 7\n15 10 Airport Zone 8\n15 11 Airport Zone 3\n"
        "15 12 Airport Zone 1\n15 13 Airport Zone 6\n15 14 Industrial Belt 7\n15 15 Industrial Belt 8\n";
        
    // 1. Parse Data
    parse_data(dataset);

    // --- USER INPUT ---
    int threshold;
    int max_clusters;
    cout << "--- Police Resource Deployment Prioritization Tool ---\n";
    cout << "Step 1: Define a 'High-Crime' Area\n";
    cout << "Enter the **minimum crime score threshold** (e.g., 5, areas below this are ignored): ";
    cin >> threshold;

    cout << "\nStep 2: Define Deployment Priority\n";
    cout << "Enter the **maximum number of clusters (hotspots)** to rank (e.g., 5): ";
    cin >> max_clusters;

    // A Max-Heap (Priority Queue) to store and rank cluster scores. 
    // We store a tuple: <Score, Row (1-based), Col (1-based)>
    // The Max-Heap will naturally sort by the first element (Score).
    priority_queue<tuple<int, int, int>> cluster_max_heap;
    
    // 3. Iterate and Cluster
    for (int i = 0; i < ROWS; ++i) {
        for (int j = 0; j < COLS; ++j) {
            
            // Check for a new unvisited high-crime area (Cluster starting point)
            if (crime_grid[i][j] >= threshold && !visited[i][j]) {
                
                // Use DFS to find the connected component and sum its crime scores
                int score = dfs_cluster(i, j, threshold);
                
                if (score > 0) {
                    // Push (Score, 1-based Row, 1-based Col) to the Max-Heap
                    cluster_max_heap.push({score, i + 1, j + 1});
                }
            }
        }
    }
    
    // 4 & 5. Rank and Report Clusters
    cout << "\n--- TOP " << max_clusters << " HIGH-CRIME HOTSPOT RANKING ---\n";
    cout << "Ranking is based on 'Severity Score' (sum of all crime scores in the cluster).\n";

    if (cluster_max_heap.empty()) {
        cout << "No connected high-crime clusters found above the threshold of " << threshold << ".\n";
    } else {
        int rank = 1;
        
        while (!cluster_max_heap.empty() && rank <= max_clusters) {
            auto top_cluster = cluster_max_heap.top();
            cluster_max_heap.pop();
            
            int score = get<0>(top_cluster);
            int r = get<1>(top_cluster); // 1-based row
            int c = get<2>(top_cluster); // 1-based col
            
            // Get the place name for the starting coordinate
            string place_name = place_name_map[get_key(r - 1, c - 1)];

            cout << "------------------------------------------\n";
            cout << "RANK " << rank++ << " (Deployment Priority)\n";
            cout << "  Severity Score: " << score << "\n";
            cout << "  Starting Area:  " << place_name << "\n";
            cout << "  Grid Location:  [Row " << r << ", Col " << c << "]\n";
        }
    }
    
    // Report remaining clusters if the max_clusters limit was hit
    if (!cluster_max_heap.empty()) {
        cout << "\nNote: " << cluster_max_heap.size() << " additional high-crime clusters remain unranked in the top list.\n";
    }

    return 0;
}
