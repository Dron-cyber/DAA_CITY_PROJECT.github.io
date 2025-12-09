
#include <bits/stdc++.h>
using namespace std;

// Problem: Fire Incident Triage & Priority Management
// Algorithm: Max-Heap (priority_queue)
// Input: Number of incidents, then each incident with severity, occupancy, building_type (all integers)
// Output: Incidents dispatched in order of priority (highest first)

struct Incident {
    int severity;
    int occupancy;
    int building_type;
    int id; // incident id
    int risk_score() const {
        return severity*5 + occupancy*3 + building_type*2; // simple weighted score
    }
};

bool operator<(const Incident &a, const Incident &b) {
    return a.risk_score() < b.risk_score(); // Max-Heap: highest risk at top
}

int main() {
    ios::sync_with_stdio(false);
    cin.tie(nullptr);

    int n; // number of fire incidents
    cin >> n;
    priority_queue<Incident> pq;

    for(int i=1;i<=n;i++){
        int sev, occ, type;
        cin >> sev >> occ >> type;
        pq.push({sev, occ, type, i});
    }

    cout << "Dispatch order of incidents (highest priority first):\n";
    while(!pq.empty()){
        Incident inc = pq.top(); pq.pop();
        cout << "Incident " << inc.id << " with risk score " << inc.risk_score() << "\n";
    }

    return 0;
}
