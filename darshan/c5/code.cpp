#include <bits/stdc++.h>
using namespace std;

struct Patient {
    string name;
    int priority; 
};

struct Compare {
    bool operator()(Patient const &a, Patient const &b) {
        return a.priority < b.priority; // Max-Heap logic
    }
};

int main() {
    priority_queue<Patient, vector<Patient>, Compare> pq;
    int n, beds;

    cout << "Enter total beds available: ";
    cin >> beds;

    cout << "Enter number of patients: ";
    cin >> n;

    cout << "Enter Patient Name & Priority:\n";
    for(int i=0; i<n; i++){
        Patient p;
        cin >> p.name >> p.priority;
        pq.push(p);
    }

    cout << "\n--- Bed Allocation Order ---\n";
    for(int i=0; i<beds && !pq.empty(); i++) {
        Patient p = pq.top(); pq.pop();
        cout << "Bed Assigned to: " << p.name 
             << " (Priority: " << p.priority << ")\n";
    }

    cout << "\n--- Waiting Patients ---\n";
    while(!pq.empty()) {
        Patient p = pq.top(); pq.pop();
        cout << p.name << " (Priority: " << p.priority << ")\n";
    }

    return 0;
}
