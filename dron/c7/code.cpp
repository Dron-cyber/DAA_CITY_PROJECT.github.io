#include <iostream>
#include <vector>
#include <string>
#include <sstream>
#include <queue>
#include <algorithm>
#include <cmath>
#include <map>
#include <set>

using namespace std;

// --- 1. Fault Structure (Node for Max-Heap) ---
struct Fault {
    string id;
    string location;
    int x, y;
    int urgency;
    int complaints;
    int priority_score;
    string light_type;
    string reported_time;

    // Custom comparison for the Max-Heap (Priority Queue)
    // Max-Heap prioritizes the GREATEST element, so we compare priority_score directly.
    bool operator<(const Fault& other) const {
        return priority_score < other.priority_score;
    }
};

// Define the Max-Heap for fault scheduling (Priority Queue)
priority_queue<Fault> maintenance_schedule_heap;

// Map to store all faults, indexed by location/coordinates for graph lookups
map<pair<int, int>, Fault> fault_map;


// --- 2. Graph and BFS for Routing ---

// Represents a position in the city graph
struct Position {
    int x, y;
    int steps; // distance from the depot
};

// Function to calculate Euclidean distance (for simplicity in graph connection)
double distance_sq(int x1, int y1, int x2, int y2) {
    return pow(x1 - x2, 2) + pow(y1 - y2, 2);
}

// Global coordinates for the Maintenance Depot
const int DEPOT_X = 0;
const int DEPOT_Y = 0;


/**
 * @brief Breadth-First Search (BFS) to find the shortest path from a start point 
 * to the target fault location on a conceptual city grid.
 * * NOTE: Since a detailed street map is unavailable, this simulates BFS on a conceptual grid 
 * by exploring neighbors (simplified up, down, left, right movement) until the target is reached.
 * @param start_x Starting X-coordinate (e.g., Depot or previous fault).
 * @param start_y Starting Y-coordinate.
 * @param target_x Target Fault X-coordinate.
 * @param target_y Target Fault Y-coordinate.
 * @return The shortest number of steps (time units) to reach the target.
 */
int bfs_shortest_path(int start_x, int start_y, int target_x, int target_y) {
    if (start_x == target_x && start_y == target_y) return 0;

    // BFS queue: <Position>
    queue<Position> q;
    q.push({start_x, start_y, 0});
    
    // Visited set to prevent cycles and redundant checking
    set<pair<int, int>> visited;
    visited.insert({start_x, start_y});

    // Simulated movements (up, down, left, right)
    int dx[] = {0, 0, 1, -1};
    int dy[] = {1, -1, 0, 0};

    while (!q.empty()) {
        Position current = q.front();
        q.pop();

        for (int i = 0; i < 4; ++i) {
            int next_x = current.x + dx[i];
            int next_y = current.y + dy[i];

            if (next_x == target_x && next_y == target_y) {
                // Target reached! Shortest path found.
                return current.steps + 1;
            }

            // Simple check: Check if the new position is unvisited
            if (visited.find({next_x, next_y}) == visited.end()) {
                 visited.insert({next_x, next_y});
                 q.push({next_x, next_y, current.steps + 1});
            }
        }
    }
    // Should ideally not be reached if the city grid is large enough
    return -1; 
}


/**
 * @brief Parses the raw dataset and initializes the Max-Heap.
 */
void parse_data(const string& raw_data) {
    stringstream ss(raw_data);
    string line;
    getline(ss, line); // Skip the header line

    string id, location, light_type, reported_time;
    int x, y, urgency, complaints, priority_score;

    while (getline(ss, line)) {
        stringstream ls(line);
        ls >> id >> location >> x >> y >> urgency >> complaints >> priority_score >> light_type >> reported_time;

        // Replace underscores for readability if needed (assuming dataset uses them)
        replace(location.begin(), location.end(), '_', ' ');

        Fault f = {
            id, location, x, y, urgency, complaints, priority_score, light_type, reported_time
        };
        
        // Insert into the Max-Heap for scheduling
        maintenance_schedule_heap.push(f);
        
        // Store for graph lookups (if needed)
        fault_map[{x, y}] = f;
    }
}


int main() {
    // Dataset provided by the user (Underscores added for stable C++ stringstream parsing)
    const string dataset = 
        "FaultID Location Coord_X Coord_Y Urgency Complaints PriorityScore LightType ReportedTime\n"
        "F101 Market_Circle 4 25 8 9 143 LED 2025-12-08_18:22\n"
        "F102 Hospital_Road 33 72 8 10 26 Sodium 2025-12-08_19:10\n"
        "F103 School_Zone_East 55 81 7 8 22 LED 2025-12-08_17:50\n"
        "F104 Residential_Block_A 16 16 5 5 61 Fluorescent 2025-12-08_20:15\n"
        "F105 IT_&_Tech_Park_Main_Road 18 40 10 17 37 LED 2025-12-08_16:40\n"
        "F106 Airport_Road_Junction 12 22 8 12 28 LED 2025-12-08_17:10\n"
        "F107 Temple_Street 48 75 6 5 17 Sodium 2025-12-08_21:00\n"
        "F108 University_Road_North 70 78 7 9 23 LED 2025-12-08_18:45\n"
        "F109 Shopping_Complex_Lane 53 60 4 3 11 Fluorescent 2025-12-08_20:40\n"
        "F110 Industrial_Area_Gate_2 10 30 9 11 29 Sodium 2025-12-08_15:55\n";

    // 1. Initialization and Priority Scheduling (Max-Heap)
    parse_data(dataset);
    
    cout << "--- Streetlight Maintenance Scheduling System ---\n";
    cout << "Depot Location: (" << DEPOT_X << ", " << DEPOT_Y << ")\n\n";

    int current_x = DEPOT_X;
    int current_y = DEPOT_Y;
    int maintenance_order = 1;
    
    // 2. Process Faults (Greedy + Routing)
    while (!maintenance_schedule_heap.empty()) {
        
        // a) Greedy Selection: Pop the highest priority fault from the Max-Heap (O(log N))
        Fault next_fault = maintenance_schedule_heap.top();
        maintenance_schedule_heap.pop();
        
        // b) Routing Optimization: Run BFS for shortest path (O(V+E))
        int travel_steps = bfs_shortest_path(current_x, current_y, next_fault.x, next_fault.y);

        cout << "--- JOB #" << maintenance_order++ << " (Priority Score: " << next_fault.priority_score << ") ---\n";
        cout << "  Fault ID: " << next_fault.id << " (" << next_fault.light_type << ")\n";
        cout << "  Location: " << next_fault.location << " (" << next_fault.x << ", " << next_fault.y << ")\n";
        cout << "  Urgency: " << next_fault.urgency << " | Complaints: " << next_fault.complaints << "\n";
        cout << "  >> ROUTING: Shortest path from (" << current_x << ", " << current_y << ") to target: **" 
             << travel_steps << " steps**.\n\n";

        // Update the maintenance team's current location to the just-repaired fault
        current_x = next_fault.x;
        current_y = next_fault.y;
    }

    // 3. Final Report
    cout << "------------------------------------------\n";
    cout << "All " << maintenance_order - 1 << " faults have been scheduled and repaired.\n";
    cout << "Final team location: (" << current_x << ", " << current_y << ").\n";

    return 0;
}

