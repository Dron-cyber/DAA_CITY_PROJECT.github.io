/*
    NOTE:
    This program demonstrates a dynamic land-allocation system using a Segment Tree.
    The initial plot statuses are sample input provided by the user only for
    testing the allocation, release, and range-query operations.

    The code is fully scalable — larger datasets, real city layouts, or CSV-based
    inputs can be integrated without changing the core Segment Tree logic.
*/

#include <iostream>
#include <vector>
#include <cmath>
#include <algorithm>

using namespace std;

// Global variables for the Segment Tree
vector<int> plots; // The base array of land plots (0: Free, 1: Occupied)
vector<int> tree;  // The Segment Tree array
int N;             // Total number of plots

// --- Segment Tree Functions ---

/**
 * @brief Builds the Segment Tree.
 * Each node stores the COUNT OF FREE PLOTS in its range.
 * Time Complexity: O(N)
 * @param node Index of the current node in the 'tree' array.
 * @param start Start index of the range covered by 'node' in the 'plots' array.
 * @param end End index of the range covered by 'node' in the 'plots' array.
 */
void build(int node, int start, int end) {
    if (start == end) {
        // Leaf node: stores the status of a single plot.
        // We store 1 if the plot is free (0), and 0 if it's occupied (1).
        tree[node] = (plots[start] == 0) ? 1 : 0;
        return;
    }
    
    int mid = start + (end - start) / 2;
    
    // Recursively build children
    build(2 * node, start, mid);
    build(2 * node + 1, mid + 1, end);
    
    // Internal node: aggregate the results from children.
    // Sum of free plots in the left range + Sum of free plots in the right range.
    tree[node] = tree[2 * node] + tree[2 * node + 1];
}

/**
 * @brief Point Update: Allocates or releases a single plot.
 * Time Complexity: O(log N)
 * @param node Index of the current node.
 * @param start Start index of the range.
 * @param end End index of the range.
 * @param idx Index of the plot being updated.
 * @param value New status of the plot (0 or 1).
 */
void update(int node, int start, int end, int idx, int value) {
    if (start == end) {
        // Leaf node reached. Update the base array and the tree node.
        plots[idx] = value;
        // Update tree value: 1 if free (value=0), 0 if occupied (value=1).
        tree[node] = (value == 0) ? 1 : 0;
        return;
    }
    
    int mid = start + (end - start) / 2;
    
    // Recurse down the appropriate child based on the index.
    if (start <= idx && idx <= mid) {
        update(2 * node, start, mid, idx, value);
    } else {
        update(2 * node + 1, mid + 1, end, idx, value);
    }
    
    // Update internal node: re-calculate the sum of free plots from children.
    tree[node] = tree[2 * node] + tree[2 * node + 1];
}

/**
 * @brief Range Query: Finds the total number of free plots in a given range [L, R].
 * Time Complexity: O(log N)
 * @param node Index of the current node.
 * @param start Start index of the range covered by 'node'.
 * @param end End index of the range covered by 'node'.
 * @param L Query range left boundary.
 * @param R Query range right boundary.
 * @return The sum of free plots in the intersection of [start, end] and [L, R].
 */
int query(int node, int start, int end, int L, int R) {
    // 1. Completely outside the query range
    if (start > R || end < L) {
        return 0; // Contributes 0 free plots
    }
    
    // 2. Completely inside the query range
    if (L <= start && end <= R) {
        return tree[node]; // Return the pre-calculated aggregate value
    }
    
    // 3. Partially overlaps
    int mid = start + (end - start) / 2;
    
    // Recursively query left and right children and sum the results
    int p1 = query(2 * node, start, mid, L, R);
    int p2 = query(2 * node + 1, mid + 1, end, L, R);
    
    return p1 + p2;
}

// --- Main Program and User Interface ---

void printPlotStatus() {
    cout << "\n--- Current City Plot Status ---\n";
    cout << "Index:\t";
    for (int i = 0; i < N; ++i) {
        cout << i << "\t";
    }
    cout << "\nStatus:\t";
    for (int status : plots) {
        cout << (status == 1 ? "OCC" : "FREE") << "\t";
    }
    cout << "\n--------------------------------\n";
}

int main() {
    cout << "--- My City Map Land Management System (Segment Tree) ---\n";
    
    // 1. Input Total Plots
    cout << "Enter the total number of land plots (N): ";
    if (!(cin >> N) || N <= 0) {
        cerr << "Invalid plot count.\n";
        return 1;
    }

    // Resize data structures (Segment Tree size is roughly 4*N)
    plots.resize(N);
    tree.resize(4 * N + 1); 

    // 2. Input Initial Plot Status
    cout << "Enter initial status for each plot (0 for Free, 1 for Occupied):\n";
    for (int i = 0; i < N; ++i) {
        cout << "Plot " << i << ": ";
        int status;
        if (!(cin >> status) || (status != 0 && status != 1)) {
            cerr << "Invalid status. Use 0 or 1. Exiting.\n";
            return 1;
        }
        plots[i] = status;
    }

    // 3. Build the Segment Tree
    build(1, 0, N - 1); // Start building from root (node 1) covering the whole range [0, N-1]

    // 4. Main Loop for Dynamic Operations
    int choice;
    do {
        printPlotStatus();
        cout << "\n--- Select Operation ---\n";
        cout << "1. Allocate Land (Update Status to Occupied)\n";
        cout << "2. Release Land (Update Status to Free)\n";
        cout << "3. Query Free Plots in Range\n";
        cout << "4. Exit\n";
        cout << "Enter choice: ";
        cin >> choice;

        if (choice == 1 || choice == 2) {
            // Update Operation
            int plot_idx;
            cout << "Enter plot index to update (0 to " << N - 1 << "): ";
            if (!(cin >> plot_idx) || plot_idx < 0 || plot_idx >= N) {
                cout << "Invalid index.\n";
                continue;
            }
            
            int new_status = (choice == 1) ? 1 : 0; // 1: Occupied, 0: Free
            
            if (plots[plot_idx] == new_status) {
                cout << "Plot " << plot_idx << " is already " << (new_status == 1 ? "Occupied" : "Free") << ".\n";
            } else {
                update(1, 0, N - 1, plot_idx, new_status);
                cout << "SUCCESS: Plot " << plot_idx << " status updated to " << (new_status == 1 ? "OCCUPIED" : "FREE") << ".\n";
            }
            
        } else if (choice == 3) {
            // Range Query Operation
            int L, R;
            cout << "Enter query range start index (L): ";
            cin >> L;
            cout << "Enter query range end index (R): ";
            if (!(cin >> R) || L < 0 || R >= N || L > R) {
                cout << "Invalid range.\n";
                continue;
            }
            
            // Perform the O(log N) query
            int free_count = query(1, 0, N - 1, L, R);
            
            cout << "\nQUERY RESULT: In the range [" << L << ", " << R << "], there are " 
                 << free_count << " FREE plots available.\n";
                 
        } else if (choice == 4) {
            cout << "Exiting City Land Management System.\n";
        } else {
            cout << "Invalid choice. Please try again.\n";
        }
    } while (choice != 4);

    return 0;
}
