/*
    NOTE:
    The parking dataset used in this program is a sample dataset created 
    specifically for demonstrating the Min-Heap (for nearest free slot) 
    and Hash Map (for O(1) occupancy lookup) working together in a 
    real-time parking management system.

    The system is fully scalable — more slots, additional attributes 
    (such as place names, vehicle types, timestamps), or real parking 
    layouts can be added without modifying the core heap and hashing logic.
*/

#include <iostream>
#include <vector>
#include <string>
#include <sstream>
#include <queue>
#include <unordered_map>
#include <algorithm>
#include <functional>
#include <limits>

using namespace std;

// --- GLOBAL CONFIGURATION AND DATA STRUCTURES ---

// Min-Heap: Stores the IDs of all currently FREE parking slots (smallest ID = closest).
priority_queue<int, vector<int>, greater<int>> free_slots_min_heap;

// Hash Map: Tracks OCCUPIED slots: Key=Vehicle ID, Value=Slot ID. (O(1) lookup)
unordered_map<string, int> occupancy_map;

// Map to store initial slot data (Now simplified as place_name is removed)
struct SlotInfo {
    int distance;
    bool is_occupied;
    string vehicle_id;
};
unordered_map<int, SlotInfo> parking_lot_info;


/**
 * @brief Parses the raw dataset and initializes the system state.
 *
 * NOTE: The dataset format is now: slot_id distance_from_gate occupied vehicle_id (if occupied).
 */
void initialize_system(const string& raw_data) {
    stringstream ss(raw_data);
    string line;
    getline(ss, line); // Skip the header line

    int slot_id, distance;
    int occupied_int;
    string vehicle_id;

    while (ss >> slot_id >> distance >> occupied_int) {
        
        vehicle_id = ""; // Reset vehicle ID for each slot

        // Read the potential vehicle ID if the slot is occupied
        if (occupied_int == 1) {
            if (!(ss >> vehicle_id)) {
                // Failsafe, though unlikely with the provided data structure
                vehicle_id = "N/A"; 
            }
        }

        // Store initial slot information
        parking_lot_info[slot_id] = {
            distance, 
            (occupied_int == 1), 
            vehicle_id
        };

        if (occupied_int == 0) {
            // Add free slots to the Min-Heap
            free_slots_min_heap.push(slot_id);
        } else if (occupied_int == 1) {
            // Add occupied slots to the Hash Map
            occupancy_map[vehicle_id] = slot_id;
        }
    }
}

/**
 * @brief Displays the main operational menu to the user, including status updates.
 */
void display_menu() {
    cout << "\n-------------------------------------------------\n";
    cout << "           Parking System Control Menu\n";
    cout << "-------------------------------------------------\n";
    cout << "1. New Vehicle Arrival (Assign nearest empty slot)\n";
    cout << "2. Vehicle Exit (Make slot empty)\n";
    cout << "3. Exit Program\n";
    cout << "-------------------------------------------------\n";
    cout << "System Status: Free Slots: " << free_slots_min_heap.size() 
         << " | Occupied: " << occupancy_map.size() << "\n";
    
    if (!free_slots_min_heap.empty()) {
        int closest_slot_id = free_slots_min_heap.top();
        // Distance is used here for context since place_name is missing
        cout << "NEXT UP: Closest Available Slot is " << closest_slot_id 
             << " (Distance: " << parking_lot_info[closest_slot_id].distance << " units)\n";
    } else {
        cout << "Parking lot is FULL.\n";
    }
    cout << "Enter your choice (1-3): ";
}

/**
 * @brief Handles vehicle arrival and assigns the closest available slot.
 */
void handle_arrival() {
    string vehicle_id;
    cout << "Enter arriving Vehicle ID (e.g., KA28FX1234): ";
    cin >> vehicle_id;

    if (occupancy_map.count(vehicle_id)) {
        cout << "[ERROR] Vehicle " << vehicle_id << " is already parked in Slot " 
             << occupancy_map[vehicle_id] << ".\n";
        return;
    }

    if (free_slots_min_heap.empty()) {
        cout << "[SYSTEM] ERROR: Parking lot is full. Cannot assign slot to " << vehicle_id << ".\n";
        return;
    }

    // Min-Heap (Greedy): Get the closest available slot (O(log N))
    int assigned_slot = free_slots_min_heap.top();
    free_slots_min_heap.pop(); 
    int distance = parking_lot_info[assigned_slot].distance;

    // Hash Map: Record the assignment (O(1) average)
    occupancy_map[vehicle_id] = assigned_slot; 

    // Update slot info
    parking_lot_info[assigned_slot].is_occupied = true;
    parking_lot_info[assigned_slot].vehicle_id = vehicle_id;

    cout << "\n[SUCCESS] Vehicle **" << vehicle_id << "** assigned to **Slot " << assigned_slot 
         << "** (Distance: " << distance << " units).\n";
}

/**
 * @brief Handles vehicle exit and releases the parking slot.
 */
void handle_exit() {
    string vehicle_id;
    cout << "Enter exiting Vehicle ID: ";
    cin >> vehicle_id;

    // Hash Map (Hashing): Find the occupied slot (O(1) average)
    auto it = occupancy_map.find(vehicle_id);

    if (it == occupancy_map.end()) {
        cout << "[ERROR] Vehicle ID **" << vehicle_id << "** not found in occupancy records. Check ID.\n";
        return;
    }

    int released_slot = it->second;

    // Hash Map: Remove the occupancy record (O(1) average)
    occupancy_map.erase(it); 

    // Min-Heap: Restore slot availability (O(log N))
    free_slots_min_heap.push(released_slot); 

    // Update slot info
    parking_lot_info[released_slot].is_occupied = false;
    parking_lot_info[released_slot].vehicle_id = "";
    
    cout << "\n[SUCCESS] Vehicle **" << vehicle_id << "** released **Slot " << released_slot 
         << "** (Distance: " << parking_lot_info[released_slot].distance << " units). Slot is now FREE.\n";
}


int main() {
    // New Dataset provided by the user (without place_name)
    const string dataset = 
        "slot_id distance_from_gate occupied vehicle_id\n"
        "1 12 0\n2 18 1 KA25MJ4432\n3 22 0\n4 10 1 KA28FJ1123\n"
        "5 15 0\n6 20 0\n7 8 1 KA25AC9921\n8 14 0\n9 19 0\n10 5 1 KA25P4451\n"
        "11 9 1 KA19AB7722\n12 15 0\n13 7 1 KA25TR5543\n14 13 0\n15 20 0\n16 6 1 KA25M8891\n"
        "17 11 0\n18 9 1 KA28ZQ3491\n19 16 0\n20 7 1 KA25BC9933\n"
        "21 13 1 KA19QW1192\n22 19 0\n23 4 1 KA01HZ9911\n24 10 1 KA28AA4478\n25 16 0\n26 6 1 KA25LM1122\n"
        "27 12 0\n28 8 0\n29 10 1 KA25JK7711\n30 15 0\n";
    
    cout << "--- Initializing Parking System State from Dataset ---\n";
    initialize_system(dataset);
    cout << "Initialization complete. Ready for transactions.\n";
    
    

    int choice;
    do {
        display_menu();
        if (!(cin >> choice)) {
            // Handle invalid input (non-integer)
            cout << "[ERROR] Invalid input. Please enter a number (1, 2, or 3).\n";
            cin.clear();
            cin.ignore(numeric_limits<streamsize>::max(), '\n');
            continue;
        }

        switch (choice) {
            case 1:
                handle_arrival();
                break;
            case 2:
                handle_exit();
                break;
            case 3:
                cout << "\nExiting the Parking Management System. Goodbye!\n";
                break;
            default:
                cout << "[ERROR] Invalid choice. Please select 1, 2, or 3.\n";
                break;
        }
    } while (choice != 3);

    return 0;
}
