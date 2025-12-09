#include <iostream>
#include <queue>
#include <unordered_map>
#include <vector>
#include <string>
#include <sstream>
#include <iomanip>

using namespace std;

// Medicine batch structure
struct MedicineBatch {
    string batchID;
    string medicineName;
    string expiryDate;
    int quantity;
    string arrivalDate;
    string storageLocation;
    
    // For min-heap comparison (earliest expiry first)
    bool operator>(const MedicineBatch& other) const {
        return expiryDate > other.expiryDate;
    }
};

int main() {
    // Min-Heap (priority_queue) - smallest expiry date at top
    priority_queue<MedicineBatch, vector<MedicineBatch>, greater<MedicineBatch>> expiryHeap;
    
    // Hash Map: medicineName -> total quantity available
    unordered_map<string, int> medicineStock;
    
    // Initialize dataset
    vector<MedicineBatch> initialStock = {
        {"B001", "Paracetamol 500mg", "2025-02-10", 120, "2024-12-20", "Shelf A1"},
        {"B002", "Paracetamol 500mg", "2025-04-05", 150, "2025-01-15", "Shelf A1"},
        {"B003", "Ibuprofen 400mg", "2025-01-15", 80, "2024-12-10", "Shelf B2"},
        {"B004", "Ibuprofen 400mg", "2025-03-22", 100, "2025-01-05", "Shelf B2"},
        {"B005", "Amoxicillin 250mg", "2024-12-28", 60, "2024-11-30", "Refrigerator R1"},
        {"B006", "Amoxicillin 250mg", "2025-02-18", 90, "2025-01-02", "Refrigerator R1"},
        {"B007", "Cetrizine 10mg", "2025-03-10", 200, "2025-01-01", "Shelf C3"},
        {"B008", "Cetrizine 10mg", "2025-05-12", 180, "2025-02-05", "Shelf C3"},
        {"B009", "Azithromycin 500mg", "2025-01-30", 50, "2024-12-18", "Shelf D1"},
        {"B010", "Azithromycin 500mg", "2025-04-25", 70, "2025-01-22", "Shelf D1"},
        {"B011", "ORS Powder", "2024-12-20", 40, "2024-11-20", "Shelf E1"},
        {"B012", "ORS Powder", "2025-02-15", 65, "2025-01-10", "Shelf E1"},
        {"B013", "Vitamin C Tablets", "2025-03-08", 140, "2025-01-12", "Shelf F2"},
        {"B014", "Vitamin C Tablets", "2025-06-18", 160, "2025-03-01", "Shelf F2"},
        {"B015", "Insulin Vial", "2024-12-15", 30, "2024-11-25", "Refrigerator R2"},
        {"B016", "Insulin Vial", "2025-01-20", 45, "2024-12-28", "Refrigerator R2"}
    };
    
    // Load initial stock into heap and hash map
    for (const auto& batch : initialStock) {
        expiryHeap.push(batch);
        medicineStock[batch.medicineName] += batch.quantity;
    }
    
    cout << "=== Pharmacy Inventory Expiry Manager (Min-Heap + Hash Map) ===\n";
    cout << "16 batches loaded. FIFO expiry management active.\n\n";
    
    cout << "Commands:\n";
    cout << "  sell <medicine_name> <quantity>\n";
    cout << "    Example: sell Paracetamol 500mg 25\n";
    cout << "  add <batchID> <medicine> <expiry> <qty> <arrival> <location>\n";
    cout << "    Example: add B017 Paracetamol 500mg 2025-06-01 100 2025-03-15 Shelf A1\n";
    cout << "  stock <medicine_name>\n";
    cout << "    Example: stock Ibuprofen 400mg\n";
    cout << "  next_expiry\n";
    cout << "    Shows soonest expiring batch\n\n";
    
    cout << "Available medicines: ";
    for (const auto& med : medicineStock) {
        cout << med.first << " ";
    }
    cout << "\n\n";
    
    string command;
    while (true) {
        cout << "pharmacy> ";
        getline(cin, command);
        if (command.empty()) continue;
        
        stringstream ss(command);
        string type;
        ss >> type;
        
        if (type == "sell") {
            string medName;
            int qty;
            ss >> medName >> qty;
            
            if (medicineStock.find(medName) == medicineStock.end()) {
                cout << "❌ Medicine '" << medName << "' not in stock.\n";
                continue;
            }
            if (medicineStock[medName] < qty) {
                cout << "❌ Insufficient stock. Available: " << medicineStock[medName] << endl;
                continue;
            }
            
            // Sell from earliest expiry batches (FIFO)
            int remaining = qty;
            while (remaining > 0 && !expiryHeap.empty()) {
                MedicineBatch top = expiryHeap.top();
                if (top.medicineName != medName) {
                    expiryHeap.pop();
                    continue;
                }
                
                int sellQty = min(remaining, top.quantity);
                if (sellQty == top.quantity) {
                    expiryHeap.pop(); // Entire batch sold
                } else {
                    // Partial sale - update batch and push back
                    MedicineBatch updated = top;
                    updated.quantity -= sellQty;
                    expiryHeap.pop();
                    expiryHeap.push(updated);
                    break;
                }
                remaining -= sellQty;
                medicineStock[medName] -= sellQty;
            }
            
            cout << "✅ Sold " << qty << " units of '" << medName << "'. Remaining: " << medicineStock[medName] << endl;
            
        } else if (type == "add") {
            string batchID, medName, expiry, arrival, location;
            int qty;
            ss >> batchID >> medName >> expiry >> qty >> arrival >> location;
            
            MedicineBatch newBatch{batchID, medName, expiry, qty, arrival, location};
            expiryHeap.push(newBatch);
            medicineStock[medName] += qty;
            
            cout << "✅ Added batch " << batchID << " (" << qty << " units of '" << medName << "'). Total stock: " << medicineStock[medName] << endl;
            
        } else if (type == "stock") {
            string medName;
            getline(ss, medName);
            size_t start = medName.find_first_not_of(" \t");
            if (start != string::npos) medName = medName.substr(start);
            
            if (medicineStock.find(medName) != medicineStock.end()) {
                cout << "📦 '" << medName << "': " << medicineStock[medName] << " units available\n";
            } else {
                cout << "❌ '" << medName << "' not in inventory\n";
            }
            
        } else if (type == "next_expiry") {
            if (expiryHeap.empty()) {
                cout << "✅ No stock - inventory empty\n";
            } else {
                MedicineBatch next = expiryHeap.top();
                cout << "⏰ Soonest expiry: Batch " << next.batchID << " ('" << next.medicineName 
                     << "') expires " << next.expiryDate << " (Qty: " << next.quantity << ")\n";
            }
            
        } else if (type == "exit" || type == "quit") {
            break;
        } else {
            cout << "❌ Unknown command. Type 'sell', 'add', 'stock', 'next_expiry', or 'exit'\n";
        }
    }
    
    cout << "\n=== Session Complete ===\n";
    return 0;
}

