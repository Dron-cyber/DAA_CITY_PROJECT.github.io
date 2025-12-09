/*
    NOTE:
    This program demonstrates the gate assignment algorithm using a sample set 
    of flight records entered by the user. The input dataset is only for 
    illustration and testing of the greedy scheduling logic that assigns gates 
    based on arrival and departure times.

    The system is fully scalable — larger datasets, real airport schedules, or 
    CSV-based inputs can be integrated without modifying the core heap-based 
    gate allocation algorithm.
*/

#include <iostream>
#include <vector>
#include <string>
#include <algorithm>
#include <sstream>
#include <iomanip>
#include <queue>
#include <unordered_map>

using namespace std;

// --- Data Structure Definitions ---

/**
 * @brief Represents a single flight.
 */
struct Flight {
    string id;
    string arrivalStr;   
    string departureStr; 
    int arrival;         // Time in minutes
    int departure;       // Time in minutes
};

// --- Utility Functions ---

/*
 * @brief Converts HH:MM string to total minutes from midnight (00:00).
 * @param timeStr Time in "HH:MM" format.
 * @return Total minutes.
 */
int timeToMinutes(const string& timeStr) {
    if (timeStr.length() != 5 || timeStr[2] != ':') return -1;
    try {
        int hh = stoi(timeStr.substr(0, 2));
        int mm = stoi(timeStr.substr(3, 2));
        return hh * 60 + mm;
    } catch (...) {
        return -1; // Return -1 on failure
    }
}

/*
 * @brief Converts total minutes back to HH:MM format for output.
 * @param minutes Total minutes from midnight.
 * @return Time string in "HH:MM" format.
 */
string minutesToTime(int minutes) {
    int hh = minutes / 60;
    int mm = minutes % 60;
    stringstream ss;
    ss << setfill('0') << setw(2) << hh << ":" << setfill('0') << setw(2) << mm;
    return ss.str();
}

// --- Main Algorithm Implementation ---

int main() {
    
    vector<Flight> flights;
    int numFlights;

    // 1. Get User Input for Flight Data
    cout << "--- Gate Assignment Input ---\n";
    cout << "Enter the total number of flights: ";
    if (!(cin >> numFlights) || numFlights <= 0) {
        cerr << "Invalid number of flights. Exiting.\n";
        return 1;
    }
    cin.ignore(numeric_limits<streamsize>::max(), '\n'); // Clear buffer

    cout << "Enter flight details (ID, Arrival HH:MM, Departure HH:MM):\n";
    for (int i = 0; i < numFlights; ++i) {
        Flight f;
        string line;
        
        cout << "Flight " << i + 1 << " ID: ";
        getline(cin, f.id);
        
        cout << "Flight " << i + 1 << " Arrival (HH:MM): ";
        getline(cin, f.arrivalStr);
        
        cout << "Flight " << i + 1 << " Departure (HH:MM): ";
        getline(cin, f.departureStr);

        // Convert times and validate input
        f.arrival = timeToMinutes(f.arrivalStr);
        f.departure = timeToMinutes(f.departureStr);

        if (f.arrival == -1 || f.departure == -1 || f.arrival >= f.departure) {
            cerr << "Warning: Invalid time format or departure time is not after arrival for flight " << f.id << ". Skipping this flight.\n";
            i--; // Decrement i to re-prompt for the current flight number
            continue;
        }

        flights.push_back(f);
    }
    
    if (flights.empty()) {
        cout << "No valid flights to process. Exiting.\n";
        return 0;
    }

    // 2. Greedy Strategy Step 1: Sort by Arrival Time
    sort(flights.begin(), flights.end(), [](const Flight &a, const Flight &b) {
        return a.arrival < b.arrival;
    });

    // 3. Greedy Strategy Step 2: Initialize Gate Tracking Structures
    
    // Min-Heap (Priority Queue): Stores the availability time (<Departure_Time, Gate_Number>)
    // The top element is the earliest free gate.
    priority_queue<pair<int,int>, vector<pair<int,int>>, greater<pair<int,int>>> gateHeap;

    int gateCount = 0; // Tracks the minimum number of gates used
    unordered_map<string,int> flightGate; // Maps flight ID to its assigned gate number

    // 4. Process Flights and Assign Gates
    for (const auto &f : flights) {
        
        if (!gateHeap.empty() && gateHeap.top().first <= f.arrival) {
            // REUSE GATE: The earliest available gate is free before the current flight arrives.
            int gateNum = gateHeap.top().second;
            gateHeap.pop(); 
            gateHeap.push({f.departure, gateNum}); // Update availability time
            flightGate[f.id] = gateNum;
            
        } else {
            // NEW GATE: No gate is available, so a new one is required.
            gateCount++;
            gateHeap.push({f.departure, gateCount}); // Add new gate with its release time
            flightGate[f.id] = gateCount;
        }
    }

    // 5. Output Results
    
    cout << "\n--- Gate Assignment Schedule ---\n";
    cout << "Total Gates Required: " << gateCount << "\n";
    cout << "--------------------------------\n";
    cout << "FlightID\tArrival\tDeparture\tGate\n";
    
    for (const auto &f : flights) {
        cout << f.id << "\t\t" 
             << minutesToTime(f.arrival) << "\t" 
             << minutesToTime(f.departure) << "\t" 
             << flightGate.at(f.id) << "\n";
    }

    return 0;
}
